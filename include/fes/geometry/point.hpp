// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/geometry/point.hpp
/// @brief Point in Geographic Coordinate System
#pragma once

#include <boost/geometry.hpp>
#include <ostream>
#include <sstream>
#include <string>

#include "fes/detail/math.hpp"
#include "fes/geometry/ecef.hpp"

namespace fes {
namespace geometry {

/// Base class for the Point point.
using point_t = boost::geometry::model::point<
    double, 2, boost::geometry::cs::geographic<boost::geometry::degree>>;

/// @brief Geographic point.
class Point : public point_t {
 public:
  /// Default constructor.
  Point() = default;

  /// Build a Point point from a latitude and longitude expressed in
  /// degrees.
  constexpr Point(double lon, double lat) : point_t(lon, lat) {}

  /// Convert the point to an ECEF point.
  FES_MATH_CONSTEXPR explicit operator geometry::EarthCenteredEarthFixed()
      const;

  /// Write the geographic point to a stream.
  friend auto operator<<(std::ostream& os, const Point& point) -> std::ostream&;

  /// Convert the point to a string representation.
  explicit inline operator std::string() const {
    auto ss = std::stringstream{};
    ss << *this;
    return ss.str();
  }

  /// Return the longitude in degrees.
  constexpr auto lon() const { return get<0>(); }

  /// Return the latitude in degrees.
  constexpr auto lat() const { return get<1>(); }

  /// Set the longitude in degrees.
  ///
  /// @param[in] lon The longitude in degrees.
  inline auto lon(const double lon) { set<0>(lon); }

  /// Set the latitude in degrees.
  ///
  /// @param[in] lat The latitude in degrees.
  inline auto lat(const double lat) { set<1>(lat); }

  /// Return true if this instance is valid.
  FES_MATH_CONSTEXPR auto is_valid() const -> bool {
    return !std::isnan(lon()) && !std::isnan(lat());
  }

  /// Test if the point is equal to another point.
  ///
  /// @param[in] other The other point.
  /// @return True if the point is equal to the other point.
  inline auto operator==(const Point& other) const -> bool {
    return boost::geometry::equals(*this, other);
  }
};

}  // namespace geometry
}  // namespace fes

namespace boost {
namespace geometry {
namespace traits {

namespace fg = fes::geometry;

/// @brief Tag of the Point point.
template <>
struct tag<fg::Point> {
  /// @brief Point tag.
  using type = point_tag;
};

/// @brief Coordinate type of the Point point.
template <>
struct coordinate_type<fg::Point> {
  /// @brief Coordinate type is double.
  using type = double;
};

/// @brief Coordinate system of the Point point.
template <>
struct coordinate_system<fg::Point> {
  /// @brief Coordinate system is geographic in degrees.
  using type = cs::geographic<degree>;
};

/// @brief Dimension of the Point point.
template <>
struct dimension<fg::Point> : boost::mpl::int_<2> {};

/// @brief Accessor for the Point point.
template <std::size_t I>
struct access<fg::Point, I> {
  /// @brief Get the coordinate at the specified index.
  ///
  /// @param[in] p The point.
  /// @return The coordinate at the specified index.
  static auto get(const fg::Point& p) -> double { return p.template get<I>(); }

  /// @brief Set the coordinate at the specified index.
  ///
  /// @param[in,out] p The point.
  /// @param[in] value The value.
  static void set(fg::Point& p, double value) { p.template set<I>(value); }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

namespace fes {
namespace geometry {

/// @brief Inserts a Geographic point into a stream.
/// @param[in,out] os The stream.
/// @param[in] point The point.
/// @return The stream.
inline auto operator<<(std::ostream& os, const Point& point) -> std::ostream& {
  os << boost::geometry::wkt(point);
  return os;
}

FES_MATH_CONSTEXPR Point::operator geometry::EarthCenteredEarthFixed() const {
  // Global variables of Earth's geometric constants (WGS84)
  // Equatorial Radius [m]
  constexpr const double A = 6378137.0;

  // Polar radius [m]
  // B = 6356752.3142;

  // Eccentricity sqrt((A * A - B * B) / (A * A))
  constexpr const double E = 0.08181919092890632683;

  auto sincos_x = detail::math::sincosd(lon());
  auto sincos_y = detail::math::sincosd(lat());
  const auto chi =
      std::sqrt(1.0 - detail::math::pow<2>(E) *
                          detail::math::pow<2>(std::get<0>(sincos_y)));
  const auto a_chi = A / chi;

  return {a_chi * std::get<1>(sincos_y) * std::get<1>(sincos_x),
          a_chi * std::get<1>(sincos_y) * std::get<0>(sincos_x),
          (a_chi * (1.0 - detail::math::pow<2>(E))) * std::get<0>(sincos_y)};
}

}  // namespace geometry
}  // namespace fes
