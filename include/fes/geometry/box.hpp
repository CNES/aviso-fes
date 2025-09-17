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
#include "fes/geometry/triangle.hpp"

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
    auto lon1 = detail::math::normalize_angle(min_corner.lon());
    auto lon2 = detail::math::normalize_angle(max_corner.lon());
    auto lat1 = min_corner.lat();
    auto lat2 = max_corner.lat();

    crosses_dateline_ = lon1 > lon2;

    if (crosses_dateline_) {
      boost::geometry::assign_values(box1_, lon1, lat1, 180.0, lat2);
      boost::geometry::assign_values(box2_, -180.0, lat1, lon2, lat2);
    } else {
      boost::geometry::assign_values(box1_, lon1, lat1, lon2, lat2);
    }
    if (!boost::geometry::is_valid(box1_) ||
        (crosses_dateline_ && !boost::geometry::is_valid(box2_))) {
      throw std::invalid_argument("Invalid box: check the corner coordinates");
    }
  }

  /// @brief Get the minimum corner of the box.
  ///
  /// @return The minimum corner of the box.
  constexpr auto min_corner() const -> Point {
    return Point(boost::geometry::get<boost::geometry::min_corner, 0>(box1_),
                 boost::geometry::get<boost::geometry::min_corner, 1>(box1_));
  }

  /// @brief Get the maximum corner of the box.
  ///
  /// @return The maximum corner of the box.
  constexpr auto max_corner() const -> Point {
    if (crosses_dateline_) {
      return Point(boost::geometry::get<boost::geometry::max_corner, 0>(box2_),
                   boost::geometry::get<boost::geometry::max_corner, 1>(box2_));
    }
    return Point(boost::geometry::get<boost::geometry::max_corner, 0>(box1_),
                 boost::geometry::get<boost::geometry::max_corner, 1>(box1_));
  }

  /// Write the geographic box to a stream.
  friend auto operator<<(std::ostream& os, const Box& box) -> std::ostream&;

  /// Get the WKT representation of the box.
  auto wkt() const -> boost::geometry::wkt_manipulator<box_t> {
    if (crosses_dateline_) {
      auto box = box_t{};
      boost::geometry::assign_values(
          box, boost::geometry::get<0>(box1_.min_corner()),
          boost::geometry::get<1>(box1_.min_corner()),
          boost::geometry::get<0>(box2_.max_corner()),
          boost::geometry::get<1>(box2_.max_corner()));
      return boost::geometry::wkt(box);
    }
    return boost::geometry::wkt(box1_);
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
    if (crosses_dateline_) {
      return boost::geometry::intersects(box1_, geometry) ||
             boost::geometry::intersects(box2_, geometry);
    } else {
      return boost::geometry::intersects(box1_, geometry);
    }
  }

 private:
  box_t box1_{};
  box_t box2_{};
  bool crosses_dateline_{false};
};

inline auto operator<<(std::ostream& os, const Box& box) -> std::ostream& {
  os << box.wkt();
  return os;
}

}  // namespace geometry
}  // namespace fes
