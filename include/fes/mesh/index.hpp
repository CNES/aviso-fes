// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/mesh/index.hpp
/// @brief Mesh indexer
#pragma once
#include <Eigen/Core>
#include <boost/geometry.hpp>
#include <cstdint>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "fes/geometry/ecef.hpp"
#include "fes/geometry/point.hpp"
#include "fes/geometry/triangle.hpp"
#include "fes/string_view.hpp"

namespace fes {
namespace mesh {

/// @brief Properties of a triangle selected in a mesh.
///
/// Hold the information about a triangle found. This is used to construct the
/// result of a query to the index when the provided point is inside the mesh.
struct SelectedTriangle {
  /// Flag to indicate if the point is inside the mesh.
  bool inside{};
  /// The triangle index.
  std::int32_t index{-1};
  /// The point to be used to calculate the interpolation weights. If the query
  /// point is inside the triangle, this is the query point. Otherwise, it is
  /// the projection of the query point on the nearest edge.
  geometry::Point point{};
  /// The selected triangle.
  geometry::Triangle triangle{};

  /// Default constructor.
  constexpr SelectedTriangle() = default;

  /// Construct a new selected triangle.
  ///
  /// @param[in] inside Flag to indicate if the point is inside the mesh.
  /// @param[in] triangle_index The triangle index.
  /// @param[in] point The point to be used to calculate the interpolation
  /// weights.
  /// @param[in] triangle The selected triangle.
  inline SelectedTriangle(const bool inside, const std::int32_t triangle_index,
                          geometry::Point point, geometry::Triangle triangle)
      : inside(inside),
        index(triangle_index),
        point(point),
        triangle(std::move(triangle)) {}
};

/// %Index the triangles of a mesh.
class Index : public std::enable_shared_from_this<Index> {
 public:
  /// Default constructor.
  ///
  /// @param[in] lon The longitude coordinates of the mesh vertices.
  /// @param[in] lat The latitude coordinates of the mesh vertices.
  /// @param[in] triangles The mesh triangles.
  Index(Eigen::VectorXd lon, Eigen::VectorXd lat,
        Eigen::Matrix<int32_t, -1, 3> triangles);

  /// Search the triangle that contains a point. If such a triangle is not
  /// found, it returns the nearest triangle if it's within a given
  /// distance.
  ///
  /// @param[in] point The point.
  /// @param[in] max_distance The maximum distance to the nearest triangle.
  /// @return The selected triangle.
  auto search(const geometry::Point& point,
              const double max_distance) const -> SelectedTriangle;

  /// Get the number of positions in the index
  inline auto n_positions() const noexcept -> size_t { return lon_.size(); }

  /// Get the number of triangles in the index
  inline auto n_triangles() const noexcept -> size_t {
    return triangles_.rows();
  }

  /// Get the longitude coordinates of the mesh vertices.
  constexpr auto lon() const noexcept -> Eigen::VectorXd const& { return lon_; }

  /// Get the latitude coordinates of the mesh vertices.
  constexpr auto lat() const noexcept -> Eigen::VectorXd const& { return lat_; }

  /// Get the mesh triangles.
  constexpr auto triangles() const noexcept
      -> Eigen::Matrix<int32_t, -1, 3> const& {
    return triangles_;
  }

  /// @brief Get a string representation of the index state.
  ///
  /// @return The string representation of the index state.
  auto getstate() const -> std::string;

  /// @brief Build an index from serialized state.
  ///
  /// @param[in] data The serialized state.
  /// @return The index.
  static auto setstate(const string_view& data) -> Index;

 private:
  /// Values stored in the R*Tree : Vertex of the triangle in ECEF coordinates,
  /// index of vertex (0, 1 or 2) and index of triangle.
  using value_t =
      std::pair<geometry::EarthCenteredEarthFixed, std::pair<int8_t, int32_t>>;

  /// R*Tree type
  using rtree_t =
      boost::geometry::index::rtree<value_t, boost::geometry::index::rstar<16>>;

  /// The latitude coordinates of the mesh vertices.
  Eigen::VectorXd lon_;

  /// The longitude coordinates of the mesh vertices.
  Eigen::VectorXd lat_;

  /// The indices of the mesh vertices that form each triangle.
  Eigen::Matrix<int32_t, -1, 3> triangles_;

  /// The R*Tree
  rtree_t rtree_{};

  /// Build the selected triangle.
  inline auto build_triangle(const int triangle_index) const
      -> geometry::Triangle {
    const Eigen::Vector3i& vertex_indices = triangles_.row(triangle_index);
    const auto i0 = vertex_indices(0);
    const auto i1 = vertex_indices(1);
    const auto i2 = vertex_indices(2);

    return {geometry::Point(lon_(i0), lat_(i0)),
            geometry::Point(lon_(i1), lat_(i1)),
            geometry::Point(lon_(i2), lat_(i2))};
  }
};

}  // namespace mesh
}  // namespace fes
