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

/// Base class for the Point point.
using box_t = boost::geometry::model::box<point_t>;

/// @brief Geographic box.
class Box : public box_t {
 public:
  /// Default constructor.
  Box() = default;

  /// @brief Build a Box from two points.
  ///
  /// @param[in] min_corner The minimum corner of the box.
  /// @param[in] max_corner The maximum corner of the box.
  constexpr Box(const Point& min_corner, const Point& max_corner)
      : box_t{
            {detail::math::normalize_angle(min_corner.lon()), min_corner.lat()},
            {detail::math::normalize_angle(max_corner.lon()),
             max_corner.lat()}} {}

  /// @brief Get the minimum corner of the box.
  ///
  /// @return The minimum corner of the box.
  inline auto min_corner() const -> Point {
    auto* base = reinterpret_cast<const box_t*>(this);
    return Point{base->min_corner().get<0>(), base->min_corner().get<1>()};
  }

  /// @brief Get the maximum corner of the box.
  ///
  /// @return The maximum corner of the box.
  inline auto max_corner() const -> Point {
    auto* base = reinterpret_cast<const box_t*>(this);
    return Point{base->max_corner().get<0>(), base->max_corner().get<1>()};
  }

  /// Write the geographic box to a stream.
  friend auto operator<<(std::ostream& os, const Box& box) -> std::ostream&;

  /// Convert the box to a string representation.
  explicit inline operator std::string() const {
    auto ss = std::stringstream{};
    ss << *this;
    return ss.str();
  }

  /// @brief Check if a triangle intersects the box.
  ///
  /// @param[in] triangle The triangle.
  /// @return True if the triangle intersects the box, false otherwise.
  inline auto intersects(const Triangle& triangle) const -> bool {
    return boost::geometry::intersects(triangle, *this);
  }
};

}  // namespace geometry
}  // namespace fes

namespace boost {
namespace geometry {
namespace traits {

namespace fg = fes::geometry;

/// @brief Tag of the Box box.
template <>
struct tag<fg::Box> {
  /// @brief Box tag.
  using type = box_tag;
};

/// @brief Point type of the Box box.
template <>
struct point_type<fg::Box> {
  /// @brief Point type of the Box box.
  using type = fg::Point;
};

/// @brief Get the minimum corner of the Box box.
template <std::size_t Dimension>
struct indexed_access<fg::Box, min_corner, Dimension> {
  /// @brief Get the coordinate at the specified index.
  ///
  /// @param[in] box The box.
  /// @return The coordinate at the specified index.
  static auto get(const fg::Box& box) -> double {
    return box.min_corner().get<Dimension>();
  }

  /// @brief Set the coordinate at the specified index.
  ///
  /// @param[in,out] box The box.
  /// @param[in] value The value.
  static void set(fg::Box& box, double value) {
    geometry::set<Dimension>(box.min_corner(), value);
  }
};

/// @brief Index access for the Box box.
template <std::size_t Dimension>
struct indexed_access<fg::Box, max_corner, Dimension> {
  /// @brief Get the coordinate at the specified index.
  ///
  /// @param[in] box The box.
  /// @return The coordinate at the specified index.
  static auto get(const fg::Box& box) -> double {
    return box.max_corner().get<Dimension>();
  }

  /// @brief Set the coordinate at the specified index.
  ///
  /// @param[in,out] box The box.
  /// @param[in] value The value.
  static void set(fg::Box& box, double value) {
    geometry::set<Dimension>(box.max_corner(), value);
  }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

namespace fes {
namespace geometry {

/// @brief Write the geographic box to a stream.
///
/// @param[in,out] os The output stream.
/// @param[in] box The box.
/// @return The output stream.
inline auto operator<<(std::ostream& os, const Box& box) -> std::ostream& {
  os << boost::geometry::wkt(box);
  return os;
}

}  // namespace geometry
}  // namespace fes
