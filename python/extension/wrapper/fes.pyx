cimport cpython.datetime
cimport cython
cimport libc.math
cimport libc.time
cimport numpy
cimport fes


numpy.import_array()


# Datetime C API initialization
cpython.datetime.import_datetime()


# Declares the necessary types to get information of a numpy array that
# contains dates.
cdef extern from "numpy/arrayobject.h":
    ctypedef enum NPY_DATETIMEUNIT:
        NPY_FR_Y
        NPY_FR_M
        NPY_FR_W
        NPY_FR_D
        NPY_FR_h
        NPY_FR_m
        NPY_FR_s
        NPY_FR_ms
        NPY_FR_us
        NPY_FR_ns
        NPY_FR_ps
        NPY_FR_fs
        NPY_FR_as
        NPY_FR_GENERIC

    ctypedef struct NpyAuxData:
        pass

    ctypedef struct PyArray_Descr:
        NpyAuxData* c_metadata

    ctypedef struct PyArrayObject:
        pass

    ctypedef struct PyArray_DatetimeMetaData:
        NPY_DATETIMEUNIT base

    ctypedef struct PyArray_DatetimeDTypeMetaData:
        PyArray_DatetimeMetaData meta

    PyArray_Descr* PyArray_DESCR(PyArrayObject*)

    int PyDataType_ISDATETIME(PyArrayObject*)


# Translates a NPY_DATETIMEUNIT value into a string
cdef char* ndarray_date_unit(NPY_DATETIMEUNIT unit):
    if unit == NPY_FR_Y:
        return "years"
    elif unit == NPY_FR_M:
        return "months"
    elif unit == NPY_FR_W:
        return "weeks"
    elif unit == NPY_FR_D:
        return "days"
    elif unit == NPY_FR_h:
        return "hours"
    elif unit == NPY_FR_m:
        return "minutes"
    elif unit == NPY_FR_s:
        return "seconds"
    elif unit == NPY_FR_ms:
        return "milliseconds"
    elif unit == NPY_FR_us:
        return "microseconds"
    elif unit == NPY_FR_ns:
        return "nanoseconds"
    elif unit == NPY_FR_ps:
        return "picoseconds"
    elif unit == NPY_FR_fs:
        return "femtoseconds"
    elif unit == NPY_FR_as:
        return "attoseconds"
    elif unit == NPY_FR_GENERIC:
        return "generic"
    else:
        raise RuntimeError("unit unknown %d" % unit)

# Checks that the numpy ndarray given contains dates with an expected date
# unit.
cdef ndarray_check_date_unit(object array, NPY_DATETIMEUNIT unit):
    cdef:
        PyArray_Descr* dtype
        PyArray_DatetimeMetaData metadata
        PyArray_DatetimeDTypeMetaData* dtype_metadata

    dtype = PyArray_DESCR(<PyArrayObject*>(array))
    if not PyDataType_ISDATETIME(<PyArrayObject*>(dtype)):
        raise ValueError(
            "cannot get datetime metadata from non-datetime type")

    dtype_metadata = <PyArray_DatetimeDTypeMetaData*>(dtype[0].c_metadata)
    metadata = dtype_metadata.meta
    if metadata.base != unit:
        raise ValueError(
            "array has wrong datetime unit, expected %r got %r" % (
                ndarray_date_unit(unit), ndarray_date_unit(metadata.base)))


cdef class Handler:
    """
    FES handler

    :param tide: Computation mode. If tide is equals to "tide", the handler
        computes the tide, if modeshe computes the radial tide.
    :type tide: str
    :param mode: One of 'memory', 'io' which request loading grids into
        memory or direct access from NetCDF grids.
    :type mode: str
    :param path: Path to the configuration file.
    :type path: str
    """
    cdef void* wrapped

    cdef _check(self, int rc):
        cdef:
            const char* c_str
            bytes py_bytes;

        if rc:
            c_str = fes.fes_error(self.wrapped)
            py_bytes = c_str;
            raise RuntimeError(py_bytes.decode())

    def __cinit__(self, str tide not None, str mode not None, str path not None):
        cdef:
            fes.fes_enum_tide_type c_tide
            fes.fes_enum_access c_mode
            const char* c_str
            bytes py_bytes;

        if tide == 'ocean':
            c_tide = fes.FES_TIDE
        elif tide == 'radial':
            c_tide = fes.FES_RADIAL
        else:
            raise ValueError("tide must be ocean or radial not %r" % tide)

        if mode == 'io':
            c_mode = fes.FES_IO
        elif mode == 'memory':
            c_mode = fes.FES_MEM
        else:
            raise ValueError("mode must be io or memory not %r" % mode)

        py_bytes = path.encode()
        c_str = py_bytes

        self._check(
            fes.fes_new(cython.address(self.wrapped), c_tide, c_mode, c_str))

    def __dealloc__(self):
        fes.fes_delete(self.wrapped)

    def scalar(self,
               double lat,
               double lon,
               cpython.datetime.datetime date not None):
        """
        Tidal computation for scalar values

        :param lat: Latitude in degrees (positive north) for the position
            at which tide is computed.
        :type lat: float
        :param lon: Longitude in degrees for the position at which tide is
            computed.
        :type lon: float
        :param date: Date at which tide is computed
        :type date: datetime.datetime
        :return: a tuple that contains:
            * Computed height of the diurnal and
                semi-diunral constituents of the tidal spectrum
                (in centimeters) or None if no data is available
                at the given position.
            * Computed height of the long period wave constituents of
                the tidal spectrum (in centimeters). This value is always
                computed because this value does not dependon input grids.
        :rtype: tuple
        """
        cdef:
            int rc
            double h
            double h_long_period

        rc = fes.fes_core(self.wrapped,
                          lat,
                          lon,
                          fes.cnes_julian_day_from_datetime(date),
                          &h,
                          &h_long_period)
        if rc:
            if fes.fes_errno(self.wrapped) == fes.NO_DATA:
                return (None, h_long_period)
            else:
                self._check(rc)
        return (h, h_long_period)

    def set_buffer_size(self, int size):
        """
        Sets the size of the read buffer

        :param size: Size of the buffer in MB
        :type size: int
        """
        self._check(fes.fes_set_buffer_size(self.wrapped, size))

    def min_number(self):
        """
        Gets the minimum number of points used during the interpolation

        :return: TODO
        :rtype: int
        """
        return fes.fes_min_number(self.wrapped)

    @cython.boundscheck(False)
    def vector(self,
               numpy.float64_t[:] lats not None,
               numpy.float64_t[:] lons not None,
               numpy.ndarray times not None):
        """
        Tidal computation for vector values

        :param lat: Latitude in degrees (positive north) for the position
            at which tide is computed.
        :type lat: numpy.ndarray
        :param lon: Longitude in degrees for the position at which tide is
            computed.
        :type lon: numpy.ndarray
        :param date: Date at which tide is computed
        :type date: numpy.ndarray
        :return: a tuple that contains:
            * Computed height of the diurnal and
                semi-diunral constituents of the tidal spectrum
                (in centimeters) or None if no data is available
                at the given position.
            * Computed height of the long period wave constituents of
                the tidal spectrum (in centimeters). This value is always
                computed because this value does not dependon input grids.
        :rtype: tuple
        """
        cdef:
            int ix
            numpy.npy_intp dims[1]
            double a
            double b

        if lats.shape[0] != lons.shape[0]:
            raise ValueError(
                "lats, lons could not be broadcast together with "
                "shapes (%d,) (%d,)" % (lats.shape[0], lons.shape[0]))

        if lats.shape[0] != times.shape[0]:
            raise ValueError(
                "lats, times could not be broadcast together with "
                "shapes (%d,) (%d,)" % (lats.shape[0], times.shape[0]))

        # Checks the type of vector containing the dates.
        ndarray_check_date_unit(times, NPY_FR_us)
        cdef numpy.int64_t[:] view = times.view('i8')

        # Construction of vectors results.
        dims[0] = lats.shape[0]

        cdef numpy.ndarray h = numpy.PyArray_EMPTY(
            1, dims, numpy.NPY_FLOAT64, 0)
        cdef numpy.ndarray h_long_period = numpy.PyArray_EMPTY(
            1, dims, numpy.NPY_FLOAT64, 0)

        cdef numpy.float64_t[:] vh = h
        cdef numpy.float64_t[:] vh_long_period = h_long_period

        # Compute tide
        for ix in range(dims[0]):
            rc = fes.fes_core(self.wrapped,
                              lats[ix],
                              lons[ix],
                              (view[ix] * 1e-6) / 86400 + 7305,
                              &vh[ix],
                              &vh_long_period[ix])

            if rc:
                if fes.fes_errno(self.wrapped) == fes.FES_NO_DATA:
                    vh[ix] = libc.math.nan("NaN")
                else:
                    self._check(rc)

        return (h, h_long_period)


