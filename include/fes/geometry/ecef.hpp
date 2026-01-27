// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/geometry/ecef.hpp
/// @brief Earth-centered, Earth-fixed (ECEF) point
#pragma once

#include <boost/geometry.hpp>
#include <ostream>
#include <sstream>
#include <string>

namespace fes {
namespace geometry {

/// Base class for the Earth Centered Earth Fixed (ECEF) point.
using ecef_t =
    boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian>;

/// Earth Centered Earth Fixed (ECEF) point.
class EarthCenteredEarthFixed : public ecef_t {
 public:
  /// Default constructor.
  EarthCenteredEarthFixed() = default;

  /// Constructor.
  ///
  /// @param[in] x The x coordinate.
  /// @param[in] y The y coordinate.
  /// @param[in] z The z coordinate.
  constexpr EarthCenteredEarthFixed(double x, double y, double z)
      : ecef_t(x, y, z) {}

  /// Get the x coordinate.
  constexpr auto x() const { return get<0>(); }

  /// Get the y coordinate.
  constexpr auto y() const { return get<1>(); }

  /// Get the z coordinate.
  constexpr auto z() const { return get<2>(); }

  /// Set the x coordinate.
  ///
  /// @param[in] x The x coordinate.
  inline auto x(const double x) { set<0>(x); }

  /// Set the y coordinate.
  ///
  /// @param[in] y The y coordinate.
  inline auto y(const double y) { set<1>(y); }

  /// Set the z coordinate.
  ///
  /// @param[in] z The z coordinate.
  inline auto z(const double z) { set<2>(z); }

  /// Write the ECEF point to a stream.
  friend auto operator<<(std::ostream& os, const EarthCenteredEarthFixed& ecef)
      -> std::ostream&;

  /// Convert the point to a string representation.
  explicit inline operator std::string() const {
    auto ss = std::stringstream{};
    ss << *this;
    return ss.str();
  }

  /// Test if the point is equal to another point.
  /// @param[in] other The other point.
  /// @return True if the point is equal to the other point.
  inline auto operator==(const EarthCenteredEarthFixed& other) const -> bool {
    return boost::geometry::equals(*this, other);
  }
};

}  // namespace geometry
}  // namespace fes

namespace boost {
namespace geometry {
namespace traits {

namespace fg = fes::geometry;

/// @brief Tag for the Earth Centered Earth Fixed (ECEF) point.
template <>
struct tag<fg::EarthCenteredEarthFixed> {
  /// @brief The tag is point_tag.
  using type = point_tag;
};

/// @brief Coordinate type for the Earth Centered Earth Fixed (ECEF) point.
template <>
struct coordinate_type<fg::EarthCenteredEarthFixed> {
  /// @brief The coordinate type is double.
  using type = double;
};

/// @brief Coordinate system for the Earth Centered Earth Fixed (ECEF) point.
template <>
struct coordinate_system<fg::EarthCenteredEarthFixed> {
  /// @brief The coordinate system is cartesian.
  using type = cs::cartesian;
};

/// @brief Dimension for the Earth Centered Earth Fixed (ECEF) point.
template <>
struct dimension<fg::EarthCenteredEarthFixed> : boost::mpl::int_<3> {};

/// @brief Access to the Earth Centered Earth Fixed (ECEF) point.
template <std::size_t I>
struct access<fg::EarthCenteredEarthFixed, I> {
  /// @brief Get the Ith coordinate of the Earth Centered Earth Fixed (ECEF)
  /// point.
  ///
  /// @param[in] p The Earth Centered Earth Fixed (ECEF) point.
  /// @return The Ith coordinate of the Earth Centered Earth Fixed (ECEF) point.
  static auto get(const fg::EarthCenteredEarthFixed& p) -> double {
    return p.template get<I>();
  }

  /// @brief Set the Ith coordinate of the Earth Centered Earth Fixed (ECEF)
  /// point.
  ///
  /// @param[in,out] p The Earth Centered Earth Fixed (ECEF) point.
  /// @param[in] value The new value.
  static void set(fg::EarthCenteredEarthFixed& p, double value) {
    p.template set<I>(value);
  }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost

namespace fes {
namespace geometry {

/// @brief Inserts a ECEF point into a stream.
/// @param[in,out] os The stream.
/// @param[in] ecef The ECEF point.
/// @return The stream.
inline auto operator<<(std::ostream& os, const EarthCenteredEarthFixed& ecef)
    -> std::ostream& {
  os << "ECEF(" << ecef.x() << " " << ecef.y() << " " << ecef.z() << ")";
  return os;
}

}  // namespace geometry
}  // namespace fes
