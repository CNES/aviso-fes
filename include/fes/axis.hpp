// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/axis.hpp
/// @brief Axis
#pragma once
#include <Eigen/Core>
#include <boost/optional.hpp>
#include <memory>
#include <sstream>
#include <tuple>

#include "fes/detail/math.hpp"

namespace fes {

/// @brief A coordinate axis a variable that specifies one of the coordinates
/// of a variable's values.
class Axis {
 public:
  /// Default constructor.
  Axis() = default;

  /// Build an axis from a vector of points.
  ///
  /// @param[in] points The axis points.
  /// @param[in] epsilon The tolerance used to determine if the axis is
  /// a longitude axis.
  /// @param[in] is_longitude True if the axis represents longitude in degrees.
  explicit Axis(const Eigen::Ref<const Eigen::VectorXd>& points,
                const double epsilon = 1e-6, const bool is_longitude = false);

  /// Build an axis from start, end and step.
  ///
  /// @param[in] start The first value of the axis.
  /// @param[in] end The last value of the axis.
  /// @param[in] step The step of the axis.
  /// @param[in] is_longitude True if the axis represents longitude in degrees.
  Axis(const double start, const double end, const double step,
       const bool is_longitude = false)
      : Axis(Eigen::VectorXd::LinSpaced(
                 static_cast<int64_t>(std::round((end - start) / step)) + 1,
                 start, end),
             1e-6, is_longitude) {}

  /// Compare two axes for equality.
  ///
  /// @param[in] other The other axis.
  /// @return True if the axes are equal.
  constexpr auto operator==(const Axis& other) const -> bool {
    return is_longitude_ == other.is_longitude_ && period_ == other.period_ &&
           is_ascending_ == other.is_ascending_ && size_ == other.size_ &&
           start_ == other.start_ && step_ == other.step_;
  }

  /// Return the size of the axis.
  constexpr auto size() const -> int64_t { return size_; }

  /// Return the first value of the axis.
  constexpr auto start() const -> double { return start_; }

  /// Return the last value of the axis.
  constexpr auto end() const -> double { return (*this)(size() - 1); }

  /// Return the step of the axis.
  constexpr auto step() const -> double { return step_; }

  /// Return the minimum value of the axis.
  constexpr auto min_value() const -> double {
    return is_ascending_ ? start() : end();
  }

  /// Return the maximum value of the axis.
  constexpr auto max_value() const -> double {
    return is_ascending_ ? end() : start();
  }

  /// True if the axis is ascending.
  constexpr auto is_ascending() const -> bool { return is_ascending_; }

  /// True if the axis represents longitude in degrees.
  constexpr auto is_longitude() const -> bool { return is_longitude_; }

  /// Return true if tis instance represents a periodic longitude axis.
  constexpr auto is_periodic() const -> double {
    return period_ == detail::math::circle_degrees<double>();
  }

  /// Return the value at the given index.
  ///
  /// @param[in] index The index.
  /// @return The value at the given index.
  constexpr auto operator()(const int64_t index) const -> double {
    if (index < 0 || index >= size_) {
      throw std::out_of_range("The index is out of range.");
    }
    return static_cast<double>(start_ + index * step_);
  }

  /// Search the index on the axis that is closest to the given value.
  ///
  /// @param[in] coordinate The coordinate to search.
  /// @param[in] bounded True if the search should be bounded by the
  /// axis limits. If false, return 0 if the value is smaller than the
  /// axis start and size() - 1 if the value is larger than the axis
  /// end.
  /// @return The index of the closest value. Return -1 if the value is
  /// out of bounds and bounded is false.
  FES_MATH_CONSTEXPR auto find_index(const double coordinate,
                                     const bool bounded = false) const noexcept
      -> int64_t {
    auto index = static_cast<int64_t>(
        std::round((normalize_coordinate(coordinate) - start_) / step_));
    if (index < 0) {
      return bounded ? 0 : -1;
    }
    if (index >= this->size_) {
      return bounded ? this->size_ - 1 : -1;
    }
    return index;
  }

  /// @brief Given a coordinate position, find grids elements around it.
  /// This mean that
  /// @code
  /// (*this)(i0) <= coordinate < (*this)(i1)
  /// @endcode
  ///
  /// @param[in] coordinate position in this coordinate system
  /// @return None if coordinate is outside the axis definition domain otherwise
  /// the tuple (i0, i1)
  auto find_indices(double coordinate) const
      -> boost::optional<std::tuple<int64_t, int64_t>>;

  /// @brief Get a string representation of this axis.
  ///
  /// @return String representation of this axis
  explicit operator std::string() const {
    std::stringstream ss;
    ss << "Axis(";
    if (is_longitude()) {
      ss << "longitude, period=" << period_ << ", ";
    }
    ss << "range=[" << min_value() << ", " << max_value() << "], ";
    ss << "step=" << step() << ", ";
    ss << "size=" << size() << ")";
    return ss.str();
  }

 private:
  /// True if the axis represents longitude in degrees.
  bool is_longitude_{};
  /// Period value for longitude axes (360°), or 0 for non-periodic axes.
  double period_{};
  /// True if the axis is ascending.
  bool is_ascending_{};
  /// The size of the axis.
  int64_t size_{};
  /// The first value of the axis.
  double start_{};
  /// The step between two values of the axis.
  double step_{};

  /// Determines whether the values contained in the vector are evenly spaced
  /// from each other.
  ///
  /// @param[in] points Values to be tested
  /// @return The increment between two values if the values are evenly spaced
  static auto is_evenly_spaced(const Eigen::Ref<const Eigen::VectorXd>& points)
      -> boost::optional<double>;

  /// Put longitude into the range [0, period_] degrees.
  static auto normalize_longitude(const Eigen::VectorXd& points)
      -> std::unique_ptr<Eigen::VectorXd>;

  /// Initializes the axis container from values.
  auto initialize(const Eigen::Ref<const Eigen::VectorXd>& values,
                  const double epsilon = 1e-6) -> void;

  /// Returns the normalized value of the coordinate with the respect to the
  /// axis definition.
  ///
  /// @param[in] coordinate The coordinate to be normalized.
  /// @return The normalized value of the coordinate.
  constexpr auto normalize_coordinate(const double coordinate) const noexcept
      -> double {
    if (is_periodic() &&
        (coordinate >= min_value() + detail::math::circle_degrees<double>() ||
         coordinate < min_value())) {
      return detail::math::normalize_angle(
          coordinate, min_value(), detail::math::circle_degrees<double>());
    }
    return coordinate;
  }
};

}  // namespace fes
