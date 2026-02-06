// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/interface/wave.hpp
/// @brief Tidal wave interface.
#pragma once

#include <memory>
#include <string>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"
#include "fes/detail/math.hpp"
#include "fes/types.hpp"

namespace fes {

/// @brief Frequency unit enumeration.
enum FrequencyUnit : uint8_t {
  kRadianPerHour,  ///< Radian per hour
  kDegreePerHour   ///< Degree per hour
};

/// @brief Angular unit enumeration.
enum AngleUnit : uint8_t {
  kRadian,  ///< Radian
  kDegree   ///< Degree
};

/// @brief Gets the frequency scale factor based on the specified unit.
/// @tparam U The frequency unit.
/// @return The frequency scale factor.
template <FrequencyUnit U>
constexpr auto frequency_scale() noexcept -> double {
  return U == FrequencyUnit::kRadianPerHour
             ? 1.0
             : 180.0 / detail::math::pi<double>();
}

/// @brief Possible type of tidal wave.
enum WaveType : uint8_t {
  kLongPeriod,  //!< Long period tidal waves
  kShortPeriod  //!< Short period tidal waves
};

/// @brief Arguments required to compute nodal corrections.
class NodalCorrectionsArgs {
  /// Astronomic angles used to compute the nodal corrections
  angle::Astronomic angles_;

  /// Whether to group modulations
  bool group_modulations_{false};

 public:
  /// @brief Constructor.
  /// @param[in] angles Astronomic angles used to compute the nodal corrections.
  explicit constexpr NodalCorrectionsArgs(const angle::Astronomic& angles,
                                          bool group_modulations = false)
      : angles_(angles), group_modulations_(group_modulations) {}

  /// @brief Gets the astronomic angles.
  /// @return The astronomic angles.
  constexpr auto angles() const noexcept -> const angle::Astronomic& {
    return angles_;
  }

  /// Gets the mutable astronomic angles.
  /// @return The mutable astronomic angles.
  constexpr auto angles() noexcept -> angle::Astronomic& { return angles_; }

  /// @brief Whether to group modulations together when computing nodal
  /// corrections.
  /// @return Whether to group modulations together when computing nodal
  /// corrections.
  constexpr auto group_modulations() const noexcept -> bool {
    return group_modulations_;
  }
};

/// @brief Tidal wave interface.
class WaveInterface {
 public:
  /// @brief Constructor.
  WaveInterface(const ConstituentId ident, WaveType type) noexcept
      : id_(ident), type_(type) {}

  /// @brief Destructor.
  virtual ~WaveInterface() = default;

  /// @brief Clones the wave.
  /// @return A unique pointer to the cloned wave.
  virtual auto clone() const -> std::unique_ptr<WaveInterface> = 0;

  /// @brief Indicates whether the tide is provided by the model.
  /// @return `true` if the tide is provided by the model, `false` otherwise.
  constexpr auto is_modeled() const noexcept -> bool { return is_modeled_; }

  /// @brief Sets whether the tide is provided by the model.
  /// @param is_modeled `true` if the tide is provided by the model, `false`
  /// otherwise.
  constexpr void set_is_modeled(bool is_modeled) noexcept {
    is_modeled_ = is_modeled;
  }

  /// @brief Gets the constituent identifier.
  /// @return The constituent identifier.
  constexpr auto ident() const noexcept -> ConstituentId { return id_; }

  /// @brief Gets the name of the tidal wave.
  /// @return The name of the tidal wave.
  inline auto name() const -> const char* { return constituents::name(id_); }

  /// @brief Gets the latex name of the tidal wave.
  /// @return The latex name of the tidal wave.
  auto latex_name() const -> std::string;

  /// @brief Gets the type of tidal wave.
  /// @return The type of tidal wave.
  constexpr auto type() const noexcept -> WaveType { return type_; }

  /// @brief Gets the frequency in radians per hour.
  /// @return The frequency in radians per hour.
  virtual auto frequency() const noexcept -> double = 0;

  /// @brief Gets the frequency
  /// @param unit The frequency unit.
  /// @return The frequency in radians per hour or degrees per hour.
  template <FrequencyUnit U>
  constexpr auto frequency() const noexcept -> double {
    return U == FrequencyUnit::kRadianPerHour
               ? frequency()
               : detail::math::degrees(frequency());
  }

  /// Gets the period of the wave (hours)
  constexpr auto period() const noexcept -> double {
    return detail::math::two_pi<double>() / frequency();
  }

  /// @brief Gets the tide value.
  /// @return The tide value.
  constexpr auto tide() const noexcept -> const Complex& { return tide_; }

  /// @brief Sets the tide value.
  /// @param tide The tide value.
  constexpr void set_tide(const Complex& tide) noexcept { tide_ = tide; }

  /// @brief Computes the tide value, applying the nodal corrections.
  /// @return The tide value after applying the nodal corrections.
  /// @note Computes
  /// @f$\Re(T e^{-i(v+u)})=\Re(T)\cos(v+u)+\Im(T)\sin(v+u)@f$
  FES_MATH_CONSTEXPR auto corrected_tide() const noexcept -> double {
    auto phi = vu();
    auto raw_tide = tide();
    return f() *
           (raw_tide.real() * std::cos(phi) + raw_tide.imag() * std::sin(phi));
  }

  /// @brief Gets the Greenwich argument, in radians.
  /// @return The Greenwich argument.
  constexpr auto v() const noexcept -> double { return v_; }

  /// @brief Gets the nodal correction for phase, in radians.
  /// @return The nodal correction for phase.
  constexpr auto u() const noexcept -> double { return u_; }

  /// @brief Gets the nodal correction for amplitude, in radians.
  /// @return The nodal correction for amplitude.
  constexpr auto f() const noexcept -> double { return f_; }

  /// @brief Gets the sum of the Greenwich argument and the nodal phase
  /// correction, in radians.
  /// @return The sum of the Greenwich argument and the nodal phase
  /// correction, normalized to the interval [0, 2π).
  constexpr auto vu() const noexcept -> double {
    return detail::math::normalize_angle(v_ + u_, 0.0,
                                         detail::math::two_pi<double>());
  }

  /// @brief Sets the nodal corrections directly (for bulk updates with known
  /// values).
  /// @param f Nodal correction for amplitude.
  /// @param u Nodal correction for phase.
  /// @param v Greenwich argument.
  /// @param unit The angular unit of the nodal corrections.
  constexpr void set_nodal_corrections(double f, double u, double v,
                                       AngleUnit unit) noexcept {
    f_ = f;
    u_ = unit == AngleUnit::kRadian ? u : detail::math::radians(u);
    v_ = unit == AngleUnit::kRadian ? v : detail::math::radians(v);
  }

  /// @brief Computes the nodal corrections for the wave.
  /// @param[in] angles Astronomical angles used to compute nodal corrections.
  /// @param[in] group_modulations If true, applies group modulations to nodal
  /// corrections.
  virtual auto compute_nodal_corrections(const angle::Astronomic& angles,
                                         const bool group_modulations)
      -> void = 0;

  /// Gets the XDO numerical representation of the wave
  virtual auto xdo_numerical() const -> std::string = 0;

  /// Gets the XDO alphabetical representation of the wave
  virtual auto xdo_alphabetical() const -> std::string = 0;

  /// Gets the Doodson number of the wave
  /// @note The 7th number follows the convention established in Doodson &
  /// Warburg's 1941 book. This number can be 0, 1, 2, or -1, representing
  /// multiples of 90 degrees added to the tidal argument when using cosine
  /// functions
  virtual auto doodson_numbers() const -> Vector7b = 0;

 protected:
  Complex tide_{};     ///< Tide value
  double v_{};         ///< Greenwich argument
  double f_{};         ///< Nodal correction for amplitude
  double u_{};         ///< Nodal correction for phase
  ConstituentId id_;   ///< Constituent identifier
  WaveType type_;      ///< Type of tidal wave
  bool is_modeled_{};  ///< Indicates whether the tide is provided by the model
};

}  // namespace fes
