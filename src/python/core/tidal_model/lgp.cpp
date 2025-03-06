// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/lgp.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/optional.hpp"

namespace py = pybind11;

template <typename T>
void init_lgp1_model(py::module& m, const std::string& suffix) {
  py::class_<fes::tidal_model::LGP1<T>, fes::AbstractTidalModel<T>,
             std::shared_ptr<fes::tidal_model::LGP1<T>>>(
      m, ("LGP1" + suffix).c_str(),
      R"__doc__(
Handle the wave models loaded from finite elements using LGP1
discretization.
)__doc__")
      .def(py::init<
               std::shared_ptr<fes::mesh::Index>,
               typename fes::tidal_model::LGP1<T>::codes_t, fes::TideType,
               double,
               boost::optional<std::tuple<double, double, double, double>>>(),
           py::arg("index"), py::arg("codes"),
           py::arg("tide_type") = fes::TideType::kTide,
           py::arg("max_distance") = 0, py::arg("bbox") = boost::none,
           R"__doc__(
Construct a LGP1 tidal model.

Args:
    index: The index of the finite elements.
    codes: The LGP1 codes.
    tide_type: The type of tide handled by the model.
    max_distance: The maximum distance allowed to extrapolate the wave
        models. By default, extrapolation is disabled, all points outside
        the finite elements will be considered undefined.
    bbox: The bounding box to consider when selecting the LGP codes. It is
        represented by a tuple of four values: the minimum longitude, the
        minimum latitude, the maximum longitude, and the maximum latitude. If
        the bounding box is not provided, all LGP codes will be considered.
)__doc__")
      .def("index", &fes::tidal_model::LGP1<T>::index, R"__doc__(
Get the index of the finite elements.

Returns:
    The index of the finite elements.
)__doc__")
      .def("selected_indices", &fes::tidal_model::LGP1<T>::selected_indices,
           R"__doc__(Retrieve the indices for wave model values that intersect
the specified bounding box.

Returns:
  A vector containing the selected indices. If no bounding box is set, an empty
  vector is returned.
)__doc__")
      .def(py::pickle(
          [](const fes::tidal_model::LGP1<T>& self) {
            return py::bytes(self.getstate());
          },
          [](const py::bytes& state) {
            char* buffer = nullptr;
            py::ssize_t length = 0;
            if (PyBytes_AsStringAndSize(state.ptr(), &buffer, &length) != 0) {
              throw py::error_already_set();
            }
            return fes::tidal_model::LGP1<T>::setstate(
                fes::string_view(buffer, length));
          }));
}

template <typename T>
void init_lgp2_model(py::module& m, const std::string& suffix) {
  py::class_<fes::tidal_model::LGP2<T>, fes::AbstractTidalModel<T>,
             std::shared_ptr<fes::tidal_model::LGP2<T>>>(
      m, ("LGP2" + suffix).c_str(),
      R"__doc__(
Handle the wave models loaded from finite elements using LGP2
discretization.
)__doc__")
      .def(py::init<
               std::shared_ptr<fes::mesh::Index>,
               typename fes::tidal_model::LGP2<T>::codes_t, fes::TideType,
               double,
               boost::optional<std::tuple<double, double, double, double>>>(),
           py::arg("index"), py::arg("codes"),
           py::arg("tide_type") = fes::TideType::kTide,
           py::arg("max_distance") = 0, py::arg("bbox") = boost::none,
           R"__doc__(
Construct a LGP2 tidal model.

Args:
    index: The index of the finite elements.
    codes: The LGP2 codes.
    tide_type: The type of tide handled by the model.
    max_distance: The maximum distance allowed to extrapolate the wave
        models. By default, extrapolation is disabled, all points outside
        the finite elements will be considered undefined.
    bbox: The bounding box to consider when selecting the LGP codes. It is
        represented by a tuple of four values: the minimum longitude, the
        minimum latitude, the maximum longitude, and the maximum latitude. If
        the bounding box is not provided, all LGP codes will be considered.
)__doc__")
      .def("index", &fes::tidal_model::LGP2<T>::index, R"__doc__(
Get the index of the finite elements.

Returns:
    The index of the finite elements.
)__doc__")
      .def("selected_indices", &fes::tidal_model::LGP2<T>::selected_indices,
           R"__doc__(Retrieve the indices for wave model values that intersect
the specified bounding box.

Returns:
  A vector containing the selected indices. If no bounding box is set, an empty
  vector is returned.
)__doc__")
      .def(py::pickle(
          [](const fes::tidal_model::LGP2<T>& self) {
            return py::bytes(self.getstate());
          },
          [](const py::bytes& state) {
            char* buffer = nullptr;
            py::ssize_t length = 0;
            if (PyBytes_AsStringAndSize(state.ptr(), &buffer, &length) != 0) {
              throw py::error_already_set();
            }
            return fes::tidal_model::LGP2<T>::setstate(
                fes::string_view(buffer, length));
          }));
}

void init_lgp_model(py::module& m) {
  init_lgp1_model<double>(m, "Complex128");
  init_lgp1_model<float>(m, "Complex64");
  init_lgp2_model<double>(m, "Complex128");
  init_lgp2_model<float>(m, "Complex64");
}
