// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/table.hpp
/// @brief Table of tidal constituents
#pragma once
#include <Eigen/Core>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "fes/angle/astronomic.hpp"
#include "fes/darwin/wave.hpp"
#include "fes/tidal_constituents.hpp"
#include "fes/types.hpp"

namespace fes {
namespace darwin {

/// @brief Iterator on the waves of a table.
///
/// The iterator skips the waves that are not selected (the shared pointer is
/// null).
///
/// @tparam Range Range of waves
/// @tparam RangeType Type of the range
template <typename Range, typename RangeType>
class WaveTableIterator : public std::iterator_traits<RangeType> {
 public:
  /// Type of value returned by the iterator.
  using value_t = typename std::iterator_traits<RangeType>::value_type;

  /// Returns the underlying iterator
  constexpr auto base() const noexcept -> RangeType { return it_; }

  /// Returns the current item
  constexpr auto operator*() noexcept -> value_t { return *it_; }

  /// Advances the iterator
  constexpr auto operator++() noexcept -> WaveTableIterator& {
    ++it_;
    normalize();
    return *this;
  }

  /// Tests if the iterators are equal
  /// @param[in] rhs iterator to compare
  /// @returns true if the two iterators are equal
  constexpr auto operator==(const WaveTableIterator& rhs) const noexcept
      -> bool {
    return it_ == rhs.it_;
  }

  /// Tests if the iterators are different
  /// @param[in] rhs iterator to compare
  /// @returns true if the two iterators are different
  constexpr auto operator!=(const WaveTableIterator& rhs) const noexcept
      -> bool {
    return !(*this == rhs);
  }

  /// Tests if the iterator are equal
  /// @param[in] rhs iterator to compare
  /// @returns true if the two iterators are equal
  template <typename OtherRange, typename OtherRangeType>
  constexpr auto operator==(
      const WaveTableIterator<OtherRange, OtherRangeType>& rhs) const noexcept
      -> bool {
    return it_ == rhs.base();
  }

  /// Tests if the iterator are different
  /// @param[in] rhs iterator to compare
  /// @returns true if the two iterators are different
  template <typename OtherRange, typename OtherRangeType>
  constexpr auto operator!=(
      const WaveTableIterator<OtherRange, OtherRangeType>& rhs) const noexcept
      -> bool {
    return !(*this == rhs);
  }

  /// Constructor
  /// @param[in] it Iterator on the beginning of the range
  /// @param[in] end Iterator on the end of the range
  constexpr WaveTableIterator(RangeType it, RangeType end) noexcept
      : it_{std::move(it)}, end_{std::move(end)} {
    normalize();
  }

 private:
  /// Normalizes the iterator (skips items that are evaluated to false)
  constexpr auto normalize() noexcept -> void {
    for (; it_ != end_ && !(*it_); it_++) {
    }
  }

  /// Iterator on the current item
  RangeType it_;
  /// Iterator on the end of the range
  RangeType end_;
};

/// Properties of tide waves computed
class WaveTable : public TidalConstituents {
 public:
  /// Alias to a mutable iterator on the waves
  using iterator_t =
      WaveTableIterator<std::vector<std::shared_ptr<Wave>>,
                        std::vector<std::shared_ptr<Wave>>::iterator>;

  /// Alias to a constant iterator on the waves
  using const_iterator_t =
      WaveTableIterator<std::vector<std::shared_ptr<Wave>>,
                        std::vector<std::shared_ptr<Wave>>::const_iterator>;

  /// Build a table from a list of constituent names
  explicit WaveTable(const std::vector<std::string>& waves = {});

  /// Build a table from a list of constituent identifiers
  explicit WaveTable(const std::vector<Constituent>& waves);

  /// Create a wave properties from its identifier
  /// @param[in] ident Wave identifier
  /// @return Wave properties
  static auto wave_factory(const Constituent ident) -> std::shared_ptr<Wave>;

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[out] value The tide value
  void set_tide(uint8_t ident, const Complex& value) override {
    if (ident >= wave_index_.size()) {
      throw std::out_of_range("ident out of range");
    }
    set_tide(static_cast<Constituent>(ident), value);
  }

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[in] value The tide value
  void set_tide(Constituent ident, const Complex& value) {
    (*this)[ident]->tide(value);
  }

  /// Compute nodal corrections.
  ///
  /// @param[in] angles Astronomic angle, indicating the date on which the tide
  /// is to be calculated.
  inline auto compute_nodal_corrections(
      const angle::Astronomic& angles) noexcept -> void {
    std::for_each(waves_.begin(), waves_.end(), [&angles](auto& item) {
      if (item) {
        item->nodal_a(angles);
        item->nodal_g(angles);
      }
    });
  }

  /// @brief Compute waves by admittance from these 7 major ones : O1, Q1, K1,
  /// 2N2, N2, M2, K2.
  auto admittance() -> void;

  /// Get the wave properties from its index
  inline auto operator[](const size_t index) const
      -> const std::shared_ptr<Wave>& {
    if (index >= wave_index_.size()) {
      throw std::out_of_range("index out of range");
    }
    return waves_[static_cast<size_t>(wave_index_[index])];
  }

  /// Get the wave properties from its identifier
  constexpr auto operator[](const Constituent ident) const
      -> const std::shared_ptr<Wave>& {
    return ((*this).*getter_)(ident);
  }

  /// Get the wave properties from its identifier
  constexpr auto operator[](const Constituent ident) -> std::shared_ptr<Wave>& {
    return std::remove_const_t<std::shared_ptr<Wave>&>(
        ((*this).*getter_)(ident));
  }

  /// Returns an iterator to the beginning of the wave table
  inline auto begin() const noexcept -> const_iterator_t {
    return const_iterator_t{waves_.begin(), waves_.end()};
  }

  /// Returns an iterator to the end of the wave table
  inline auto end() const noexcept -> const_iterator_t {
    return const_iterator_t{waves_.end(), waves_.end()};
  }

  /// Returns an iterator to the beginning of the wave table
  inline auto begin() noexcept -> iterator_t {
    return iterator_t{waves_.begin(), waves_.end()};
  }

  /// Returns an iterator to the end of the wave table
  inline auto end() noexcept -> iterator_t {
    return iterator_t{waves_.end(), waves_.end()};
  }

  /// Returns the list of tidal constituents handled by this instance
  inline auto constituents() const -> std::vector<std::string> {
    auto names = std::vector<std::string>();
    names.reserve(waves_.size());
    for (const auto& item : waves_) {
      if (item) {
        names.emplace_back(item->name());
      }
    }
    return names;
  }

  /// Returns the size of the table
  inline auto size() const noexcept -> size_t { return wave_index_.size(); }

  /// Return the list of tidal waves such that their period is more than
  /// twice the duration of the time series analyzed
  ///
  /// @param[in] duration Duration of the time series analyzed in seconds
  /// @param[in] f Number of times the period of the wave is greater than
  /// the duration of the time series analyzed
  /// @return List of selected tidal waves.
  static auto select_waves_for_analysis(double duration, double f = 2.0)
      -> std::vector<std::string>;

  /// Harmonic analysis of the tide
  ///
  /// The harmonic analysis method consists in expressing the ocean tidal
  /// variations as a sum of independent constituents accordingly to the tidal
  /// potential spectrum. Then the sea surface elevation at a point
  /// \f$(x, y)\f$ and time \f$(t\f$) can be expressed as a linear sum as
  /// follow:
  ///
  /// \f[
  ///   S_{ap} = S_{0}(x, y) + \sum_{k=0}^n f_{k}(t)S_{k}(x, y)
  ///     \times cos [\omega_{k}t + {v}_{k}(t) + u_{k}(t) - G_{k}(x,y)]
  /// \f]
  ///
  /// where:
  ///   * \f$(n\f$) is the number of constituents,
  ///   * \f$S_{0}(x, y)\f$ is the mean sea level,
  ///   * \f$S_{k}(x, y)\f$ is the amplitude of the constituent of index
  ///     \f$(k\f$),
  ///   * \f$G_{k}(x, y)\f$ is the phase lag relative to Greenwich time,
  ///   * \f$w_{k}\f$ is the angular frequency of the constituent of index
  ///     \f$(k\f$),
  ///   * \f$v_{k}\f$ is the astronomical argument at time \f$(t\f$),
  ///   * \f$f_{k}(t)\f$ is the nodal correction coefficient applied to
  ///     the amplitude of the constituent of index \f$(k\f$),
  ///   * \f$u_{k}(t)\f$ is the nodal correction coefficient applied to
  ///     the phase of the constituent of index \f$(k\f$).
  ///
  ///   The a priori analysis spectrum includes the most important astronomical
  ///   constituents in the Darwin development, completed by Schureman in 1958,
  ///   and many non-linear waves. The definition of tidal constants and
  ///   astronomical arguments is taken from FES2014 tidal prediction software
  ///   and a complete definition of waves is also available in
  ///   Schureman (1958).
  ///   This spectrum is the most commonly used for harmonic analysis due the
  ///   simplification given by the nodal correction concept (\f$(f\f$) and
  ///   \f$(u\f$) coefficients above) which allows dealing with slow motions of
  ///   the lunar ascending node and reducing the number of constituents in the
  ///   tidal spectrum. More details about this harmonic analysis method can be
  ///   found in Ponchaut et al. 1999.
  ///
  /// @param[in] h Sea level
  /// @param[in] f Nodal correction coefficient applied to the
  /// amplitude of the constituents analyzed.
  /// @param[in] vu Astronomical argument at time \f$(t\f$) + the
  /// nodal correction coefficient applied to the phase of the
  /// constituents analyzed.
  static auto harmonic_analysis(const Eigen::Ref<const Eigen::VectorXd>& h,
                                const DynamicRef<const Eigen::MatrixXd>& f,
                                const DynamicRef<const Eigen::MatrixXd>& vu)
      -> Eigen::VectorXcd;

  /// Calculate the tide of a given time series.
  ///
  /// @param[in] epoch Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] wave Tidal wave properties computed by an harmonic analysis.
  /// @return the tide at the given time.
  /// @param[in] formulae The formulae used to compute the astronomical angles.
  /// @return the tide at the given time.
  auto tide_from_tide_series(
      const Eigen::Ref<const Eigen::VectorXd>& epoch,
      const Eigen::Ref<const Eigen::VectorXcd>& wave,
      const angle::Formulae& formulae = angle::Formulae::kSchuremanOrder3) const
      -> Eigen::VectorXd;

  /// Calculate the tide for a given date from a grid describing the properties
  /// of tidal waves over an area.
  ///
  /// @param[in] epoch Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] wave Tidal wave properties computed by an harmonic analysis.
  /// @param[in] formulae The formulae used to compute the astronomical angles.
  /// @param[in] num_threads Number of threads to use for the computation. If
  /// set to 0, the number of threads is automatically determined.
  auto tide_from_mapping(
      double epoch, const DynamicRef<const Eigen::MatrixXcd>& wave,
      const angle::Formulae& formulae = angle::Formulae::kSchuremanOrder3,
      size_t num_threads = 0) const -> Eigen::MatrixXd;

  /// Compute nodal modulations for amplitude and phase.
  ///
  /// @param[in] epoch: Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] formulae The formulae used to compute the astronomical angles.
  /// @return The nodal correction for amplitude, v greenwich argument) + u
  /// (nodal correction for phase).
  /// @throw std::invalid_argument if the size of the epoch vector is not
  /// equal to the size of the leap seconds vector.
  auto compute_nodal_modulations(const Eigen::Ref<const Eigen::VectorXd>& epoch,
                                 const angle::Formulae& formulae) const
      -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>;

 private:
  /// Typename to a function pointer to get a wave from the table
  using Getter =
      const std::shared_ptr<Wave>& (WaveTable::*)(const Constituent) const;

  /// Get a wave from the table
  Getter getter_{nullptr};

  /// Wave table
  std::vector<std::shared_ptr<Wave>> waves_{};

  /// An array that maps linear indices (0, 1, 2, 3, ...) to the wave
  /// identifiers defined in the table. If the table is complete, this mapping
  /// is an identity mapping {0:0, 1:1, 2:2, ...}.
  std::vector<uint8_t> wave_index_{};

  /// Get a wave from the table
  ///
  /// @param[in] ident Wave identifier
  /// @return The wave
  auto direct_access(const Constituent ident) const
      -> const std::shared_ptr<Wave>& {
    return waves_[static_cast<size_t>(ident)];
  }

  /// Get a wave from the table
  ///
  /// @param[in] ident Wave identifier
  /// @return The wave
  inline auto sparse_access(const Constituent ident) const
      -> const std::shared_ptr<Wave>& {
    const auto& result = waves_[static_cast<size_t>(ident)];
    if (result == nullptr) {
      auto msg = std::string("Wave ") + WaveTable::wave_factory(ident)->name() +
                 " is not available";
      throw std::out_of_range(msg);
    }
    return result;
  }
};

}  // namespace darwin
}  // namespace fes
