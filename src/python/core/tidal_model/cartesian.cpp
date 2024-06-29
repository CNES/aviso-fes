// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/cartesian.hpp"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

template <typename T>
void init_cartesian_model(py::module& m, const std::string& suffix) {
  py::class_<fes::tidal_model::Cartesian<T>, fes::AbstractTidalModel<T>,
             std::shared_ptr<fes::tidal_model::Cartesian<T>>>(
      m, ("Cartesian" + suffix).c_str(),
      "A tidal model that uses a Cartesian grid to store the wave models.")
      .def(py::init<fes::Axis, fes::Axis, fes::TideType, bool>(),
           py::arg("lon"), py::arg("lat"),
           py::arg("tide_type") = fes::TideType::kTide,
           py::arg("longitude_major") = true,
           R"__doc__(
Construct a Cartesian tidal model.

Args:
     lon: The longitude axis.
     lat: The latitude axis.
     tide_type: The type of tide.
     longitude_major: If true, the longitude axis is the major axis.
)__doc__")
      .def("lon", &fes::tidal_model::Cartesian<T>::lon, R"__doc__(
Get the longitude axis.

Returns:
     The longitude axis.
)__doc__")
      .def("lat", &fes::tidal_model::Cartesian<T>::lat, R"__doc__(
Get the latitude axis.

Returns:
     The latitude axis.
)__doc__")
      .def(py::pickle(
          [](const fes::tidal_model::Cartesian<T>& self) {
            return py::bytes(self.getstate());
          },
          [](const py::bytes& state) {
            char* buffer = nullptr;
            py::ssize_t length = 0;
            if (PyBytes_AsStringAndSize(state.ptr(), &buffer, &length) != 0) {
              throw py::error_already_set();
            }
            return fes::tidal_model::Cartesian<T>::setstate(
                fes::string_view(buffer, length));
          }));
}

void init_cartesian_model(py::module& m) {
  init_cartesian_model<double>(m, "Complex128");
  init_cartesian_model<float>(m, "Complex64");
}
