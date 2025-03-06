// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/mesh/index.hpp"

#include <algorithm>
#include <exception>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "fes/detail/isviewstream.hpp"
#include "fes/detail/math.hpp"
#include "fes/detail/serialize.hpp"

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
  auto values = std::vector<value_t>{};
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
  rtree_ = rtree_t{values.begin(), values.end()};
}

auto Index::search(const geometry::Point& point,
                   const double max_distance) const -> SelectedTriangle {
  constexpr size_t kMaxNeighbors = 11;
  auto min_distance = std::numeric_limits<double>::max();
  auto triangle_indices = std::set<int>();

  // Query position in ECEF coordinates
  auto cartesian_point = static_cast<geometry::EarthCenteredEarthFixed>(point);
  // Find the nearest triangles
  std::for_each(rtree_.qbegin(boost::geometry::index::nearest(cartesian_point,
                                                              kMaxNeighbors)),
                rtree_.qend(),
                [&cartesian_point, &min_distance,
                 &triangle_indices](const auto& item) -> void {
                  triangle_indices.emplace(item.second.second);
                  min_distance = std::min(
                      min_distance,
                      boost::geometry::distance(cartesian_point, item.first));
                });

  // We consider the distance accurate to 1 cm
  if (min_distance < 1e-2) {
    min_distance = 0;
  }

  // Check for each selected triangle if the point is inside.
  for (auto& ix : triangle_indices) {
    auto triangle = build_triangle(ix);
    if (triangle.covered_by(point)) {
      return {true, ix, point, std::move(triangle)};
    }
  }

  auto result = SelectedTriangle{};

  // If the nearest vertex is it not too far away, looking for the nearest
  // triangle.
  if (min_distance < max_distance) {
    min_distance = std::numeric_limits<double>::max();

    for (auto& ix : triangle_indices) {
      auto triangle = build_triangle(ix);
      auto distance = triangle.distance(point);

      if (distance < min_distance) {
        min_distance = distance;
        result.index = ix;
        result.triangle = std::move(triangle);
      }
    }
    result.point = result.triangle.project(point);
  }
  return result;
}

auto Index::selected_triangles(const geometry::Box& bbox) const
    -> std::vector<int64_t> {
  auto result = std::vector<int64_t>{};
  result.reserve(triangles_.rows());

  for (int64_t ix = 0; ix < triangles_.rows(); ++ix) {
    if (bbox.intersects(build_triangle(ix))) {
      result.push_back(ix);
    }
  }

  if (result.empty()) {
    throw std::invalid_argument("no triangle intersects the bounding box");
  }

  return result;
}

auto Index::getstate() const -> std::string {
  auto ss = std::stringstream();
  ss.exceptions(std::stringstream::failbit);
  detail::serialize::write_matrix(ss, lon_);
  detail::serialize::write_matrix(ss, lat_);
  detail::serialize::write_matrix(ss, triangles_);
  return ss.str();
}

auto Index::setstate(const string_view& data) -> Index {
  detail::isviewstream ss(data);
  ss.exceptions(std::stringstream::failbit);
  try {
    auto lon = detail::serialize::read_matrix<double, Eigen::Dynamic, 1>(ss);
    auto lat = detail::serialize::read_matrix<double, Eigen::Dynamic, 1>(ss);
    auto triangles =
        detail::serialize::read_matrix<int32_t, Eigen::Dynamic, 3>(ss);
    return Index(std::move(lon), std::move(lat), std::move(triangles));
  } catch (const std::exception&) {
    throw std::invalid_argument("invalid index state");
  }
}

}  // namespace mesh
}  // namespace fes
