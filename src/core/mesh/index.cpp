// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/mesh/index.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace fes {
namespace mesh {

void init_index(py::module& m) {
  py::class_<Index, std::shared_ptr<Index>>(
      m, "Index",
      "Provide efficient spatial indexing of mesh triangles for rapid spatial "
      "queries.")
      .def(py::init<Eigen::VectorXd, Eigen::VectorXd,
                    Eigen::Matrix<int32_t, -1, 3>>(),
           py::arg("lon"), py::arg("lat"), py::arg("triangles"),
           R"__doc__(
Construct an index of a mesh.

Args:
    lon: Specify the longitude of the vertices.
    lat: Specify the latitude of the vertices.
    triangles: Provide the triangles of the mesh.
)__doc__",
           py::call_guard<py::gil_scoped_release>())
      .def("lon", &Index::lon, R"__doc__(
Retrieve the longitude of the vertices.

Returns:
    The longitude of the vertices.
)__doc__")
      .def("lat", &Index::lat, R"__doc__(
Retrieve the latitude of the vertices.

Returns:
    The latitude of the vertices.
)__doc__")
      .def("triangles", &Index::triangles, R"__doc__(
Retrieve the triangles of the mesh.

Returns:
    The triangles of the mesh.
)__doc__");
}

}  // namespace mesh
}  // namespace fes

auto init_mesh_index(py::module& m) -> void { fes::mesh::init_index(m); }
