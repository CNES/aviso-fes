// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/geometry/triangle.hpp
/// @brief Triangle in Geographic Coordinate System
#pragma once
#include <boost/geometry.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>

#include "fes/detail/geometry/algorithm.hpp"
#include "fes/geometry/point.hpp"

namespace fes {
namespace geometry {

/// Base class of the geodetic triangle.
using triangle_t = boost::geometry::model::polygon<Point, false>;

/// @brief Geodetic triangle.
class Triangle : public triangle_t {
 public:
  using triangle_t::triangle_t;

  /// Construct a triangle from three points.
  ///
  /// @param[in] v1 The first vertex.
  /// @param[in] v2 The second vertex.
  /// @param[in] v3 The third vertex.
  inline Triangle(const Point &v1, const Point &v2, const Point &v3)
      : triangle_t{{{v1.lon(), v1.lat()},
                    {v2.lon(), v2.lat()},
                    {v3.lon(), v3.lat()},
                    {v1.lon(), v1.lat()}}} {}

  /// Get the first vertex.
  inline auto v1() const -> const Point & { return this->outer()[0]; }

  /// Get the second vertex.
  inline auto v2() const -> const Point & { return this->outer()[1]; }

  /// Get the third vertex.
  inline auto v3() const -> const Point & { return this->outer()[2]; }

  /// Set the first vertex.
  ///
  /// @param[in] v1 The first vertex.
  inline auto v1(const Point &v1) -> void { this->outer()[0] = v1; }

  /// Set the second vertex.
  ///
  /// @param[in] v2 The second vertex.
  inline auto v2(const Point &v2) -> void { this->outer()[1] = v2; }

  /// Set the third vertex.
  ///
  /// @param[in] v3 The third point.
  inline auto v3(const Point &v3) -> void { this->outer()[2] = v3; }

  /// Returns the vertex index of the triangle corresponding to the given point,
  /// or -1 if the point is not a vertex of the triangle.
  ///
  /// @param[in] point The point.
  inline auto is_vertex(const Point &point) const -> int {
    auto *outer = this->outer().data();
    for (int ix = 0; ix < 3; ++ix) {
      if (point == *(outer++)) {
        return ix;
      }
    }
    return -1;
  }

  /// Test if the point is inside or on the border of the triangle.
  ///
  /// @param[in] point The point.
  /// @return True if the point is inside of or on the border of the triangle,
  /// else false
  /// @note The search for the point is carried out in Cartesian space, because
  /// the calculation of the angles of the reference triangle is also carried
  /// out in this space. The search carried out in geodesic space can cause
  /// inconsistencies between these two spaces and generate erroneous values.
  inline auto covered_by(const geometry::Point &point) const -> bool {
    return boost::geometry::covered_by(point, *this);
  }

  /// Check if the triangle is valid (in the OGC sense)
  inline auto is_valid() const -> bool {
    return boost::geometry::is_valid(*this);
  }

  /// Check if the triangle is empty.
  inline auto is_empty() const -> bool {
    return boost::geometry::is_empty(*this);
  }

  /// Calculate the distance between the triangle and a point.
  ///
  /// @param[in] point The point.
  /// @return The distance between the triangle and the point in meters.
  inline auto distance(const Point &point) const -> double {
    return detail::geometry::distance(point, *this);
  }

  /// Calculate the area of the triangle.
  ///
  /// @return The area of the triangle in square meters.
  inline auto area() const -> double { return detail::geometry::area(*this); }

  /// Write the triangle to a stream.
  friend auto operator<<(std::ostream &os, const Triangle &triangle)
      -> std::ostream &;

  /// Convert the triangle to a string representation.
  explicit operator std::string() const;

  /// Test if two triangles are equal.
  ///
  /// @param[in] other The other triangle.
  /// @return True if the triangles are equal, else false.
  inline auto operator==(const Triangle &other) const -> bool {
    return boost::geometry::equals(*this, other);
  }

  /// Compute the angles \f$\xi\f$ and \f$\eta\f$ of the reference right-angled
  /// triangle in the Cartesian space.
  ///
  /// @param[in] point The point.
  /// @return A tuple containing the angles  \f$\xi\f$ and \f$\eta\f$.
  /// @warning The given point must be inside the triangle otherwise the result
  /// is undefined.
  auto reference_right_angled(const Point &point) const
      -> std::tuple<double, double>;

  /// Calculate the projection of a point on the triangle.
  ///
  /// @param[in] point The point.
  /// @return The projection of the point on the triangle.
  inline auto project(const Point &point) const -> Point {
    boost::geometry::model::segment<Point> segment;

    detail::geometry::closest_points(point, *this, segment);
    return point == segment.second ? segment.first : segment.second;
  }
};

}  // namespace geometry
}  // namespace fes

namespace boost {
namespace geometry {
namespace traits {

namespace fg = fes::geometry;

/// @brief Tag of the triangle geometry.
template <>
struct tag<fg::Triangle> {
  /// @brief Type of the tag.
  using type = polygon_tag;
};
/// @brief Type of a ring of the triangle geometry.
template <>
struct ring_const_type<fg::Triangle> {
  /// @brief Type of a ring of the triangle geometry.
  using type = const model::polygon<fg::Point, false>::ring_type &;
};
/// @brief Type of a mutable ring of the triangle geometry.
template <>
struct ring_mutable_type<fg::Triangle> {
  /// @brief Type of a mutable ring of the triangle geometry.
  using type = model::polygon<fg::Point, false>::ring_type &;
};
/// @brief Type of the interior of the triangle geometry.
template <>
struct interior_const_type<fg::Triangle> {
  /// @brief Type of the interior of the triangle geometry.
  using type = const model::polygon<fg::Point, false>::inner_container_type &;
};
/// @brief Type of a mutable interior of the triangle geometry.
template <>
struct interior_mutable_type<fg::Triangle> {
  /// @brief Type of a mutable interior of the triangle geometry.
  using type = model::polygon<fg::Point, false>::inner_container_type &;
};

/// @brief Get the ring of the triangle geometry.
template <>
struct exterior_ring<fg::Triangle> {
  /// @brief Get the ring of the triangle geometry.
  /// @param[in] p The triangle geometry.
  /// @return The ring of the triangle geometry.
  static auto get(model::polygon<fg::Point, false> &p)
      -> model::polygon<fg::Point, false>::ring_type & {
    return p.outer();
  }
  /// @brief Get the ring of the triangle geometry.
  /// @param[in] p The triangle geometry.
  /// @return The ring of the triangle geometry.
  static auto get(model::polygon<fg::Point, false> const &p)
      -> model::polygon<fg::Point, false>::ring_type const & {
    return p.outer();
  }
};

/// @brief Get the interior of the triangle geometry.
template <>
struct interior_rings<fg::Triangle> {
  /// @brief Get the interior of the triangle geometry.
  /// @param[in] p The triangle geometry.
  /// @return The interior of the triangle geometry.
  static auto get(model::polygon<fg::Point, false> &p)
      -> model::polygon<fg::Point, false>::inner_container_type & {
    return p.inners();
  }
  /// @brief Get the interior of the triangle geometry.
  /// @param[in] p The triangle geometry.
  /// @return The interior of the triangle geometry.
  static auto get(model::polygon<fg::Point, false> const &p)
      -> model::polygon<fg::Point, false>::inner_container_type const & {
    return p.inners();
  }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

namespace fes {
namespace geometry {

/// @brief Inserts a Triangle into a stream.
/// @param[in,out] os The stream.
/// @param[in] triangle The triangle.
/// @return The stream.
inline auto operator<<(std::ostream &os, const Triangle &triangle)
    -> std::ostream & {
  os << boost::geometry::wkt(triangle);
  return os;
}

inline Triangle::operator std::string() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

}  // namespace geometry
}  // namespace fes
