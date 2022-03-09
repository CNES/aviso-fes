# C-API

Main routines for the FES prediction software.

## Typedefs

### FES

`FES` handler


```c
typedef void* FES
```

## Enumeration Type Documentation

### `fes_enum_access`

Possible type of access.

```c
typedef enum {
    FES_IO = 0,
    FES_MEM
} fes_enum_access;
```

<table>
<tr>
  <th>Enumerator</th>
  <th></th>
</tr>
<tr>
  <td>FES_IO</td>
  <td>Direct access (Grids are not loaded into memory)</td>
</tr>
<tr>
  <td>FES_MEM</td>
  <td>Memory access (Grids are loaded into memory)</td>
</tr>
</table>

### `fes_enum_error`

The result codes that can be returned by functions in the FES C SDK.

```c
typedef enum {
    FES_SUCCESS,
    FES_NO_MEMORY,
    FES_NETCDF_ERROR,
    FES_IO_ERROR,
    FES_INI_ERROR,
    FES_NO_DATA,
    FES_VALUE_ERROR
} fes_enum_error;
```

<table>
<tr>
  <th>Enumerator</th>
  <th></th>
</tr>
<tr>
  <td>FES_SUCCESS</td>
  <td>No error reporting</td>
</tr>
<tr>
  <td>FES_NO_MEMORY</td>
  <td>Not enough memory</td>
</tr>
<tr>
  <td>FES_NETCDF_ERROR</td>
  <td>NetCDF error</td>
</tr>
<tr>
  <td>FES_IO_ERROR</td>
  <td>IO error</td>
</tr>
<tr>
  <td>FES_INI_ERROR</td>
  <td>Invalid configuration file</td>
</tr>
<tr>
  <td>FES_NO_DATA</td>
  <td>No data available in grids for the location asked.</td>
</tr>
<tr>
  <td>FES_VALUE_ERROR</td>
  <td>Function receives an argument that has the right type but an
  inappropriate value.</td>
</tr>
</table>

### `enum fes_enum_tide_type`

Possible type of tide.

```c
typedef enum {
    FES_TIDE = 0,
    FES_RADIAL
} fes_enum_tide_type;
```

<table>
<tr>
  <th>Enumerator</th>
  <th></th>
</tr>
<tr>
  <td>FES_TIDE</td>
  <td>Ocean tide</td>
</tr>
<tr>
  <td>FES_RADIAL</td>
  <td>Radial tide</td>
</tr>
</table>

## Function Documentation

### `fes_core`

Driver for tidal computation.

```c
int fes_core(FES             handle,
             const double    lat,
             const double    lon,
             const double    time,
             double *        h,
             double *        h_long_period)
```

**Parameters**:

* **handle**(*FES*): FES handler
* **lat**(*const double*): Latitude in degrees (positive north) for the
  position at which tide is computed.
* **lon**(*const double*): Longitude in degrees for the position at which
  tide is computed.
* **time**(*const double*): Julian day (days since 1950-01-01 00:00:00.000
  UTC).
* **h**(*double**): Computed height of the diurnal and semi-diunral
  constituents of the tidal spectrum (in centimeters).
* **h_long_period**(*double**): Computed height of the long period wave
  constituents of the tidal spectrum (in centimeters).

**Returns**(*int*): 0 on success or 1 on failure.

> **_NOTE_**
>
> `h` is set to nan if no data is available at the given position.
> `h_long_period` is always computed because this value does not depend on
> input grids.

### `fes_delete`

Frees the memory used.

```c
void fes_delete(FES handle)
```

**Parameters**:

* **handle**(*FES*): FES handler

### `fes_errno`

Returns the last error code.

```c
fes_enum_error fes_errno(FES handle)
```

**Parameters**:

* **handle**(*FES*): FES handler

**Returns**(*fes_enum_error*): the error code.

### `fes_error`

Returns the message error.

```c
const char* fes_error(FES handle)
```

**Parameters**:

* **handle**(*FES*): FES handler

**Returns**(*const char**): the message error.

### `fes_min_number`

Return the minimum number of points used to interpolate the tidal waves for the
asked position.

```c
int fes_min_number(FES handle)
```

**Parameters**

* **handle**(*FES*): FES handler

**Returns**(*int*): the minimum number of points used during the interpolation

### `fes_new`

Initializes the computation of the tide.

```c
int fes_new(FES *                       handle,
            const fes_enum_tide_type    tide,
            const fes_enum_access       mode,
            const char *const           path)
```

**Parameters**:

* **handle**(*FES*): FES handler
* **tide**(*const fes_enum_tide_type*): Computation mode. If mode is equals
    to `FES_TIDE`, the core function computes the tide otherwise she computes
    the radial tide.
* **mode**(*const fes_enum_access*): One of `FES_MEM`, `FES_IO` which
    request loading grids into memory or direct access from NetCDF grids.
* **path**(*const char**): Path to the configuration file.

**Returns**(*int*): 0 on success or 1 on failure.

> **_NOTE_**
>
> The user can set the size of the buffer when the `FES_IO` mode is
> used, with the `FES_BUFFER_SIZE` environment variable. This variable must
> define a positive integer representing the size of the memory in Mb.

### `fes_set_buffer_size`

Sets the size of the buffer.

```c
int fes_set_buffer_size(FES	handle, const size_t size)
```

**Parameters**:

* **handle**(*FES*): FES handler
* **size**(*const size_t*): Size of the buffer in MB

**Returns**(*int*): 0 on success or 1 on failure.

### `fes_dump_template`

Dump the template of the configuration file that the library uses.

```c
int fes_dump_template(const char* path)
```

**Parameters**:

* **path**(*const char*): Path to the dump of the configuration file to
  create

**Returns**(*int*): 0 on success or 1 on failure.
