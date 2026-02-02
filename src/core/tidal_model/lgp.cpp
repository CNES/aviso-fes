// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/lgp.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/optional.hpp"  // IWYU pragma: keep

namespace py = pybind11;

namespace fes {

template <typename T>
void init_lgp1_model(py::module& m, const std::string& suffix) {
  py::class_<tidal_model::LGP1<T>, AbstractTidalModel<T>,
             std::shared_ptr<tidal_model::LGP1<T>>>(m,
                                                    ("LGP1" + suffix).c_str(),
                                                    R"__doc__(
Handle the wave models loaded from finite elements using LGP1
discretization.
)__doc__")
      .def(py::init<
               std::shared_ptr<mesh::Index>,
               typename tidal_model::LGP1<T>::codes_t, ConstituentMap, TideType,
               double,
               boost::optional<std::tuple<double, double, double, double>>>(),
           py::arg("index"), py::arg("codes"), py::arg("constituent_map"),
           py::arg("tide_type") = kTide, py::arg("max_distance") = 0,
           py::arg("bbox") = boost::none,
           R"__doc__(
Construct a LGP1 tidal model.

Args:
    index: The index of the finite elements.
    codes: The LGP1 codes.
    constituent_map: The constituent map between IDs and names.
    tide_type: The type of tide handled by the model.
    max_distance: The maximum distance allowed to extrapolate the wave
        models. By default, extrapolation is disabled, all points outside
        the finite elements will be considered undefined.
    bbox: The bounding box to consider when selecting the LGP codes. It is
        represented by a tuple of four values: the minimum longitude, the
        minimum latitude, the maximum longitude, and the maximum latitude. If
        the bounding box is not provided, all LGP codes will be considered.
)__doc__")
      .def("index", &tidal_model::LGP1<T>::index, R"__doc__(
Get the index of the finite elements.

Returns:
    The index of the finite elements.
)__doc__")
      .def("selected_indices", &tidal_model::LGP1<T>::selected_indices,
           R"__doc__(Retrieve the indices for wave model values that intersect
the specified bounding box.

Returns:
  A vector containing the selected indices. If no bounding box is set, an empty
  vector is returned.
)__doc__");
}

template <typename T>
void init_lgp2_model(py::module& m, const std::string& suffix) {
  py::class_<tidal_model::LGP2<T>, AbstractTidalModel<T>,
             std::shared_ptr<tidal_model::LGP2<T>>>(m,
                                                    ("LGP2" + suffix).c_str(),
                                                    R"__doc__(
Handle the wave models loaded from finite elements using LGP2
discretization.
)__doc__")
      .def(py::init<
               std::shared_ptr<mesh::Index>,
               typename tidal_model::LGP2<T>::codes_t, ConstituentMap, TideType,
               double,
               boost::optional<std::tuple<double, double, double, double>>>(),
           py::arg("index"), py::arg("codes"), py::arg("constituent_map"),
           py::arg("tide_type") = kTide, py::arg("max_distance") = 0,
           py::arg("bbox") = boost::none,
           R"__doc__(
Construct a LGP2 tidal model.

Args:
    index: The index of the finite elements.
    codes: The LGP2 codes.
    constituent_map: The constituent map between IDs and names.
    tide_type: The type of tide handled by the model.
    max_distance: The maximum distance allowed to extrapolate the wave
        models. By default, extrapolation is disabled, all points outside
        the finite elements will be considered undefined.
    bbox: The bounding box to consider when selecting the LGP codes. It is
        represented by a tuple of four values: the minimum longitude, the
        minimum latitude, the maximum longitude, and the maximum latitude. If
        the bounding box is not provided, all LGP codes will be considered.
)__doc__")
      .def("index", &tidal_model::LGP2<T>::index, R"__doc__(
Get the index of the finite elements.

Returns:
    The index of the finite elements.
)__doc__")
      .def("selected_indices", &tidal_model::LGP2<T>::selected_indices,
           R"__doc__(Retrieve the indices for wave model values that intersect
the specified bounding box.

Returns:
  A vector containing the selected indices. If no bounding box is set, an empty
  vector is returned.
)__doc__");
}

}  // namespace fes

void init_lgp_model(py::module& m) {
  fes::init_lgp1_model<double>(m, "Complex128");
  fes::init_lgp1_model<float>(m, "Complex64");
  fes::init_lgp2_model<double>(m, "Complex128");
  fes::init_lgp2_model<float>(m, "Complex64");
}
