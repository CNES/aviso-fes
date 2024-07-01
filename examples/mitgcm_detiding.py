"""
************************************
Tidal Waves Analysis in MITGCM Model
************************************
"""
# %%
import dask.array
import dask.array.core
import dask.distributed
import dask.utils
import dask_kubernetes
import intake
import matplotlib.pyplot
import numpy
import pyfes

# %%
# Definition of the calculation period of the analysis (the spin-up period is
# not included).
START_DATE = numpy.datetime64('2011-11-13')
END_DATE = numpy.datetime64('2012-11-12')

# %%
# Loading the dataset.
cat_url = ('https://raw.githubusercontent.com/pangeo-data/pangeo-datastore/'
           'master/intake-catalogs/ocean/llc4320.yaml')
cat = intake.Catalog(cat_url)

ssh = cat.LLC4320_SSH.to_dask()
ssh

# %%
# Selection of the computation time series (the spin-off period of the model is
# skipped).
time_series = ssh.time.values
period = (time_series >= START_DATE) & (time_series <= END_DATE)
print('number of layers to process %d' % len(time_series[period]))
print('period [%s, %s]' %
      (time_series[period].min(), time_series[period].max()))

# %%
# Creation of the object controlling the harmonic analysis of the waves M2, K1,
# O1, P1, S1, S2.
wave_table = pyfes.WaveTable(['M2', 'K1', 'O1', 'P1', 'S1', 'S2'])
print('%d tidal constituents to be analysed' % len(wave_table))

# %%
# Creation of the Dask cluster.

cluster = dask_kubernetes.KubeCluster()
cluster.adapt(minimum=10, maximum=40)
client = dask.distributed.Client(cluster)
cluster

# %%


def compute_nodal_modulations(client, waves, time_series):
    """Compute nodal corrections for a given time series."""
    t = time_series.astype('datetime64[s]')
    f, v0u = waves.compute_nodal_modulations(t, pyfes.get_leap_seconds(t))
    return (dask.array.from_delayed(client.scatter(f, broadcast=True),
                                    shape=f.shape,
                                    dtype=f.dtype),
            dask.array.from_delayed(client.scatter(v0u, broadcast=True),
                                    shape=v0u.shape,
                                    dtype=v0u.dtype))


# %%
# Calculation of nodal corrections for the selected time series.
f, v0u = compute_nodal_modulations(client, wave_table, time_series[period])


# %%
def load_faces(ds, face, period, indices=None):
    """Load a face from the time series."""
    if indices is None:
        indices = slice(0, None, 1)
    ds = ds.Eta
    ds = ds.transpose('face', 'j', 'i', 'time')
    return ds.isel(face=face, time=period, i=indices, j=indices).data


# %%
# The calculation takes a little time. In order to make it faster, the processed
# grid is sub-sampled. If you want to process it completely, replace the
# following line with ``indices=None``.
indices = slice(0, None, 8)

# %%
# In this example, we will analyze the part of the grid representing Europe
# (face=2). Since the tide is strong, near the northwest coast, the effects of
# the tide in this area can be better illustrated.
face = 2

ds = load_faces(ssh, face, period, indices=indices)
ds


# %%
def dask_array_rechunk(da, axis=2):
    """Search for the optimal block cutting without modifying the axis 'axis'
    in order to optimize its access in memory."""
    nblocks = 1

    def calculate_chuncks_size(chunks, size):
        result = numpy.array(chunks).prod() * size
        return result / (1000**2)

    while True:
        chunks = []
        div = int(numpy.sqrt(nblocks))
        for index, item in enumerate(da.chunks):
            chunks.append(
                numpy.array(item).sum() * (div if index == axis else 1))
        chunks = tuple(item // div for index, item in enumerate(chunks))
        chuncks_size = calculate_chuncks_size(chunks, da.dtype.itemsize)
        if chuncks_size > 100 and chuncks_size < 150:
            return chunks
        nblocks += 1


ds = ds.rechunk(dask_array_rechunk(ds))
ds

# %%
# Normally, we just have to call the function ``dask.array.apply_along_axis``.
# This function tries to validate the arguments by executing once the function
# to be vectorized with arbitrary values. However, it does not work here,
# because one of our parameters is a matrix. To avoid this problem, the function
# has been copied and modified in this notebook.


def _apply_along_axis(arr, func1d, func1d_axis, func1d_args, func1d_kwargs):
    """Wrap apply_along_axis."""
    return numpy.apply_along_axis(func1d, func1d_axis, arr, *func1d_args,
                                  **func1d_kwargs)


def apply_along_axis(func1d, axis, arr, *args, **kwargs):
    """Apply the harmonic analysis to 1-D slices along the given axis."""
    arr = dask.array.core.asarray(arr)

    # Validate and normalize axis.
    arr.shape[axis]
    axis = len(arr.shape[:axis])

    # Rechunk so that analyze is applied over the full axis.
    arr = arr.rechunk(arr.chunks[:axis] + (arr.shape[axis:axis + 1], ) +
                      arr.chunks[axis + 1:])

    # Test out some data with the function.
    test_data = numpy.ones(args[0].shape[1], dtype=arr.dtype)
    test_result = numpy.array(func1d(test_data, *args, **kwargs))

    # Map analyze over the data to get the result
    # Adds other axes as needed.
    result = arr.map_blocks(
        _apply_along_axis,
        name=dask.utils.funcname(func1d) + '-along-axis',
        dtype=test_result.dtype,
        chunks=(arr.chunks[:axis] + test_result.shape + arr.chunks[axis + 1:]),
        drop_axis=axis,
        new_axis=list(range(axis, axis + test_result.ndim, 1)),
        func1d=func1d,
        func1d_axis=axis,
        func1d_args=args,
        func1d_kwargs=kwargs,
    )

    return result


# %%
# Finally, we perform our harmonic analysis on the face of the model loaded in
# memory.
#
# .. note::
#
#   To analyze the entire time series, we would have to loop on the 13 faces
#   storing the different geographical areas of the Earth.
future = apply_along_axis(pyfes.WaveTable.harmonic_analysis, 2, ds, *(f, v0u))
analysis = future.compute()

# %%
# The result tensor is arranged in order to place the estimated tidal waves on
# the first dimension.
analysis = numpy.transpose(analysis, [2, 0, 1])

# %%
# Visualization of the amplitude and phase calculated for the M2 wave.
#
# The tide present in the model is now calculated to correct it.
amp = numpy.absolute(analysis[0, :, :])
pha = numpy.angle(analysis[0, :, :], deg=True)

fig, (ax_pha, ax_amp) = matplotlib.pyplot.subplots(1, 2, figsize=(24, 8))

c = ax_pha.pcolormesh(pha, vmin=-180, vmax=180, cmap='bwr')
fig.colorbar(c, ax=ax_pha)
ax_pha.set_title('M2 phase (deg)')

c = ax_amp.pcolormesh(amp, vmin=-0.15, vmax=2, cmap='jet')
fig.colorbar(c, ax=ax_amp)
ax_amp.set_title('M2 amplitude (m)')

# %%
del amp
del pha

# %%

nwaves, ni, nj = analysis.shape
ts = time_series[0].astype('datetime64[s]')
tide = wave_table.tide_from_mapping(
    ts.astype('float64'),
    pyfes.get_leap_seconds(ts),
    analysis.reshape(nwaves, ni * nj),
).reshape(ni, nj)

# %%
total_ssh = ssh.Eta.sel(face=face, time=time_series[0])[indices, indices]
corrected_ssh = total_ssh - tide

# %%
# Finally, we can visualize the results of the analysis performed. The first
# figure shows the total SSH of the model, the second figure shows the SSH
# corrected by the tidal waves, and the third figure shows the tide height
# estimated by the analysis.
fig, (ax1, ax2, ax3) = matplotlib.pyplot.subplots(1, 3, figsize=(18, 6))

ax1.pcolormesh(total_ssh, cmap='jet')
ax1.set_title('Total SSH')
ax2.pcolormesh(corrected_ssh, cmap='jet')
ax2.set_title('SSH corrected')
ax3.pcolormesh(tide, cmap='jet')
ax3.set_title('Tide estimated')
