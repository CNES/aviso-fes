// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/interface/tidal_model.hpp
/// @brief Abstract tidal model.
#pragma once

#include <complex>
#include <cstdint>
#include <stdexcept>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"
#include "fes/geometry/point.hpp"
#include "fes/interface/wave_table.hpp"
#include "fes/types.hpp"

namespace fes {

/// @brief The quality flag for tidal model interpolation.
///
/// The quality flag indicates the reliability of interpolated values from the
/// tidal model:
/// * <b>0</b>: Value is undefined (no data available)
/// * <b>Positive values</b>: Value is interpolated using N data points (where N
///   equals the quality flag value)
/// * <b>Negative values</b>: Value is extrapolated using N data points (where N
///   equals the absolute value of the quality flag)
using Quality = int8_t;

/// @brief Undefined interpolation result.
constexpr Quality kUndefined = 0;

/// @brief Possible tide types
enum TideType : uint8_t {
  kTide = 0x01,    //!< Ocean Tide
  kRadial = 0x02,  //!< Radial tide
};

/// @brief Constituent values type
using ConstituentValues = std::vector<std::pair<ConstituentId, Complex>>;

/// @brief Base class for accelerating the interpolation of tidal models.
///
/// Accelerators are used to speed up the interpolation of tidal models by
/// caching the results of the last interpolation done. This class serves as a
/// base class for all tidal model accelerators.
class Accelerator {
 public:
  /// Default constructor
  /// @param[in] formulae The formulae used to calculate the astronomic angle.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @param[in] n_constituents The number of tidal constituents handled by the
  /// model.
  Accelerator(const angle::Formulae& formulae, const double time_tolerance,
              const size_t n_constituents)
      : time_tolerance_(time_tolerance),
        angle_{std::numeric_limits<double>::max(),
               angle::Astronomic(formulae)} {
    values_.reserve(n_constituents);
  }

  /// Default destructor
  virtual ~Accelerator() = default;

  /// @brief Casts the object to a pointer of type T.
  ///
  /// @tparam T The type to cast to.
  /// @return A pointer to the object casted to type T, or nullptr if the cast
  /// is not possible.
  template <typename T>
  constexpr auto cast() noexcept -> T* {
    auto* type_info = &typeid(*this);
    if (*type_info != typeid(T)) {
      return nullptr;
    }
    return dynamic_cast<T*>(this);
  }

  /// @brief Returns the tidal constituent values interpolated at the given
  /// point.
  /// @return the tidal constituent values interpolated at the given point.
  constexpr auto values() const noexcept -> const ConstituentValues& {
    return values_;
  }

  /// @brief Clears the cached interpolated values.
  auto clear() noexcept -> void { values_.clear(); }

  /// @brief Appends a tidal constituent value to the cached interpolated
  /// values.
  auto emplace_back(const ConstituentId& constituent,
                    const Complex& value) noexcept -> void {
    values_.emplace_back(constituent, value);
  }

  /// @brief Calculates the astronomic angle used to evaluate the tidal
  /// constituents at the given UTC time.
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  /// @return The astronomic angle corresponding to the given UTC time.
  auto calculate_angle(const double epoch) noexcept
      -> const angle::Astronomic& {
    if (std::abs(epoch - angle_.first) > time_tolerance_) {
      angle_.first = epoch;
      angle_.second.update(epoch);
    }
    return angle_.second;
  }

 protected:
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant
  double time_tolerance_;

  /// @brief The last angle used to evaluate the tidal constituents.
  std::pair<double, angle::Astronomic> angle_;

  /// @brief The tidal constituent values interpolated at the last point.
  ConstituentValues values_;
};

/// @brief Abstract class for a model of tidal constituents.
///
/// @tparam T The type of tidal constituents modelled.
template <typename T>
class TidalModelInterface
    : public std::enable_shared_from_this<TidalModelInterface<T>> {
 public:
  /// Build a tidal model with a given tide type.
  ///
  /// @param[in] tide_type The tide type handled by the model.
  explicit TidalModelInterface(TideType tide_type) : tide_type_(tide_type) {}

  /// Destructor
  virtual ~TidalModelInterface() = default;

  /// @brief Returns a pointer to the accelerator used to speed up the
  /// interpolation of tidal models.
  ///
  /// @param[in] formulae The formulae used to calculate the astronomic angle.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @return A pointer to the accelerator used to speed up the interpolation of
  /// tidal models.
  virtual auto accelerator(const angle::Formulae& formulae,
                           const double time_tolerance) const
      -> Accelerator* = 0;

  /// @brief Returns a wave table initialized with the tidal constituents
  /// modeled by this instance.
  ///
  /// @param[in] engine_type The engine type for the tidal constituent notation
  /// system.
  /// @return A unique pointer to an initialized wave table for tide prediction.
  auto wave_table(EngineType engine_type) const
      -> std::unique_ptr<WaveTableInterface> {
    auto wt = wave_table_factory(engine_type);
    wt->set_modeled_constituents(data_);
    wt->set_modeled_constituents(dynamic_);
    return wt;
  }

  /// Add a tidal constituent to the model.
  ///
  /// @param[in] ident The tidal constituent identifier.
  /// @param[in] wave The tidal constituent modelled.
  virtual auto add_constituent(const ConstituentId ident,
                               Vector<std::complex<T>> wave) -> void = 0;

  /// Add a tidal constituent to the model.
  ///
  /// @param[in] name The tidal constituent name. Search is not case sensitive.
  /// So  "Msqm", "MSQM" and "msqm" are equivalent.
  /// @param[in] wave The tidal constituent modelled.
  inline auto add_constituent(const std::string& name,
                              Vector<std::complex<T>> wave) -> void {
    try {
      add_constituent(constituents::parse(name), std::move(wave));
    } catch (const ConstituentValidationError&) {
      throw std::invalid_argument("constituent name not known: " + name);
    } catch (const std::invalid_argument& e) {
      throw std::invalid_argument("error adding constituent: " + name + ": " +
                                  e.what());
    }
  }

  /// Set the dynamic tidal constituents not interpolated by the model.
  ///
  /// @param[in] dynamic The dynamic tidal constituents.
  inline auto dynamic(const std::vector<std::string>& dynamic) -> void {
    dynamic_.clear();
    dynamic_.reserve(dynamic.size());
    for (auto& item : dynamic) {
      dynamic_.push_back(constituents::parse(item));
    }
  }

  /// Set the dynamic tidal constituents not interpolated by the model.
  ///
  /// @param[in] dynamic The dynamic tidal constituents.
  inline auto dynamic(std::vector<ConstituentId> dynamic) -> void {
    dynamic_ = std::move(dynamic);
  }

  /// Get the dynamic tidal constituents not interpolated by the model.
  constexpr auto dynamic() const noexcept -> const std::vector<ConstituentId>& {
    return dynamic_;
  }

  /// Interpolate the tidal constituent at a given point.
  ///
  /// @param[in] point The point to interpolate at.
  /// @param[out] quality A flag indicating if the point was extrapolated,
  /// interpolated or if the model is undefined.
  /// @param[inout] acc The accelerator to use.
  /// @return A list of interpolated tidal constituents.
  virtual auto interpolate(const geometry::Point& point, Quality& quality,
                           Accelerator& acc) const
      -> const ConstituentValues& = 0;

  /// Interpolate the tidal constituent at a given point.
  ///
  /// @param[in] point The point to interpolate at.
  /// @param[inout] wave_table The wave table to fill with the interpolated
  /// tidal constituents.
  /// @param[inout] acc The accelerator to use.
  /// @return A flag indicating if the point was extrapolated, interpolated or
  /// if the model is undefined.
  inline auto interpolate(const geometry::Point& point,
                          WaveTableInterface& wave_table,
                          Accelerator& acc) const -> Quality {
    Quality quality;
    for (auto&& item : this->interpolate(point, quality, acc)) {
      wave_table.set_tide(std::get<0>(item), std::get<1>(item));
    }
    return quality;
  }

  /// Get the tidal constituents handled by the model.
  constexpr auto data() const
      -> const std::map<ConstituentId, Vector<std::complex<T>>>& {
    return data_;
  }

  /// Clear all tidal constituents.
  inline auto clear() -> void {
    data_.clear();
    dynamic_.clear();
  }

  /// True if no tidal constituent is handled by the model.
  constexpr auto empty() const -> bool { return data_.empty(); }

  /// Get the number of tidal constituents handled by the model.
  constexpr auto size() const -> size_t { return data_.size(); }

  /// Get the tidal constituent identifiers handled by the model.
  inline auto identifiers() const -> std::vector<ConstituentId> {
    auto result = std::vector<ConstituentId>();
    result.reserve(data_.size());
    for (const auto& item : data_) {
      result.push_back(std::get<0>(item));
    }
    return result;
  }

  /// Get the tide type handled by the model.
  constexpr auto tide_type() const -> TideType { return tide_type_; }

  /// Return the memory usage of the tidal model in bytes.
  virtual auto memory_usage() const -> size_t {
    size_t memory = 0;
    for (const auto& item : data_) {
      memory += sizeof(std::get<0>(item)) + sizeof(std::get<1>(item)) +
                std::get<1>(item).size() * sizeof(std::complex<T>);
    }
    return memory;
  }

 protected:
  /// Tidal constituents handled by the model.
  std::map<ConstituentId, Vector<std::complex<T>>> data_{};

  /// List of tidal constituents handled by the model but not interpolated.
  std::vector<ConstituentId> dynamic_{};

  /// Tide type
  TideType tide_type_{TideType::kTide};
};

}  // namespace fes
