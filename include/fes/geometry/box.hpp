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
  inline Box(const Point& min_corner, const Point& max_corner)
      : box_t{
            {detail::math::normalize_angle(min_corner.lon()), min_corner.lat()},
            {detail::math::normalize_angle(max_corner.lon()),
             max_corner.lat()}} {
    // If the box crosses the dateline, split it into two separate boxes.
    // These boxes will be used for intersection tests to correctly handle the
    // dateline crossing.
    split_boxes_if_dateline_crossed();

    if (!boost::geometry::is_valid(*this)) {
      // If the box is not valid, it may be because it crosses the dateline.
      // In this case, we check if the split boxes are valid.
      if (split_boxes_) {
        if (!boost::geometry::is_valid(split_boxes_->first) ||
            !boost::geometry::is_valid(split_boxes_->second)) {
          throw std::invalid_argument(
              "Invalid box: one of the split boxes is not valid");
        }
      } else {
        throw std::invalid_argument(
            "Invalid box: check the corner coordinates");
      }
    }
  }

  /// @brief Copy constructor.
  ///
  /// @param[in] other The other box.
  Box(const Box& other) : box_t(static_cast<const box_t&>(other)) {
    if (other.split_boxes_) {
      split_boxes_ = std::make_unique<std::pair<box_t, box_t>>(
          other.split_boxes_->first, other.split_boxes_->second);
    }
  }

  /// @brief Assignment operator.
  ///
  /// @param[in] other The other box.
  /// @return The box.
  auto operator=(const Box& other) -> Box& {
    if (this != &other) {
      if (other.split_boxes_) {
        split_boxes_ = std::make_unique<std::pair<box_t, box_t>>(
            other.split_boxes_->first, other.split_boxes_->second);
      } else {
        split_boxes_.reset();
      }
      static_cast<box_t&>(*this) = static_cast<const box_t&>(other);
    }
    return *this;
  }

  /// @brief Move constructor.
  ///
  /// @param[in,out] other The other box.
  Box(Box&& other) noexcept : box_t(std::move(static_cast<box_t&>(other))) {
    split_boxes_ = std::move(other.split_boxes_);
  }

  /// @brief Move assignment operator.
  ///
  /// @param[in,out] other The other box.
  /// @return The box.
  auto operator=(Box&& other) noexcept -> Box& {
    if (this != &other) {
      split_boxes_ = std::move(other.split_boxes_);
      static_cast<box_t&>(*this) = std::move(static_cast<box_t&>(other));
    }
    return *this;
  }

  /// @brief Get the minimum corner of the box.
  ///
  /// @return The minimum corner of the box.
  constexpr auto min_corner() const -> Point {
    const auto& corner = this->box_t::min_corner();
    return Point{boost::geometry::get<0>(corner),
                 boost::geometry::get<1>(corner)};
  }

  /// @brief Get the maximum corner of the box.
  ///
  /// @return The maximum corner of the box.
  constexpr auto max_corner() const -> Point {
    const auto& corner = this->box_t::max_corner();
    return Point{boost::geometry::get<0>(corner),
                 boost::geometry::get<1>(corner)};
  }

  /// @brief Set the minimum corner of the box.
  /// @param point The minimum corner of the box.
  auto set_min_corner(const Point& point) -> void {
    box_t::min_corner() = {point.lon(), point.lat()};
    split_boxes_if_dateline_crossed();
  }

  /// @brief Set the maximum corner of the box.
  /// @param point The maximum corner of the box.
  auto set_max_corner(const Point& point) -> void {
    box_t::max_corner() = {point.lon(), point.lat()};
    split_boxes_if_dateline_crossed();
  }

  /// Write the geographic box to a stream.
  friend auto operator<<(std::ostream& os, const Box& box) -> std::ostream&;

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
    if (split_boxes_) {
      // The box crosses the dateline, so we check for intersection with both
      // boxes.
      return boost::geometry::intersects(geometry, split_boxes_->first) ||
             boost::geometry::intersects(geometry, split_boxes_->second);
    }
    // The box does not cross the dateline, so we check for intersection with
    // the box itself.
    return boost::geometry::intersects(geometry, *this);
  }

 private:
  // An optional pair of boxes to store the two parts of a dateline-crossing
  // box.
  std::unique_ptr<std::pair<box_t, box_t>> split_boxes_{nullptr};

  /// @brief Split the box into two boxes if it crosses the dateline.
  inline auto split_boxes_if_dateline_crossed() -> void {
    // A box crosses the dateline if its minimum longitude is greater than its
    // maximum.
    const auto& min_corner = this->box_t::min_corner();
    const auto& max_corner = this->box_t::max_corner();

    if (min_corner.get<0>() > max_corner.get<0>()) {
      // Pre-calculate the two boxes that represent the dateline-crossing
      // region.
      split_boxes_ = std::make_unique<std::pair<box_t, box_t>>(
          // Box 1: from min longitude to the dateline (180 degrees)
          box_t{{min_corner.get<0>(), min_corner.get<1>()},
                {180.0, max_corner.get<1>()}},
          // Box 2: from the dateline (-180 degrees) to the max longitude
          box_t{{-180.0, min_corner.get<1>()},
                {max_corner.get<0>(), max_corner.get<1>()}});
    } else {
      split_boxes_.reset();
    }
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
    auto corner = box.min_corner();
    geometry::set<Dimension>(corner, value);
    box.set_min_corner(corner);
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
    auto corner = box.max_corner();
    geometry::set<Dimension>(corner, value);
    box.set_max_corner(corner);
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
