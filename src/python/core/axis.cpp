// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/axis.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/optional.hpp"

namespace py = pybind11;

void init_axis(py::module& m) {
  py::class_<fes::Axis, std::shared_ptr<fes::Axis>>(m, "Axis",
                                                    R"__doc__(
A coordinate axis a variable that specifies one of the coordinates
of a variable's values.
)__doc__")
      .def(py::init<const Eigen::Ref<const Eigen::VectorXd>&, double, bool>(),
           py::arg("points"), py::arg("epsilon") = 1e-6,
           py::arg("is_circular") = false,
           R"__doc__(
Default constructor.

Args:
     points: The axis points.
     epsilon: The tolerance used to determine if the axis is circular.
     is_circular: True if the axis is circular. For example,
          longitude is circular.
)__doc__")
      .def("__eq__", &fes::Axis::operator==, py::arg("other"),
           "Compare two axes for equality.")
      .def("__len__", &fes::Axis::size, "Return the size of the axis.")
      .def("__getitem__", &fes::Axis::operator(), py::arg("index"),
           "Return the value of the axis at the given index.")
      .def_property_readonly("start", &fes::Axis::start,
                             "Return the first value of the axis.")
      .def_property_readonly("step", &fes::Axis::step,
                             "Return the step of the axis.")
      .def_property_readonly("is_ascending", &fes::Axis::is_ascending,
                             "Return true if the axis is ascending.")
      .def_property_readonly("is_circular", &fes::Axis::is_circular,
                             "Return true if the axis is circular.")
      .def("end", &fes::Axis::end, "Return the last value of the axis.")
      .def("min_value", &fes::Axis::min_value,
           "Return the minimum value of the axis.")
      .def("max_value", &fes::Axis::max_value,
           "Return the maximum value of the axis.")
      .def("find_index", &fes::Axis::find_index, py::arg("value"),
           py::arg("bounded") = true,
           R"__doc__(
Find the index of the axis point closest to the given value.

Args:
    value: The value to find the index for.
    bounded: If true, the index will be bounded by the axis domain.

Returns:
    The index of the axis point closest to the given value.
)__doc__")
      .def("find_indices", &fes::Axis::find_indices, py::arg("values"),
           R"__doc__(
Find the indices of the axis points closest to the given values.

Args:
    values: The values to find the indices for.

Returns:
    The indices of the axis points closest to the given values.
)__doc__");
}
