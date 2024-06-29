// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/axis.hpp
/// @brief Axis
#pragma once
#include <Eigen/Core>
#include <boost/optional.hpp>
#include <limits>
#include <memory>
#include <tuple>

#include "fes/detail/math.hpp"
#include "fes/string_view.hpp"

namespace fes {

/// @brief A coordinate axis a variable that specifies one of the coordinates
/// of a variable's values.
class Axis : public std::enable_shared_from_this<Axis> {
 public:
  /// Default constructor.
  Axis() = default;

  /// Build an axis from a vector of points.
  ///
  /// @param[in] points The axis points.
  /// @param[in] epsilon The tolerance used to determine if the axis is
  /// circular.
  /// @param[in] is_circular True if the axis is circular. For example,
  /// longitude is circular.
  explicit Axis(const Eigen::Ref<const Eigen::VectorXd>& points,
                const double epsilon = 1e-6, const bool is_circular = false)
      : is_circular_(is_circular) {
    if (points.size() > std::numeric_limits<int64_t>::max()) {
      throw std::invalid_argument(
          "the size of the axis must not contain more than " +
          std::to_string(std::numeric_limits<int64_t>::max()) + "elements.");
    }

    if (points.size() < 2) {
      throw std::invalid_argument(
          "the size of the axis must contain at least 2 elements.");
    }
    // If this is a circular axis, normalize the points.
    if (is_circular_) {
      auto normalized_values = Axis::normalize_longitude(points);
      normalized_values ? initialize(*normalized_values, epsilon)
                        : initialize(points, epsilon);
    } else {
      initialize(points);
    }
  }

  /// Compare two axes for equality.
  ///
  /// @param[in] other The other axis.
  /// @return True if the axes are equal.
  constexpr auto operator==(const Axis& other) const -> bool {
    return is_circular_ == other.is_circular_ &&
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

  /// True if the axis is circular.
  constexpr auto is_circular() const -> bool { return is_circular_; }

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

  /// @brief Serialize the axis.
  /// @return The serialized axis.
  auto getstate() const -> std::string;

  /// @brief Deserialize the axis.
  /// @param[in] data The serialized axis.
  static auto setstate(const string_view& data) -> Axis;

 private:
  /// True if the axis is circular.
  bool is_circular_{};
  /// True if the axis is ascending.
  bool is_ascending_{};
  /// The size of the axis.
  int64_t size_{};
  /// The first value of the axis.
  double start_{};
  /// The step between two values of the axis.
  double step_{};

  /// @brief Construct an axis from its properties.
  ///
  /// @param[in] is_circular True if the axis is circular.
  /// @param[in] is_ascending True if the axis is ascending.
  /// @param[in] start The first value of the axis.
  /// @param[in] size The size of the axis.
  /// @param[in] step The step between two values of the axis.
  Axis(const bool is_circular, const bool is_ascending, const double start,
       const int64_t size, const double step)
      : is_circular_(is_circular),
        is_ascending_(is_ascending),
        size_(size),
        start_(start),
        step_(step) {}

  /// Determines whether the values contained in the vector are evenly spaced
  /// from each other.
  ///
  /// @param[in] points Values to be tested
  /// @return The increment between two values if the values are evenly spaced
  static auto is_evenly_spaced(const Eigen::Ref<const Eigen::VectorXd>& points)
      -> boost::optional<double>;

  /// Put longitude into the range [0, circle_] degrees.
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
    if (is_circular_ &&
        (coordinate >= min_value() + 360.0 || coordinate < min_value())) {
      return detail::math::normalize_angle(coordinate, min_value(), 360.0);
    }
    return coordinate;
  }
};

}  // namespace fes
