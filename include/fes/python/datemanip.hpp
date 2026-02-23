// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/python/datemanip.hpp
/// @brief Python date handling
#pragma once
#include <pybind11/pybind11.h>

#include <limits>
#include <string>
#include <tuple>
// clang-format off
#include <datetime.h>
// clang-format on

#include <cstdint>

namespace py = pybind11;

namespace fes {
namespace python {
namespace datemanip {

/// Returns number of days since civil 1970-01-01.  Negative values indicate
/// days prior to 1970-01-01.
///
/// http://howardhinnant.github.io/date_algorithms.html
constexpr auto days_from_civil(int y, unsigned m, unsigned d) -> int64_t {
  y -= static_cast<int>(m <= 2);
  const auto era = (y >= 0 ? y : y - 399) / 400;
  const auto yoe = static_cast<unsigned>(y - (era * 400));  // [0, 399]
  const auto doy =
      ((153 * (m + (m > 2 ? -3 : 9)) + 2) / 5) + d - 1;          // [0, 365]
  const auto doe = (yoe * 365) + (yoe / 4) - (yoe / 100) + doy;  // [0, 146096]
  return (era * 146097LL) + static_cast<int64_t>(doe) - 719468LL;
}

/// @brief Split the Python datetime object into two integers.
/// @param[in] datetime Datetime object to split.
/// @return Two integers: the number of seconds since the epoch and the number
/// of microseconds.
inline auto split_datetime(const py::handle& datetime)
    -> std::tuple<int64_t, int64_t> {
  auto* py_object = datetime.ptr();
  if (py_object == nullptr || py_object == Py_None) {
    throw std::invalid_argument(
        "a datetime.datetime is required (got type null)");
  }

  if (PyDateTimeAPI == nullptr) {
    PyDateTime_IMPORT;
  }

  if (PyDateTimeAPI == nullptr) {
    throw std::runtime_error("unable to import the datetime module");
  }

  if (PyDateTime_Check(py_object)) {
#if PY_VERSION_HEX >= 0x030A0000
    // Public C-API macro (no cast needed)
    if (PyDateTime_DATE_GET_TZINFO(py_object) != Py_None) {
      throw std::invalid_argument(
          "only the naive datetime object can be converted to timestamp");
    }
#else
    // Portable fallback for older Python versions
    PyObject* tzinfo = PyObject_GetAttrString(py_object, "tzinfo");
    if (tzinfo == nullptr) {
      throw py::error_already_set();
    }
    const bool has_tzinfo = (tzinfo != Py_None);
    Py_DECREF(tzinfo);
    if (has_tzinfo) {
      throw std::invalid_argument(
          "only the naive datetime object can be converted to timestamp");
    }
#endif

    auto sec = days_from_civil(PyDateTime_GET_YEAR(py_object),
                               PyDateTime_GET_MONTH(py_object),
                               PyDateTime_GET_DAY(py_object)) *
               86'400;
    sec += PyDateTime_DATE_GET_HOUR(py_object) * 3'600 +
           PyDateTime_DATE_GET_MINUTE(py_object) * 60 +
           PyDateTime_DATE_GET_SECOND(py_object);
    if (std::abs(sec) > std::numeric_limits<int32_t>::max()) {
      throw std::invalid_argument("timestamp out of range for platform time_t");
    }
    return std::make_tuple(sec, PyDateTime_DATE_GET_MICROSECOND(py_object));
  }
  throw std::invalid_argument(
      "a datetime.datetime is required (got type " +
      std::string(py::str(py::type::of(datetime).attr("__name__"))) + ")");
}

/// @brief Return POSIX timestamp as int64_t
/// @param[in] datetime Datetime object to convert.
/// @return POSIX timestamp as int64_t.
auto inline as_int64(const py::handle& datetime) -> int64_t {
  auto sec_usec = split_datetime(datetime);
  return (std::get<0>(sec_usec) * 1'000'000) + std::get<1>(sec_usec);
}

/// @brief Return POSIX timestamp as float64
/// @param[in] datetime Datetime object to convert.
/// @return POSIX timestamp as float64.
auto inline as_float64(const py::handle& datetime) -> double {
  auto sec_usec = split_datetime(datetime);
  return static_cast<double>(std::get<0>(sec_usec)) +
         (static_cast<double>(std::get<1>(sec_usec)) * 1e-6);
}

}  // namespace datemanip
}  // namespace python
}  // namespace fes
