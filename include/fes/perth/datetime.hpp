#pragma once

#include <cmath>
#include <cstdint>

#include "fes/numbers.hpp"

namespace fes {
namespace perth {

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
      whole_seconds * numbers::kMicrosecondsPerSecond +
      static_cast<int64_t>(frac_part * numbers::kMicrosecondsPerSecond);

  const auto days = microseconds /
                    (numbers::kMicrosecondsPerSecond * numbers::kSecondsPerDay);
  const auto fractional_mcsec =
      microseconds %
      (numbers::kMicrosecondsPerSecond * numbers::kSecondsPerDay);

  return days +
         static_cast<double>(fractional_mcsec) /
             (numbers::kMicrosecondsPerSecond * numbers::kSecondsPerDay) +
         40587.0;
}

}  // namespace perth
}  // namespace fes
