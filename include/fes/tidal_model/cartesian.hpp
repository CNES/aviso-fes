// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tidal_model/cartesian.hpp
/// @brief Cartesian tidal model
#pragma once
#include <limits>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "fes/abstract_tidal_model.hpp"
#include "fes/axis.hpp"
#include "fes/detail/grid.hpp"
#include "fes/detail/isviewstream.hpp"
#include "fes/detail/serialize.hpp"
#include "fes/string_view.hpp"

namespace fes {
namespace tidal_model {

/// @brief %Cartesian tidal model.
///
/// @tparam T The type of the tidal model.
template <typename T>
class Cartesian : public AbstractTidalModel<T> {
 public:
  /// Build a Cartesian tidal model from its grid properties.
  ///
  /// @param[in] lon The longitude axis.
  /// @param[in] lat The latitude axis.
  /// @param[in] tide_type The tide type handled by the model.
  /// @param[in] row_major Whether the data is stored in longitude-major order.
  Cartesian(Axis lon, Axis lat, const TideType tide_type,
            const bool row_major = true)
      : AbstractTidalModel<T>(tide_type),
        row_major_(row_major),
        lon_(std::move(lon)),
        lat_(std::move(lat)) {}

  /// Add a tidal constituent to the model.
  ///
  /// @param[in] ident The tidal constituent identifier.
  /// @param[in] wave The tidal constituent modelled.
  inline auto add_constituent(const Constituent ident,
                              Vector<std::complex<T>> wave) -> void override {
    if (wave.size() != lon_.size() * lat_.size()) {
      throw std::invalid_argument("wave size does not match expected size");
    }
    this->data_.emplace(ident, std::move(wave));
  }

  /// @brief Returns a null pointer as this tidal model does not use an
  /// accelerator to speed up the calculation.
  ///
  /// @param[in] formulae The formulae used to calculate the astronomic angle.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @return A null pointer
  constexpr auto accelerator(const angle::Formulae& formulae,
                             const double time_tolerance) const
      -> Accelerator* override {
    return new Accelerator(formulae, time_tolerance, this->data_.size());
  }

  /// Interpolate the tidal model at a given point.
  ///
  /// @param[in] point The point to interpolate at.
  /// @param[inout] quality A flag indicating if the point was extrapolated.
  /// @param[inout] acc The accelerator to use.
  /// @return The interpolated tidal model.
  auto interpolate(const geometry::Point& point, Quality& quality,
                   Accelerator* acc) const -> const ConstituentValues& override;

  /// Get the longitude axis.
  ///
  /// @return The longitude axis.
  constexpr auto lon() const noexcept -> const Axis& { return lon_; }

  /// Get the latitude axis.
  ///
  /// @return The latitude axis.
  constexpr auto lat() const noexcept -> const Axis& { return lat_; }

  /// Serialize the tidal model.
  ///
  auto getstate() const -> std::string;

  /// Deserialize the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @return The tidal model.
  static auto setstate(const string_view& data) -> Cartesian<T>;

 private:
  /// Whether the data is stored in longitude-major order.
  bool row_major_;
  /// Longitude axis.
  Axis lon_;
  /// Latitude axis.
  Axis lat_;
};

// /////////////////////////////////////////////////////////////////////////////
template <typename T>
auto Cartesian<T>::interpolate(const geometry::Point& point, Quality& quality,
                               Accelerator* acc) const
    -> const ConstituentValues& {
  // Remove all previous values interpolated.
  acc->clear();
  // Find the nearest point in the grid
  auto lon_index = lon_.find_indices(point.lon());
  auto lat_index = lat_.find_indices(point.lat());

  if (!lon_index || !lat_index) {
    for (const auto& item : this->data_) {
      acc->emplace_back(
          item.first,
          std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN()));
    }
    quality = Quality::kUndefined;
    return acc->values();
  }
  int64_t i1;
  int64_t i2;
  int64_t j1;
  int64_t j2;
  std::tie(i1, i2) = *lon_index;
  std::tie(j1, j2) = *lat_index;
  const auto x1 = lon_(i1);
  const auto x2 = lon_(i2);
  const auto y1 = lat_(j1);
  const auto y2 = lat_(j2);
  auto n = int64_t{0};

  auto wxy = detail::math::bilinear_weights(
      detail::math::normalize_angle(point.lon(), x1), point.lat(), x1, y1,
      detail::math::normalize_angle(x2, x1), y2);

  auto grid = detail::Grid<std::complex<T>>(
      nullptr, static_cast<size_t>(lon_.size()),
      static_cast<size_t>(lat_.size()), row_major_);
  for (const auto& item : this->data_) {
    grid.data(item.second.data());
    auto value = detail::math::bilinear_interpolation<std::complex<double>>(
        std::get<0>(wxy), std::get<1>(wxy), std::get<2>(wxy), std::get<3>(wxy),
        grid(i1, j1), grid(i1, j2), grid(i2, j1), grid(i2, j2), n);
    acc->emplace_back(item.first, value);
  }

  quality = (n == 4)   ? Quality::kInterpolated
            : (n == 3) ? Quality::kExtrapolated3
            : (n == 2) ? Quality::kExtrapolated2
            : (n == 1) ? Quality::kExtrapolated1
                       : Quality::kUndefined;
  return acc->values();
}

template <typename T>
auto Cartesian<T>::getstate() const -> std::string {
  auto ss = std::stringstream();
  ss.exceptions(std::stringstream::failbit);
  detail::serialize::write_data(ss, row_major_);
  detail::serialize::write_string(ss, lon_.getstate());
  detail::serialize::write_string(ss, lat_.getstate());
  detail::serialize::write_data(ss, this->tide_type_);
  detail::serialize::write_constituent_map(ss, this->data_);
  return ss.str();
}

template <typename T>
auto Cartesian<T>::setstate(const string_view& data) -> Cartesian<T> {
  detail::isviewstream ss(data);
  ss.exceptions(std::stringstream::failbit);
  try {
    auto row_major = detail::serialize::read_data<bool>(ss);
    auto lon = Axis::setstate(detail::serialize::read_string(ss));
    auto lat = Axis::setstate(detail::serialize::read_string(ss));
    auto tide_type = detail::serialize::read_data<TideType>(ss);
    auto model =
        Cartesian<T>(std::move(lon), std::move(lat), tide_type, row_major);
    model.data_ =
        detail::serialize::read_constituent_map<Constituent, std::complex<T>>(
            ss);
    return model;
  } catch (const std::exception&) {
    throw std::invalid_argument("invalid tidal model state");
  }
}

}  // namespace tidal_model
}  // namespace fes
