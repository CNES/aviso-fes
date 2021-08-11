# API for Python

The Python library provides an interface to control the C API library.

## ``fes`` objects

*class* **fes.Handler** (*tide: str, mode: str, path: str*)

  Creates a new FES handler.

  * ``tide`` specifies the tide computation mode : **tide** to compute
    the short tide or ``radial`` to compute the radial tide.
  * ``mode`` drives the memory management of the library :
    ``io`` no data are loaded into memory (data for the calculation will
    be read from the netCDF grids if necessary) or ``memory`` to load
    all NetCDF grids into memory.
  * ``path`` defines the path to the configuration file to use.

> **_NOTE_**
>
>    If you want to use the library in a multi-threaded environment it
>    is necessary to create as many ``Handler`` that you have threads.

> **_WARNING_**
>
>    Since the C library is not "thread-safe", the Python class uses a
>    synchronization method to make it "thread-safe": an instance cannot
>    perform more than one task at a time within several threads.

### Instance methods:

**fes.Handler.calculate** (*lon: numpy.ndarray, lat: numpy.ndarray, date: numpy.ndarray*) -> tuple

  Tidal computation

  * ``lon`` is the longitude in degrees for the position at which tide is
    computed.
  * ``lat`` is the latitude in degrees (positive north) for the position
    at which tide is computed.
  * ``date`` date at which tide is computed.

    > **_NOTE_**
    >
    > This numpy array can contain objects of type ``datetime.datetime`` or
    > numpy dates of type ``datetime64[us]``.

  Returns a tuple that contains:

  * Computed height of the diurnal and
    semi-diurnal constituents of the tidal spectrum
    (in centimeters) or None if no data is available
    at the given position.
  * Computed height of the long period wave constituents of
    the tidal spectrum (in centimeters). This value is always
    computed because this value does not depend on input grids.
  * The minimum number of points used to interpolate the tidal waves for the
    asked positions.

  > **_NOTE_**:
  >
  > The method returns "NaN" if the numerical grids defining tida waves
  > are not undefined.

**fes.Handler.set_buffer_size** (*size: int*)

Fixed size, in MB, of read buffer in case read mode selected is "io".

**fes.Handler.dump** (*path: str*)

Dump the configuration file used in the given path.
