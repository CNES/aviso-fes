// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/axis.hpp"

#include "fes/detail/isviewstream.hpp"
#include "fes/detail/math.hpp"
#include "fes/detail/serialize.hpp"

namespace fes {

auto Axis::is_evenly_spaced(const Eigen::Ref<const Eigen::VectorXd>& points)
    -> boost::optional<double> {
  size_t n = points.size();

  // The axis is defined by a single value.
  if (n < 2) {
    return {};
  }

  auto increment =
      (points[points.size() - 1] - points[0]) / static_cast<double>(n - 1);

  // If the first two values are constant, the values are not evenly spaced.
  if (std::abs(increment) <= 1e-6) {
    return {};
  }

  for (size_t ix = 1; ix < n; ++ix) {
    if (!detail::math::is_same(points[ix] - points[ix - 1], increment, 1e-6)) {
      return {};
    }
  }
  return increment;
}

auto Axis::normalize_longitude(const Eigen::VectorXd& points)
    -> std::unique_ptr<Eigen::VectorXd> {
  auto monotonic = true;
  auto ascending = points.size() < 2 ? true : points[0] < points[1];

  for (auto ix = Eigen::Index(1); ix < points.size(); ++ix) {
    monotonic =
        ascending ? points[ix - 1] < points[ix] : points[ix - 1] > points[ix];

    if (!monotonic) {
      break;
    }
  }

  if (!monotonic) {
    auto result = std::make_unique<Eigen::VectorXd>(points);
    auto cross = false;

    for (Eigen::Index ix = 1; ix < result->size(); ++ix) {
      if (!cross) {
        cross = ascending ? (*result)[ix - 1] > (*result)[ix]
                          : (*result)[ix - 1] < (*result)[ix];
      }

      if (cross) {
        (*result)[ix] += ascending ? 360.0 : -360.0;
      }
    }
    return result;
  }
  return nullptr;
}

auto Axis::initialize(const Eigen::Ref<const Eigen::VectorXd>& values,
                      const double epsilon) -> void {
  // Determines whether the set of data provided can be represented as an
  // interval.
  auto increment = Axis::is_evenly_spaced(values);
  if (!increment) {
    throw std::invalid_argument(
        "the axis values must be evenly spaced from each other.");
  }
  auto stop = values[values.size() - 1];

  start_ = values[0];
  size_ = values.size();
  step_ = size_ == 1 ? stop - start_ : (stop - start_) / (size_ - 1);

  is_ascending_ = size_ < 2 ? true : (*this)(0) < (*this)(1);

  if (is_circular_) {
    is_circular_ =
        detail::math::is_same(static_cast<double>(std::fabs(step_ * size_)),
                              detail::math::circle_degrees<double>(), epsilon);
  }
}

auto Axis::find_indices(double coordinate) const
    -> boost::optional<std::tuple<int64_t, int64_t>> {
  // Optional value to return.
  using optional_t = boost::optional<std::tuple<int64_t, int64_t>>;

  coordinate = normalize_coordinate(coordinate);

  auto length = size_;
  auto i0 = find_index(coordinate, false);

  /// If the value is outside the circle, then the value is between the last
  /// and first index.
  if (i0 == -1) {
    return is_circular_ ? optional_t(std::make_tuple(
                              static_cast<int64_t>(length - 1), 0LL))
                        : optional_t();
  }

  // Given the delta between the found coordinate and the given coordinate,
  // chose the other index that frames the coordinate
  auto delta = coordinate - (*this)(i0);
  auto i1 = i0;
  if (delta == 0) {
    // The requested coordinate is located on an element of the axis.
    i1 == length - 1 ? --i0 : ++i1;
  } else {
    if (delta < 0) {
      // The found point is located after the coordinate provided.
      is_ascending_ ? --i0 : ++i0;
      if (is_circular_) {
        i0 = detail::math::remainder(i0, length);
      }
    } else {
      // The found point is located before the coordinate provided.
      is_ascending_ ? ++i1 : --i1;
      if (is_circular_) {
        i1 = detail::math::remainder(i1, length);
      }
    }
  }
  if (i0 >= 0 && i0 < length && i1 >= 0 && i1 < length) {
    return std::make_tuple(i0, i1);
  }
  return boost::optional<std::tuple<int64_t, int64_t>>{};
}

auto Axis::getstate() const -> std::string {
  auto ss = std::stringstream();
  ss.exceptions(std::stringstream::failbit);
  detail::serialize::write_data(ss, is_circular_);
  detail::serialize::write_data(ss, circle_);
  detail::serialize::write_data(ss, is_ascending_);
  detail::serialize::write_data(ss, start_);
  detail::serialize::write_data(ss, size_);
  detail::serialize::write_data(ss, step_);
  return ss.str();
}

auto Axis::setstate(const string_view& data) -> Axis {
  detail::isviewstream ss(data);
  ss.exceptions(std::stringstream::failbit);
  try {
    auto result = Axis();
    result.is_circular_ = detail::serialize::read_data<bool>(ss);
    result.circle_ = detail::serialize::read_data<double>(ss);
    result.is_ascending_ = detail::serialize::read_data<bool>(ss);
    result.start_ = detail::serialize::read_data<double>(ss);
    result.size_ = detail::serialize::read_data<int64_t>(ss);
    result.step_ = detail::serialize::read_data<double>(ss);
    return result;
  } catch (const std::ios_base::failure&) {
    throw std::invalid_argument("invalid axis state");
  }
}

}  // namespace fes
