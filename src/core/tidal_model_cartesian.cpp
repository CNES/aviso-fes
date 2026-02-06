// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/interface/tidal_model.hpp"
#include "fes/tidal_model/cartesian.hpp"
#include "pybind11/cast.h"

namespace py = pybind11;

namespace fes {

template <typename T>
void init_cartesian_model(py::module& m, const std::string& suffix) {
  py::class_<tidal_model::Cartesian<T>, TidalModelInterface<T>,
             std::shared_ptr<fes::tidal_model::Cartesian<T>>>(
      m, ("Cartesian" + suffix).c_str(),
      "A tidal model that uses a Cartesian grid to store the wave models.")
      .def(py::init<Axis, Axis, TideType, bool>(), py::arg("lon"),
           py::arg("lat"), py::arg("tide_type") = kTide,
           py::arg("longitude_major") = true,
           R"__doc__(
Construct a Cartesian tidal model.

Args:
     lon: The longitude axis.
     lat: The latitude axis.
     tide_type: The type of tide.
     longitude_major: If true, the longitude axis is the major axis.
)__doc__")
      .def("lon", &tidal_model::Cartesian<T>::lon,
           R"__doc__(
Get the longitude axis.

Returns:
     The longitude axis.
)__doc__")
      .def("lat", &tidal_model::Cartesian<T>::lat,
           R"__doc__(
Get the latitude axis.

Returns:
     The latitude axis.
)__doc__");
}

}  // namespace fes

void init_cartesian_model(py::module& m) {
  fes::init_cartesian_model<double>(m, "Complex128");
  fes::init_cartesian_model<float>(m, "Complex64");
}
