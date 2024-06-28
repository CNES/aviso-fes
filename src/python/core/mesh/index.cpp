// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/mesh/index.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_mesh_index(py::module& m) {
  py::class_<fes::mesh::Index, std::shared_ptr<fes::mesh::Index>>(
      m, "Index", "Index the triangles of a mesh.")
      .def(py::init<Eigen::VectorXd, Eigen::VectorXd,
                    Eigen::Matrix<int32_t, -1, 3>>(),
           py::arg("lon"), py::arg("lat"), py::arg("triangles"),
           R"__doc__(
Construct an index of a mesh.

Args:
    lon: The longitude of the vertices.
    lat: The latitude of the vertices.
    triangles: The triangles of the mesh.
)__doc__",
           py::call_guard<py::gil_scoped_release>())
      .def("lon", &fes::mesh::Index::lon, R"__doc__(
Get the longitude of the vertices.

Returns:
    The longitude of the vertices.
)__doc__")
      .def("lat", &fes::mesh::Index::lat, R"__doc__(
Get the latitude of the vertices.

Returns:
    The latitude of the vertices.
)__doc__")
      .def("triangles", &fes::mesh::Index::triangles, R"__doc__(
Get the triangles of the mesh.

Returns:
    The triangles of the mesh.
)__doc__");
}
