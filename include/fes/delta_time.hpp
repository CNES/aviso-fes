// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/delta_time.hpp
/// @brief Delta T (TT - UT1)
#pragma once
#include <array>

#include "fes/detail/math.hpp"
#include "fes/numbers.hpp"

namespace fes {

/// @brief Delta T entry structure
struct DeltaTEntry {
  double year;     ///< Decimal year
  double delta_t;  ///< Delta T in seconds
};

/// @brief Table generated dynamically from IERS (finals.all)
/// @note Generated on 2026-01-21T16:27:31+00:00Z
constexpr std::array<DeltaTEntry, 55> IERS_TABLE = {{
    {1973.0, 43.9444}, {1974.0, 44.9847}, {1975.0, 45.9713}, {1976.0, 46.9903},
    {1977.0, 48.0236}, {1978.0, 49.0803}, {1979.0, 50.0844}, {1980.0, 50.9650},
    {1981.0, 51.7885}, {1982.0, 52.5609}, {1983.0, 53.4066}, {1984.0, 54.0722},
    {1985.0, 54.6134}, {1986.0, 55.1033}, {1987.0, 55.5703}, {1988.0, 56.0721},
    {1989.0, 56.5631}, {1990.0, 57.2122}, {1991.0, 57.9439}, {1992.0, 58.7244},
    {1993.0, 59.5665}, {1994.0, 60.3887}, {1995.0, 61.2232}, {1996.0, 61.9739},
    {1997.0, 62.6409}, {1998.0, 63.2509}, {1999.0, 63.6502}, {2000.0, 63.9658},
    {2001.0, 64.1971}, {2002.0, 64.3981}, {2003.0, 64.5366}, {2004.0, 64.6356},
    {2005.0, 64.7790}, {2006.0, 64.9860}, {2007.0, 65.3179}, {2008.0, 65.6171},
    {2009.0, 65.9295}, {2010.0, 66.2152}, {2011.0, 66.4592}, {2012.0, 66.7574},
    {2013.0, 67.1050}, {2014.0, 67.4695}, {2015.0, 67.8619}, {2016.0, 68.3699},
    {2017.0, 68.7985}, {2018.0, 69.0995}, {2019.0, 69.3231}, {2020.0, 69.3891},
    {2021.0, 69.3312}, {2022.0, 69.2439}, {2023.0, 69.1967}, {2024.0, 69.1661},
    {2025.0, 69.1252}, {2026.0, 69.1160}, {2027.0, 69.0928},
}};

namespace detail {

/// @brief Binary search to find the lower bound by year.
/// @tparam Iterator The iterator type.
/// @param[in] first The beginning of the range.
/// @param[in] last The end of the range.
/// @param[in] value The year value to search for.
/// @return An iterator to the first element with year >= value, or last if
/// none.
template <typename Iterator>
constexpr auto lower_bound_by_year(Iterator first, Iterator last, double value)
    -> Iterator {
  while (first != last) {
    auto count = last - first;
    auto mid = first + count / 2;
    if (mid->year < value) {
      first = mid + 1;
    } else {
      last = mid;
    }
  }
  return first;
}

/// @brief Linear interpolation between two table entries.
/// @param[in] p1 The lower bound entry.
/// @param[in] p2 The upper bound entry.
/// @param[in] year The target year.
/// @return Interpolated delta T value.
constexpr auto interpolate(const DeltaTEntry& p1, const DeltaTEntry& p2,
                           double year) noexcept -> double {
  double fraction = (year - p1.year) / (p2.year - p1.year);
  return p1.delta_t + fraction * (p2.delta_t - p1.delta_t);
}

/// @brief Morrison & Stephenson (2004) long-term parabola.
/// @param[in] year Decimal year.
/// @return Delta T in seconds.
/// @note Valid approximation for years far from modern era.
constexpr auto morrison_stephenson_parabola(double year) noexcept -> double {
  // Reference for long-term parabola
  constexpr double kMorrisonEpoch = 1820.0;
  // Parabolic coefficient (s/century²)
  constexpr double kMorrisonCoeff = 32.0;
  // Constant offset
  constexpr double kMorrisonOffset = -20.0;

  double t_cent = (year - kMorrisonEpoch) / 100.0;
  return kMorrisonOffset + kMorrisonCoeff * t_cent * t_cent;
}

/// @brief Polynomial approximation for 1900-1973 era.
/// @param[in] year Decimal year (should be in range [1900, 1973]).
/// @return Delta T in seconds.
/// @note Coefficients from Espenak & Meeus polynomial fit.
constexpr auto polynomial_1900_1973(double year) noexcept -> double {
  double t = year - 1900.0;
  return -2.79 + 1.494119 * t - 0.0598939 * t * t +
         0.0061966 * math::pow<3>(t) - 0.000197 * math::pow<4>(t);
}

/// @brief Polynomial approximation for 1800-1900 era.
/// @param[in] year Decimal year (should be in range [1800, 1900]).
/// @return Delta T in seconds.
constexpr auto polynomial_1800_1900(double year) noexcept -> double {
  double t = year - 1900.0;
  return -2.50 + 228.95 * (t / 100.0) + 5218.61 * math::pow<2>(t / 100.0) +
         56282.84 * math::pow<3>(t / 100.0) +
         324011.78 * math::pow<4>(t / 100.0) +
         1061660.75 * math::pow<5>(t / 100.0) +
         2087298.89 * math::pow<6>(t / 100.0) +
         2513807.78 * math::pow<7>(t / 100.0) +
         1818961.41 * math::pow<8>(t / 100.0) +
         727058.63 * math::pow<9>(t / 100.0) +
         123563.95 * math::pow<10>(t / 100.0);
}

/// @brief Polynomial approximation for 1700-1800 era.
/// @param[in] year Decimal year (should be in range [1700, 1800]).
/// @return Delta T in seconds.
constexpr auto polynomial_1700_1800(double year) noexcept -> double {
  double t = year - 1700.0;
  return 8.83 + 0.1603 * t - 0.0059285 * t * t + 0.00013336 * math::pow<3>(t) -
         math::pow<4>(t) / 1174000.0;
}

/// @brief Polynomial approximation for 1600-1700 era.
/// @param[in] year Decimal year (should be in range [1600, 1700]).
/// @return Delta T in seconds.
constexpr auto polynomial_1600_1700(double year) noexcept -> double {
  double t = year - 1600.0;
  return 120.0 - 0.9808 * t - 0.01532 * t * t + math::pow<3>(t) / 7129.0;
}

}  // namespace detail

/// @brief Convert Unix epoch to decimal year.
/// @param[in] epoch Seconds since 1970-01-01T00:00:00Z.
/// @return Decimal year.
constexpr auto epoch_to_year(double epoch) noexcept -> double {
  double jd = (epoch / numbers::kSecondsPerDay) + numbers::kUnixEpochJulianDay;
  double t = (jd - static_cast<double>(numbers::kJ2000JulianDay)) /
             static_cast<double>(numbers::kDaysPerCentury);
  return 2000.0 + (t * 100.0);
}

/// @brief Computes TT - UT1 (Delta T) using IERS tables for modern era
/// and historical polynomials for earlier periods.
/// @param[in] epoch Number of seconds elapsed since 1970-01-01T00:00:00Z.
/// @return Delta T in seconds.
///
/// @details
/// - Modern era (1973-present): Linear interpolation of IERS table values
/// - 1900-1973: Espenak & Meeus polynomial
/// - 1800-1900: Historical polynomial fit
/// - 1700-1800: Historical polynomial fit
/// - 1600-1700: Historical polynomial fit
/// - Pre-1600 and future: Morrison & Stephenson (2004) parabola
///
/// @note Future extrapolation becomes increasingly uncertain.
/// @see https://www.iers.org for current Earth rotation data.
FES_MATH_CONSTEXPR auto fetch_delta_time(double epoch) -> double {
  double year = epoch_to_year(epoch);

  // Modern era: IERS table lookup with linear interpolation
  if (year >= IERS_TABLE.front().year && year <= IERS_TABLE.back().year) {
    auto it =
        detail::lower_bound_by_year(IERS_TABLE.begin(), IERS_TABLE.end(), year);

    // Exact match on first entry
    if (it == IERS_TABLE.begin()) {
      return it->delta_t;
    }

    // Interpolate between adjacent entries
    return detail::interpolate(*(it - 1), *it, year);
  }

  // Future extrapolation
  if (year > IERS_TABLE.back().year) {
    return detail::morrison_stephenson_parabola(year);
  }

  // Historical polynomials
  if (year >= 1900.0) {
    return detail::polynomial_1900_1973(year);
  }
  if (year >= 1800.0) {
    return detail::polynomial_1800_1900(year);
  }
  if (year >= 1700.0) {
    return detail::polynomial_1700_1800(year);
  }
  if (year >= 1600.0) {
    return detail::polynomial_1600_1700(year);
  }

  // Deep historical: Morrison & Stephenson parabola
  return detail::morrison_stephenson_parabola(year);
}

}  // namespace fes
