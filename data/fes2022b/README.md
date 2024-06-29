# CONTENTS

## Configuration Files

This directory contains the different configuration files to compute:

* FES2022 elevations (ocean or geocentric) - version B
  * Use [ocean_tide.yaml](ocean_tide.yaml) to compute the ocean tide
    with the native grids.
  * Use [ocean_tide_extrapolated.yaml](ocean_tide_extrapolated.yaml)
    to compute the ocean tide with the extrapolated grids.
  * Use [load_tide.yaml](load_tide.yaml) to compute the load tide.
  * Use [fes2022.yaml](fes2022.yaml)
    to compute the geocentric tide with the extrapolated grids

> **_NOTE_**
>
> The configuration files are supplied for use with the tree as described
> below.
>
> ```
> data/
> └── fes2022b
>     ├── load_tide
>     ├── ocean_tide
>     └── ocean_tide_extrapolated
>```

## Database Contents

The various sub directories located here are empty. Indeed, data necessary to
calculate the different tides are not distributed here, but on the
[AVISO](http://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes/)
website. You will find on the AVISO FTP server authenticated the following
archives for filling empty directories:

- load_tide/*.nc.xz
- ocean_tide/*.nc.xz
- ocean_tide_extrapolated/*.nc.xz

To allow you to download as small as possible archives, the files have been
compressed using the [xz](http://tukaani.org/xz) program using the LZMA2
compression algorithms. To decompress the files, you must use this command or
[7-zip](http://www.7-zip.org/) if you use Windows.

> **_NOTE_**
>
> The decompression of these archives will need 18 GB of memory.

## Database description

The FES2022 tides database includes 3 components: Tide elevations, Extrapolated Tide elevations and Tide loading. Each component contains 34 tidal components:

    2N₂, J₁, K₁, K₂, L₂, MKS₂, MN₄, MS₄, Mf, Mm, Msf, Msqm, Mtm, M₂, M₃, M₄, M₆, M₈, N₂, N₄, O₁, P₁, P₁, R₂, Sa, Ssa, S₁, S₂, S₄, T₂, µ₂, ε₂, λ₂, ν₂

### Tide Elevations

FES2022B ocean tide model is distributed; this model has
been computed using the new FES2022 tidal loading grids. Two versions
are available:

  * Non extrapolated version contained in the `ocean_tide` directory: the
    model is defined on a 1/30° grid directly
    interpolated from the finite element native grid of the model.

  * Extrapolated version contained in the `ocean_tide_extrapolated` directory: the
    above version has been extrapolated on the coasts in order to cover nearly
    entirely the coastal regions. The procedure is a simple extrapolation
    applied on 20 pixels and using mean values when necessary, it can be not
    accurate enough in some regions.
    A specific mask file is also provided to differentiate native data and extrapolated data
    for FES2022B tide elevation grids: mask_fes2022B.nc

### Tide Loading

The new FES2022 tide loading is available on the same 1/30° grid. This
dataset is provided into the `load_tide` directory.
