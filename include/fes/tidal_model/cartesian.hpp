// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tidal_model/cartesian.hpp
/// @brief Cartesian tidal model
#pragma once
#include <cmath>
#include <complex>
#include <cstdint>
#include <tuple>
#include <utility>

#include "fes/axis.hpp"
#include "fes/constituent.hpp"
#include "fes/detail/grid.hpp"
#include "fes/detail/parallel_for.hpp"
#include "fes/interface/tidal_model.hpp"

namespace fes {
namespace tidal_model {

/// @brief %Cartesian tidal model.
///
/// @tparam T The type of the tidal model.
template <typename T>
class Cartesian : public TidalModelInterface<T> {
 public:
  /// Build a Cartesian tidal model from its grid properties.
  ///
  /// @param[in] lon The longitude axis.
  /// @param[in] lat The latitude axis.
  /// @param[in] tide_type The tide type handled by the model.
  /// @param[in] row_major Whether the data is stored in longitude-major order.
  Cartesian(const Axis& lon, const Axis& lat, const TideType tide_type,
            const bool row_major = true)
      : TidalModelInterface<T>(tide_type),
        row_major_(row_major),
        lon_(lon),
        lat_(lat) {}

  /// Add a tidal constituent to the model.
  ///
  /// @param[in] ident The tidal constituent identifier.
  /// @param[in] wave The tidal constituent modelled.
  inline auto add_constituent(const ConstituentId ident,
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
                   Accelerator& acc) const -> const ConstituentValues& override;

  /// Get the longitude axis.
  ///
  /// @return The longitude axis.
  constexpr auto lon() const noexcept -> const Axis& { return lon_; }

  /// Get the latitude axis.
  ///
  /// @return The latitude axis.
  constexpr auto lat() const noexcept -> const Axis& { return lat_; }

  /// @brief Resample a tidal constituent wave to this model's grid using
  /// bilinear interpolation.
  /// @param[in] origin_lon The longitude axis of the original grid.
  /// @param[in] origin_lat The latitude axis of the original grid.
  /// @param[in] wave The wave to resample.
  /// @param[in] row_major Whether the input wave is stored in longitude-major
  /// order.
  /// @param[in] num_threads The number of threads to use for resampling. If 0,
  /// the number of threads is determined by the number of cores.
  /// @return A vector containing the resampled wave on this model's grid.
  auto resample(const Axis& origin_lon, const Axis& origin_lat,
                const Vector<std::complex<T>>& wave,
                const bool row_major = true, const size_t num_threads = 0) const
      -> Vector<std::complex<T>>;

 private:
  /// @brief Type alias for Bilinear interpolation weights.
  using Weights = std::tuple<double, double, double, double>;

  /// Helper struct to hold interpolation context
  struct InterpolationContext {
    int64_t i1;       //!< Index of the first longitude grid point.
    int64_t i2;       //!< Index of the second longitude grid point.
    int64_t j1;       //!< Index of the first latitude grid point.
    int64_t j2;       //!< Index of the second latitude grid point.
    Weights weights;  //!< Bilinear interpolation weights (w11, w12, w21, w22).

    /// @brief Check if the interpolation context is valid (i.e., the point is
    /// within the grid boundaries).
    /// @return True if the context is valid, false otherwise.
    constexpr auto is_valid() const noexcept -> bool { return i1 >= 0; }
  };

  /// Whether the data is stored in longitude-major order.
  bool row_major_;
  /// Longitude axis.
  Axis lon_;
  /// Latitude axis.
  Axis lat_;

  /// @brief Computes the interpolation context for a given point, including
  /// the indices of the surrounding grid points and the interpolation weights.
  /// @param[in] lon The longitude of the point to interpolate at.
  /// @param[in] lat The latitude of the point to interpolate at.
  /// @return An InterpolationContext struct containing the indices, weights.
  static inline auto prepare_interpolation(double lon, double lat,
                                           const Axis& lon_axis,
                                           const Axis& lat_axis)
      -> InterpolationContext;

  /// @brief Interpolates the value of a tidal constituent at a given point
  /// using the provided interpolation context and grid data.
  /// @tparam Grid The type of the grid containing the tidal constituent values.
  /// @param[in] ctx The interpolation context containing indices and weights.
  /// @param[in] grid The grid of tidal constituent values.
  /// @param[inout] n A reference to an integer that will be updated with the
  /// number of valid grid corners used in the interpolation (0, 1, 2, 3 or 4).
  /// @return The interpolated value of the tidal constituent at the given
  /// point.
  template <typename Grid>
  constexpr auto interpolate_value(const InterpolationContext& ctx,
                                   const Grid& grid, int64_t& n) const
      -> Complex;
};

// ===========================================================================
// Implementation
// ===========================================================================

template <typename T>
auto Cartesian<T>::prepare_interpolation(double lon, double lat,
                                         const Axis& lon_axis,
                                         const Axis& lat_axis)
    -> InterpolationContext {
  auto lon_index = lon_axis.find_indices(lon);
  auto lat_index = lat_axis.find_indices(lat);

  if (!lon_index || !lat_index) {
    return {-1, 0, 0, 0, {}};
  }

  int64_t i1;
  int64_t i2;
  int64_t j1;
  int64_t j2;
  std::tie(i1, i2) = *lon_index;
  std::tie(j1, j2) = *lat_index;

  auto weights = detail::math::bilinear_weights(
      detail::math::normalize_angle(lon, lon_axis(i1)), lat, lon_axis(i1),
      lat_axis(j1), detail::math::normalize_angle(lon_axis(i2), lon_axis(i1)),
      lat_axis(j2));

  return {i1, i2, j1, j2, weights};
}

// ===========================================================================

template <typename T>
template <typename Grid>
constexpr auto Cartesian<T>::interpolate_value(const InterpolationContext& ctx,
                                               const Grid& grid,
                                               int64_t& n) const -> Complex {
  return detail::math::bilinear_interpolation<Complex>(
      std::get<0>(ctx.weights), std::get<1>(ctx.weights),
      std::get<2>(ctx.weights), std::get<3>(ctx.weights), grid(ctx.i1, ctx.j1),
      grid(ctx.i1, ctx.j2), grid(ctx.i2, ctx.j1), grid(ctx.i2, ctx.j2), n);
}

// ===========================================================================

template <typename T>
auto Cartesian<T>::interpolate(const geometry::Point& point, Quality& quality,
                               Accelerator& acc) const
    -> const ConstituentValues& {
  // Remove all previous values interpolated.
  acc.clear();

  // Compute interpolation context (grid indices and weights).
  const auto ctx = prepare_interpolation(point.lon(), point.lat(), lon_, lat_);

  // Reset values to undefined, if the point is outside the grid boundaries or
  // if any of the computed weights is NaN (not a number).
  auto reset_values_to_undefined = [&]() -> const ConstituentValues& {
    constexpr auto undefined_value =
        detail::math::construct_nan<std::complex<double>>();

    for (const auto& item : this->data_) {
      acc.emplace_back(item.first, undefined_value);
    }
    quality = kUndefined;
    return acc.values();
  };

  if (!ctx.is_valid()) {
    return reset_values_to_undefined();
  }

  auto grid = detail::Grid<std::complex<T>>(
      nullptr, static_cast<size_t>(lon_.size()),
      static_cast<size_t>(lat_.size()), row_major_);

  auto n = int64_t{0};
  for (const auto& item : this->data_) {
    grid.data(item.second.data());
    auto value = interpolate_value(ctx, grid, n);

    // The computed value lies within the grid boundaries, but it is NaN (not a
    // number).
    if (std::isnan(value.real()) || std::isnan(value.imag())) {
      return reset_values_to_undefined();
    }
    acc.emplace_back(item.first, value);
  }
  // n represents the number of valid grid corners used in the bilinear
  // interpolation (0, 1, 2, or 4).
  quality = static_cast<Quality>(n);
  return acc.values();
}

template <typename T>
auto Cartesian<T>::resample(const Axis& origin_lon, const Axis& origin_lat,
                            const Vector<std::complex<T>>& wave,
                            const bool row_major,
                            const size_t num_threads) const
    -> Vector<std::complex<T>> {
  if (wave.size() != origin_lon.size() * origin_lat.size()) {
    throw std::invalid_argument("wave size does not match expected size");
  }
  const auto target_nx = lon_.size();
  const auto target_ny = lat_.size();
  const auto total = static_cast<size_t>(target_nx * target_ny);

  auto resampled = Vector<std::complex<T>>(total);
  const auto origin_grid = detail::Grid<std::complex<T>>(
      wave.data(), static_cast<size_t>(origin_lon.size()),
      static_cast<size_t>(origin_lat.size()), row_major);

  auto thread = [&](const size_t start, const size_t end) -> void {
    for (auto idx = start; idx < end; ++idx) {
      int64_t n = 0;
      const auto lon = lon_(row_major_ ? idx / target_ny : idx % target_nx);
      const auto lat = lat_(row_major_ ? idx % target_ny : idx / target_nx);

      auto ctx = prepare_interpolation(lon, lat, origin_lon, origin_lat);
      resampled(static_cast<int64_t>(idx)) =
          ctx.is_valid() ? interpolate_value(ctx, origin_grid, n)
                         : detail::math::construct_nan<std::complex<T>>();
    }
  };
  detail::parallel_for(thread, total, num_threads);
  return resampled;
}

}  // namespace tidal_model
}  // namespace fes
