// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/geometry/algorithm.hpp
/// @brief Geometry algorithms
#pragma once
#include <boost/geometry.hpp>

namespace fes {
namespace detail {
namespace geometry {

/// Calculate the distance between two geometries.
///
/// @tparam Geometry1 The type of the first geometry.
/// @tparam Geometry2 The type of the second geometry.
/// @param[in] geometry1 The first geometry.
/// @param[in] geometry2 The second geometry.
/// @return The distance between the two geometries.
template <typename Geometry1, typename Geometry2>
inline auto distance(const Geometry1& geometry1,
                     const Geometry2& geometry2) -> double {
  return boost::geometry::distance(geometry1, geometry2);
}

/// Calculate the area of a geometry.
///
/// @tparam Geometry The type of the geometry.
/// @param[in] geometry The geometry.
/// @return The area of the geometry.
template <typename Geometry>
inline auto area(const Geometry& geometry) -> double {
  return boost::geometry::area(geometry);
}

/// Compute the closest points between two geometries.
/// @tparam Geometry1 The type of the first geometry.
/// @tparam Geometry2 The type of the second geometry.
/// @tparam Segment The type of the segment.
/// @param[in] geometry1 The first geometry.
/// @param[in] geometry2 The second geometry.
/// @param[in] segment The segment.
template <typename Geometry1, typename Geometry2, typename Segment>
inline void closest_points(const Geometry1& geometry1,
                           const Geometry2& geometry2, Segment& segment) {
  boost::geometry::closest_points(geometry1, geometry2, segment);
}

}  // namespace geometry
}  // namespace detail
}  // namespace fes
