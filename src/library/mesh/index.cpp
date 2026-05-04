// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/mesh/index.hpp"

#include <algorithm>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "fes/detail/math.hpp"

namespace fes {
namespace mesh {

static auto sanity_check(
    const Eigen::VectorXd& lon, const Eigen::VectorXd& lat,
    const Eigen::Matrix<int32_t, Eigen::Dynamic, 3>& triangles) -> void {
  if (lon.size() != lat.size()) {
    throw std::invalid_argument("lon and lat must have the same size");
  }
  if (triangles.rows() == 0) {
    throw std::invalid_argument("triangles must not be empty");
  }
  if (triangles.size() > std::numeric_limits<int32_t>::max()) {
    throw std::invalid_argument("triangles must be smaller than 2^31 - 1");
  }
  auto min_triangle_id = std::numeric_limits<int32_t>::max();
  auto max_triangle_id = std::numeric_limits<int32_t>::min();
  std::for_each(triangles.data(), triangles.data() + triangles.size(),
                [&min_triangle_id, &max_triangle_id](int32_t id) {
                  min_triangle_id = std::min(min_triangle_id, id);
                  max_triangle_id = std::max(max_triangle_id, id);
                });
  if (min_triangle_id < 0) {
    throw std::invalid_argument("triangle indices must be non-negative");
  }

  if (max_triangle_id >= lon.size()) {
    throw std::invalid_argument("triangle indices must be in range [0, " +
                                std::to_string(lon.size()) + ")");
  }
}

Index::Index(Eigen::VectorXd lon, Eigen::VectorXd lat,
             Eigen::Matrix<int32_t, Eigen::Dynamic, 3> triangles)
    : lon_(std::move(lon)),
      lat_(std::move(lat)),
      triangles_(std::move(triangles)) {
  // Sanity checks on the input data
  sanity_check(lon_, lat_, triangles_);

  // Normalize the longitude to [-180, 180]
  std::for_each(lon_.data(), lon_.data() + lon_.size(),
                [](double& lon) { lon = detail::math::normalize_angle(lon); });

  // Allocate the values used to build the index.
  auto values = std::vector<ValueType>{};
  values.reserve(triangles_.rows() * 3);

  for (auto ix = 0; ix < triangles_.rows(); ++ix) {
    // Get the triangle vertices
    const Eigen::Vector3i& index = triangles_.row(ix);

    // For each edge of the triangle, add the ECEF coordinates and the triangle
    // index to the values vector.
    for (uint8_t jx = 0; jx < 3; ++jx) {
      auto kx = index(jx);
      values.emplace_back(static_cast<geometry::EarthCenteredEarthFixed>(
                              geometry::Point(lon_(kx), lat_(kx))),
                          std::make_pair(jx, ix));
    }
  }
  rtree_ = RTreeType{values.begin(), values.end()};
}

auto Index::search(const geometry::Point& point,
                   const double max_distance) const -> TriangleQueryResult {
  constexpr size_t kMaxNeighbors = 11;
  constexpr size_t kExtrapolationNeighbors = 16;
  constexpr size_t kMaxExtrapolationNeighbors = 128;
  // Distance step (in metres) at which we add another batch of
  // ``kExtrapolationNeighbors`` to the R-tree query.
  constexpr double kNeighborScaleDistance = 10'000.0;

  auto min_distance = 0.0;

  // Query position in ECEF coordinates
  auto cartesian_point = static_cast<geometry::EarthCenteredEarthFixed>(point);

  // Find the nearest triangles
  auto triangle_indices = std::vector<int32_t>{};
  std::tie(triangle_indices, min_distance) =
      nearest(cartesian_point, kMaxNeighbors);

  // Check for each selected triangle if the point is inside.
  for (auto ix : triangle_indices) {
    auto triangle = build_triangle(ix);
    if (triangle.covered_by(point)) {
      return {ix, point, std::move(triangle)};
    }
  }

  if (min_distance >= max_distance) {
    // No triangle found within the max distance, return an empty result.
    return TriangleQueryResult{};
  }

  // The point is not inside any triangle, so search for the nearest triangle
  // vertices to the query point.
  //
  // Scale the number of neighbors with the minimum distance to the mesh: the
  // farther the point, the more neighbours we sample so that extrapolation
  // stays well-conditioned. The count is clamped to
  // [kExtrapolationNeighbors, kMaxExtrapolationNeighbors].
  //
  // The previous expression -- ``std::max(128, std::min(16, scale * 16))`` --
  // collapsed to a constant 128 (the std::min always returned <= 16, then the
  // std::max promoted it back to 128); the linear ramp was effectively dead
  // code.
  const auto scale = static_cast<size_t>(min_distance / kNeighborScaleDistance);
  const auto num_neighbors = std::min(
      kMaxExtrapolationNeighbors,
      std::max(kExtrapolationNeighbors, kExtrapolationNeighbors * scale));

  std::tie(triangle_indices, min_distance) =
      nearest(cartesian_point, num_neighbors);
  auto nearest_vertices = std::vector<VertexAttribute>{};
  nearest_vertices.reserve(3 * triangle_indices.size());

  for (auto ix : triangle_indices) {
    filter_nearby_vertices(cartesian_point, ix, max_distance, nearest_vertices);
  }

  if (nearest_vertices.empty()) {
    // No vertices found within the max distance, return an empty result.
    return TriangleQueryResult{};
  }
  return TriangleQueryResult{std::move(nearest_vertices), point};
}

auto Index::selected_triangles(const geometry::Box& bbox) const
    -> std::vector<int64_t> {
  auto result = std::vector<int64_t>{};
  result.reserve(triangles_.rows());

  for (int64_t ix = 0; ix < triangles_.rows(); ++ix) {
    if (bbox.intersects(build_triangle(static_cast<int>(ix)))) {
      result.push_back(ix);
    }
  }

  if (result.empty()) {
    throw std::invalid_argument("no triangle intersects the bounding box");
  }

  return result;
}

}  // namespace mesh
}  // namespace fes
