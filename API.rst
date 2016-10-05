C-API
#####

Main routines for the FES prediction software.

Typedefs
========

FES
---

FES handler

|

.. sourcecode:: C

    typedef void* FES

Enumeration Type Documentation
==============================

enum fes_enum_access
--------------------

Possible type of access.

|

.. sourcecode:: C

    typedef enum {
      FES_IO = 0,
      FES_MEM
    } fes_enum_access;

|

+------------+--------------------------------------------------+
| Enumerator |                                                  |
+============+==================================================+
| FES_IO     | Direct access (Grids are not loaded into memory) |
+------------+--------------------------------------------------+
| FES_IO     | Memory access (Grids are loaded into memory)     |
+------------+--------------------------------------------------+

``enum fes_enum_error``
-----------------------

The result codes that can be returned by functions in the FES C SDK.

|

.. sourcecode:: C

   typedef enum {
     FES_SUCCESS,
     FES_NO_MEMORY,
     FES_NETCDF_ERROR,
     FES_IO_ERROR,
     FES_INI_ERROR,
     FES_NO_DATA,
     FES_VALUE_ERROR
   } fes_enum_error;

|

+------------------+--------------------------------------------+
| Enumerator       |                                            |
+==================+============================================+
| FES_SUCCESS      | No error reporting                         |
+------------------+--------------------------------------------+
| FES_NO_MEMORY    | Not enough memory                          |
+------------------+--------------------------------------------+
| FES_NETCDF_ERROR | netCDF error                               |
+------------------+--------------------------------------------+
| FES_IO_ERROR     | IO error                                   |
+------------------+--------------------------------------------+
| FES_INI_ERROR    | Invalid configuration file                 |
+------------------+--------------------------------------------+
| FES_NO_DATA      | No data available in grids for the         |
|                  | location asked                             |
+------------------+--------------------------------------------+
| FES_VALUE_ERROR  | Function receives an argument that has the |
|                  | right type but an inappropriate value      |
+------------------+--------------------------------------------+

``enum fes_enum_tide_type``
---------------------------

Possible type of tide.

|

.. sourcecode:: C

   typedef enum {
     FES_TIDE = 0,
     FES_RADIAL
   } fes_enum_tide_type;

|

+------------+-------------+
| Enumerator |             |
+============+=============+
| FES_TIDE   | Ocean tide  |
+------------+-------------+
| FES_RADIAL | Radial tide |
+------------+-------------+

Function Documentation
======================

``fes_core``
------------

Driver for tidal computation.

|

.. sourcecode:: C

    int fes_core(FES             handle,
                 const double    lat,
                 const double    lon,
                 const double    time,
                 double *        h,
                 double *        h_long_period)

**Parameters**:

    **handle**\(*FES*): FES handler

    **lat**\(*const double*): Latitude in degrees (positive north) for the
    position at which tide is computed.

    **lon**\(*const double*): Longitude in degrees for the position at which
    tide is computed.

    **time**\(*const double*): Julian day (days since 1950-01-01 00:00:00.000
    UTC).

    **h**\(*double**): Computed height of the diurnal and semi-diunral
    constituents of the tidal spectrum (in centimeters).

    **h_long_period**\(*double**): Computed height of the long period wave
    constituents of the tidal spectrum (in centimeters).

**Returns**: 0 on success or 1 on failure.

**Type**: int

.. note:: h is set to nan if no data is available at the given position.
  h\_long\_period is always computed because this value does not depend on input
  grids.

``fes_delete``
--------------

Frees the memory used.

|

.. sourcecode:: C

    void fes_delete(FES handle)

**Parameters**:

    **handle**\(*FES*): FES handler


``fes_errno``
-------------

Returns the last error code.

|

.. sourcecode:: C

    fes_enum_error fes_errno(FES handle)

**Parameters**:

    **handle**\(*FES*): FES handler

**Returns**: the error code.

**Type**: fes_enum_error

``fes_error``
-------------

Returns the message error.

|

.. sourcecode:: C

    const char* fes_error(FES handle)

**Parameters**:

    **handle**\(*FES*): FES handler

**Returns**: the message error.

**Type**: const char*

``fes_min_number``
------------------

Return the minimum number of points used to interpolate the tidal waves for the
asked position.

|

.. sourcecode:: C

    int fes_min_number(FES handle)

**Parameters**:

    **handle**\(*FES*): FES handler

**Returns**: the minimum number of points used during the interpolation

**Type**: int

``fes_new``
-----------

Initializes the computation of the tide.

|

.. sourcecode:: C

    int fes_new(FES *                       handle,
                const fes_enum_tide_type    tide,
                const fes_enum_access       mode,
                const char *const           path)

**Parameters**:

    **handle**\(*FES*): FES handler

    **tide**\(*const fes_enum_tide_type*): Computation mode. If mode is equals
    to ``FES_TIDE``, the core function computes the tide otherwise she computes
    the radial tide.

    **mode**\(*const fes_enum_access*):    One of ``FES_MEM``, ``FES_IO`` which
    request loading grids into memory or direct access from NetCDF grids.

    **path**\(*const char**): Path to the configuration file.

**Returns**: 0 on success or 1 on failure.

**Type**: int

.. note:: The user can set the size of the buffer when the ``FES_IO`` mode is
  used, with the ``FES_BUFFER_SIZE`` environment variable. This variable must
  define a positive integer representing the size of the memory in Mb.

``fes_set_buffer_size``
-----------------------

Sets the size of the buffer.

|

.. sourcecode:: C

    int fes_set_buffer_size(FES	handle, const size_t size)

**Parameters**:

    **handle**\(*FES*): FES handler

    **size**\(*const size_t*): Size of the buffer in MB

**Returns**: 0 on success or 1 on failure.

**Type**: int
