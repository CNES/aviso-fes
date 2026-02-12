// Copyright (c) 2026 CNES
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

#include "fes/geometry/box.hpp"
#include "fes/geometry/ecef.hpp"
#include "fes/geometry/point.hpp"
#include "fes/geometry/triangle.hpp"

namespace fes {
namespace mesh {

/// @brief Vertex attribute in a triangle.
struct VertexAttribute {
  /// The vertex ID (0, 1, or 2) in the triangle.
  uint8_t vertex_id;
  /// The triangle index.
  int32_t triangle_index;
};

/// @brief Result of a triangle query.
///
/// This structure contains the result of a triangle query. It includes the
/// triangle index, the query point, the selected triangle, and the nearest
/// vertices from triangles closest to the query point when the point is
/// outside the mesh.
struct TriangleQueryResult {
  /// The triangle index.
  std::int32_t index{-1};
  /// The query point used for the search.
  geometry::Point point{};
  /// The selected triangle.
  geometry::Triangle triangle{};
  /// List of nearest vertices from triangles closest to the query point.
  std::vector<VertexAttribute> nearest_vertices{};

  /// Default constructor.
  constexpr TriangleQueryResult() = default;

  /// @brief Constructs a TriangleQueryResult when the query point is inside the
  /// mesh.
  ///
  /// @param[in] triangle_index The triangle index.
  /// @param[in] point The point to be used to calculate the interpolation
  /// weights.
  /// @param[in] triangle The selected triangle.
  inline TriangleQueryResult(const std::int32_t triangle_index,
                             const geometry::Point& point,
                             geometry::Triangle triangle)
      : index(triangle_index), point(point), triangle(std::move(triangle)) {}

  /// @brief Constructs a TriangleQueryResult when the query point is outside
  /// the mesh.
  /// @param nearest_vertices The nearest vertices from triangles closest to the
  /// query point.
  /// @param point The point to be used to calculate the interpolation weights.
  inline TriangleQueryResult(std::vector<VertexAttribute> nearest_vertices,
                             const geometry::Point& point)
      : point(point), nearest_vertices(std::move(nearest_vertices)) {}

  /// @brief Check if the requested point is inside the mesh.
  /// @return True if the point is inside the mesh, false otherwise.
  constexpr auto is_inside() const noexcept -> bool { return index >= 0; }

  /// @brief Check if the query is valid.
  /// @return True if the query is valid, false otherwise.
  inline auto is_valid() const noexcept {
    return is_inside() || !nearest_vertices.empty();
  }
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
  auto search(const geometry::Point& point, const double max_distance) const
      -> TriangleQueryResult;

  /// Get the number of positions in the index
  inline constexpr auto n_positions() const noexcept -> size_t {
    return lon_.size();
  }

  /// Get the number of triangles in the index
  inline constexpr auto n_triangles() const noexcept -> size_t {
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

  /// @brief Get the indices of the triangles that intersect the bounding box.
  ///
  /// @param[in] bbox The bounding box.
  /// @return The indices of the triangles that intersect the bounding box.
  auto selected_triangles(const geometry::Box& bbox) const
      -> std::vector<int64_t>;

  /// @brief Return the memory usage of the mesh index in bytes.
  inline auto memory_usage() const -> size_t;

 private:
  /// Values stored in the R*Tree : Vertex of the triangle in ECEF coordinates,
  /// index of vertex (0, 1 or 2) and index of triangle.
  using ValueType =
      std::pair<geometry::EarthCenteredEarthFixed, std::pair<int8_t, int32_t>>;

  /// R*Tree type
  using RTreeType =
      boost::geometry::index::rtree<ValueType,
                                    boost::geometry::index::rstar<16>>;

  /// The latitude coordinates of the mesh vertices.
  Eigen::VectorXd lon_;

  /// The longitude coordinates of the mesh vertices.
  Eigen::VectorXd lat_;

  /// The indices of the mesh vertices that form each triangle.
  Eigen::Matrix<int32_t, -1, 3> triangles_;

  /// The R*Tree
  RTreeType rtree_{};

  /// Search the nearest triangles to a point in ECEF coordinates.
  inline auto nearest(const geometry::EarthCenteredEarthFixed& cartesian_point,
                      const size_t max_neighbors) const
      -> std::pair<std::set<int32_t>, double>;

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

  /// Filter the vertices of a triangle that are within a maximum distance from
  /// a given point.
  inline auto filter_nearby_vertices(
      const geometry::EarthCenteredEarthFixed& point, const int triangle_index,
      const double max_distance,
      std::vector<VertexAttribute>& nearest_vertices) const -> void;
};

auto Index::memory_usage() const -> size_t {
  /// R*-tree maximum number of elements per node (rstar<16>)
  constexpr size_t kMaxElementsPerNode = 16;

  /// Size of pointer
  constexpr size_t kPointerSize = sizeof(void*);

  /// Total ValueType size
  constexpr size_t kValueSize = sizeof(ValueType);

  /// Size of a 3D bounding box
  constexpr size_t kBoxSize = sizeof(geometry::Box);

  /// Small structural overhead per node
  /// (parent pointer, level, bookkeeping, alignment)
  /// Approximation based on Boost implementation.
  constexpr size_t kNodeHeaderSize = 64;

  /// Leaf node payload:
  /// each entry stores (Box + Value)
  /// 48 + 32 = 80 bytes per entry
  constexpr size_t kLeafEntrySize = kBoxSize + kValueSize;

  /// Maximum leaf node size
  constexpr size_t kLeafNodeSize =
      kNodeHeaderSize + kMaxElementsPerNode * kLeafEntrySize;

  /// Internal node payload:
  /// each entry stores (Box + child pointer)
  /// 48 + 8 = 56 bytes per entry
  constexpr size_t kInternalEntrySize = kBoxSize + kPointerSize;

  /// Maximum internal node size
  constexpr size_t kInternalNodeSize =
      kNodeHeaderSize + kMaxElementsPerNode * kInternalEntrySize;

  // Rough estimation for rstar<16>
  auto number_of_elements = rtree_.size();
  size_t leaf_nodes = 0;
  size_t internal_nodes = 0;
  if (number_of_elements > 0) {
    leaf_nodes =
        (number_of_elements + kMaxElementsPerNode - 1) / kMaxElementsPerNode;
    if (leaf_nodes > 1) {
      internal_nodes =
          (leaf_nodes + (kMaxElementsPerNode - 2)) / (kMaxElementsPerNode - 1);
    }
  }
  return number_of_elements * kValueSize + leaf_nodes * kLeafNodeSize +
         internal_nodes * kInternalNodeSize;
}

auto Index::nearest(const geometry::EarthCenteredEarthFixed& cartesian_point,
                    const size_t max_neighbors) const
    -> std::pair<std::set<int32_t>, double> {
  auto triangle_indices = std::set<int>();
  auto min_distance = std::numeric_limits<double>::max();
  std::for_each(rtree_.qbegin(boost::geometry::index::nearest(cartesian_point,
                                                              max_neighbors)),
                rtree_.qend(),
                [&cartesian_point, &min_distance,
                 &triangle_indices](const auto& item) -> void {
                  triangle_indices.emplace(item.second.second);
                  min_distance = std::min(
                      min_distance,
                      boost::geometry::distance(cartesian_point, item.first));
                });
  return std::make_pair(std::move(triangle_indices), min_distance);
}

auto Index::filter_nearby_vertices(
    const geometry::EarthCenteredEarthFixed& point, const int triangle_index,
    const double max_distance,
    std::vector<VertexAttribute>& nearest_vertices) const -> void {
  const Eigen::Vector3i& vertex_indices = triangles_.row(triangle_index);
  for (uint8_t vertex_id = 0; vertex_id < 3; ++vertex_id) {
    const auto vertex_index = vertex_indices(vertex_id);
    const auto vertex = geometry::EarthCenteredEarthFixed(
        geometry::Point(lon_(vertex_index), lat_(vertex_index)));
    const auto distance = detail::geometry::distance(point, vertex);
    if (distance <= max_distance) {
      nearest_vertices.push_back({vertex_id, triangle_index});
    }
  }
}

}  // namespace mesh
}  // namespace fes
