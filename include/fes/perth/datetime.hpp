#pragma once

#include <cmath>
#include <cstdint>

namespace fes {
namespace perth {

/// @brief The Modified Julian Date epoch in seconds.
constexpr double kModifiedJulianEpoch = 2400000.5;

/// @brief The number of seconds in a day.
constexpr int64_t kSecondsPerDay = 86400;

/// @brief The number of microseconds in a second.
constexpr int64_t kMicrosecondsPerSecond = 1000000;

/// @brief The number of days in a century.
constexpr int64_t kDaysPerCentury = 36525;

/// @brief Julian Day Number for the J2000.0 epoch.
constexpr int64_t kJ2000JulianDay = 2451545;

/// @brief Converts a UTC epoch given in seconds since 1970-01-01T00:00:00Z
/// (1970-01-01) to a Modified Julian Date (MJD).
/// @param[in] epoch Number of seconds elapsed since 1970-01-01T00:00:00Z.
/// @return The Modified Julian Date corresponding to the epoch.
inline auto epoch_to_modified_julian_date(const double epoch) -> double {
  // 1 MJD = 86400 seconds, and MJD starts at 1858-11-17
  // 1970-01-01 is 40587 days after 1858-11-17
  double int_part{};
  const double frac_part = std::modf(epoch, &int_part);

  const auto whole_seconds = static_cast<int64_t>(int_part);
  const auto microseconds =
      whole_seconds * kMicrosecondsPerSecond +
      static_cast<int64_t>(frac_part * kMicrosecondsPerSecond);

  const auto days = microseconds / (kMicrosecondsPerSecond * kSecondsPerDay);
  const auto fractional_mcsec =
      microseconds % (kMicrosecondsPerSecond * kSecondsPerDay);

  return days +
         static_cast<double>(fractional_mcsec) /
             (kMicrosecondsPerSecond * kSecondsPerDay) +
         40587.0;
}

}  // namespace perth
}  // namespace fes
