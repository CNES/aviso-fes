cimport numpy
cimport cpython.datetime
cimport libc
cimport libc.string


cdef extern from "sys/types.h" nogil:
    ctypedef long int time_t


cdef extern from "time.h" nogil:
    struct tm:
        int tm_sec
        int tm_min
        int tm_hour
        int tm_mday
        int tm_mon
        int tm_year
    time_t timegm(tm*)


# FES API declaration
cdef extern from "fes.h" nogil:
    ctypedef enum fes_enum_access:
        FES_IO
        FES_MEM
    ctypedef enum fes_enum_tide_type:
        FES_TIDE
        FES_RADIAL
    ctypedef enum fes_enum_error:
      FES_SUCCESS,
      FES_NO_MEMORY,
      FES_NETCDF_ERROR,
      FES_IO_ERROR,
      FES_INI_ERROR,
      FES_NO_DATA,
      FES_VALUE_ERROR
    int fes_new(void**, fes_enum_tide_type, fes_enum_access, char*)
    int fes_set_buffer_size(void*, size_t)
    void fes_delete(void*)
    int fes_core(void*, double, double, double, double*, double*)
    int fes_min_number(void*)
    char* fes_error(void*)
    fes_enum_error fes_errno(void*)


# Converts a python datetime object into a Julian day CNES
cdef inline double cnes_julian_day_from_datetime(
        cpython.datetime.datetime datetime):
    cdef:
        tm time_map
        time_t microseconds
        double result

    libc.string.memset(&time_map, 0, sizeof(tm));
    time_map.tm_year = cpython.datetime.PyDateTime_GET_YEAR(datetime) - 1900
    time_map.tm_mon  = cpython.datetime.PyDateTime_GET_MONTH(datetime) - 1
    time_map.tm_mday = cpython.datetime.PyDateTime_GET_DAY(datetime)
    time_map.tm_hour = cpython.datetime.PyDateTime_DATE_GET_HOUR(datetime)
    time_map.tm_min  = cpython.datetime.PyDateTime_DATE_GET_MINUTE(datetime)
    time_map.tm_sec  = cpython.datetime.PyDateTime_DATE_GET_SECOND(datetime)
    microseconds     = cpython.datetime.PyDateTime_DATE_GET_MICROSECOND(datetime)

    result = ((timegm(&time_map) + microseconds * 1e-6) / 86400) + 7305

    return result
