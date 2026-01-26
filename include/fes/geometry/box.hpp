// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/geometry/box.hpp
/// @brief Definition of a geographic box
#pragma once

#include <boost/geometry.hpp>

#include "fes/detail/math.hpp"
#include "fes/geometry/point.hpp"

namespace fes {
namespace geometry {

/// Box type definition.
using box_t = boost::geometry::model::box<point_t>;

/// @brief Geographic box.
class Box {
 public:
  /// Default constructor.
  Box() = default;

  /// @brief Build a Box from two points.
  ///
  /// @param[in] min_corner The minimum corner of the box.
  /// @param[in] max_corner The maximum corner of the box.
  Box(const Point& min_corner, const Point& max_corner) {
    boost::geometry::assign_values(
        box_, min_corner.lon(), min_corner.lat(),
        max_corner.lon() < min_corner.lon()
            ? detail::math::normalize_angle(max_corner.lon(), min_corner.lon(),
                                            360.0)
            : max_corner.lon(),
        max_corner.lat());
  }

  /// @brief Get the minimum corner of the box.
  ///
  /// @return The minimum corner of the box.
  constexpr auto min_corner() const -> Point {
    return Point(boost::geometry::get<boost::geometry::min_corner, 0>(box_),
                 boost::geometry::get<boost::geometry::min_corner, 1>(box_));
  }

  /// @brief Get the maximum corner of the box.
  ///
  /// @return The maximum corner of the box.
  constexpr auto max_corner() const -> Point {
    return Point(boost::geometry::get<boost::geometry::max_corner, 0>(box_),
                 boost::geometry::get<boost::geometry::max_corner, 1>(box_));
  }

  /// Write the geographic box to a stream.
  friend auto operator<<(std::ostream& os, const Box& box) -> std::ostream&;

  /// Get the WKT representation of the box.
  auto wkt() const -> boost::geometry::wkt_manipulator<box_t> {
    return boost::geometry::wkt(box_);
  }

  /// Convert the box to a string representation.
  explicit inline operator std::string() const {
    auto ss = std::stringstream{};
    ss << *this;
    return ss.str();
  }

  /// @brief Check if a geometry intersects the box.
  ///
  /// @param[in] geometry The geometry to test.
  /// @return True if the geometry intersects the box, false otherwise.
  template <typename Geometry>
  inline auto intersects(const Geometry& geometry) const -> bool {
    return boost::geometry::intersects(box_, geometry);
  }

 private:
  box_t box_{};
};

inline auto operator<<(std::ostream& os, const Box& box) -> std::ostream& {
  os << box.wkt();
  return os;
}

}  // namespace geometry
}  // namespace fes
