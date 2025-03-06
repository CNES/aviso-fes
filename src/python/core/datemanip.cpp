// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/python/datemanip.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

auto init_datemanip(py::module& m) -> void {
  m.def("as_int64", &fes::python::datemanip::as_int64, py::arg("date"),
        R"__doc__(
Convert a datetime.datetime object to an int64_t timestamp. The timestamp is
the number of microseconds since the epoch.

Args:
    date (datetime.datetime): The date to convert.

Returns:
    The timestamp.
)__doc__")
      .def("as_float64", &fes::python::datemanip::as_float64, py::arg("date"),
           R"__doc__(
Convert a datetime.datetime object to a float64_t timestamp. The timestamp is
the number of seconds since the epoch. Fractions of seconds are kept.

Args:
    date (datetime.datetime): The date to convert.

Returns:
    The timestamp.
)__doc__");
}  // namespace fes::python
