// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/delta_time.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>

namespace fes {
namespace {

/// Number of seconds in a Julian year.
constexpr double kSecondsPerJulianYear = 365.25 * 86400.0;

/// Delta T value at a calendar date (00:00:00Z).
struct DatedValue {
  int year;        ///< Year
  int month;       ///< Month
  int day;         ///< Day
  double delta_t;  ///< Delta T in seconds
};

/// Delta T value at a Julian epoch year (as counted by epoch_to_year).
struct YearValue {
  double year;     ///< Julian epoch year
  double delta_t;  ///< Delta T in seconds
};

/// Unix epoch of a proleptic Gregorian date at 00:00:00Z.
constexpr auto epoch_of(int year, int month, int day) -> double {
  // days_from_civil, H. Hinnant, http://howardhinnant.github.io/date_algorithms
  year -= month <= 2 ? 1 : 0;
  const int era = (year >= 0 ? year : year - 399) / 400;
  const int yoe = year - (era * 400);
  const int doy =
      (((153 * (month > 2 ? month - 3 : month + 9)) + 2) / 5) + day - 1;
  const int doe = (yoe * 365) + (yoe / 4) - (yoe / 100) + doy;
  return static_cast<double>((era * 146097) + doe - 719468) * 86400.0;
}

/// Unix epoch of a DatedValue.
constexpr auto epoch_of(const DatedValue& item) -> double {
  return epoch_of(item.year, item.month, item.day);
}

/// Unix epoch of a Julian epoch year, the inverse of epoch_to_year.
constexpr auto epoch_of_julian_year(double year) -> double {
  return (((year - 2000.0) * 365.25) +
          static_cast<double>(numbers::kJ2000JulianDay) -
          numbers::kUnixEpochJulianDay) *
         86400.0;
}

/// Unix epoch of a Modified Julian Date.
constexpr auto epoch_of_mjd(double mjd) -> double {
  return (mjd + numbers::kModifiedJulianEpoch - numbers::kUnixEpochJulianDay) *
         86400.0;
}

/// Unix epoch of the first entry of the IERS table.
auto table_first_epoch() -> double {
  return epoch_of_mjd(IERS_TABLE.front().mjd);
}

/// Unix epoch of the last entry of the IERS table.
auto table_last_epoch() -> double {
  return epoch_of_mjd(IERS_TABLE.back().mjd);
}

}  // namespace

// Test epoch_to_year conversion
TEST(DeltaT, EpochToYear) {
  // Unix epoch should be 1970.0
  EXPECT_NEAR(epoch_to_year(0.0), 1970.0, 0.01);

  // J2000 epoch: 2000-01-01T12:00:00Z
  double j2000_epoch = 946728000.0;  // Seconds since Unix epoch
  EXPECT_NEAR(epoch_to_year(j2000_epoch), 2000.0, 0.01);

  // Mid-year 2020 approximately
  double mid_2020 = 1593561600.0;  // 2020-07-01T00:00:00Z
  EXPECT_NEAR(epoch_to_year(mid_2020), 2020.5, 0.01);
}

// The table holds monthly means stamped at the mean epoch of their samples,
// and is reproduced exactly at its own timestamps.
TEST(DeltaT, TableNodes) {
  for (std::size_t ix = 1; ix < IERS_TABLE.size(); ++ix) {
    EXPECT_GT(IERS_TABLE[ix].mjd, IERS_TABLE[ix - 1].mjd);
    EXPECT_LE(IERS_TABLE[ix].mjd - IERS_TABLE[ix - 1].mjd, 31.0);
  }
  for (const auto& item : IERS_TABLE) {
    EXPECT_NEAR(fetch_delta_time(epoch_of_mjd(item.mjd)), item.delta_t, 1e-9)
        << "MJD " << item.mjd;
  }
}

// Delta T is continuous one second either side of every branch boundary.
//
// Over 2 s, a continuous function changes by at most its steepest slope times
// 2 s. The bound on |dDeltaT/dt| given for each boundary is above the slope of
// the formulas meeting there (published polynomials, parabola, table segments,
// trend, handover and blend weights: 18.7 s/yr at -500, 10.0 s/yr at 500, at
// most 2.2 s/yr elsewhere); 1e-9 s absorbs the rounding of values up to
// 2e4 s. The largest tolerance (-500) is 1.3e-6 s, and the table end is
// checked to 6.4e-8 s.
TEST(DeltaT, ContinuityAtBranchBoundaries) {
  struct Boundary {
    const char* name;
    double epoch;
    double max_rate;  ///< Upper bound of |dDeltaT/dt| (s/yr) around epoch
  };
  const auto handover_start = epoch_of_julian_year(
      epoch_to_year(table_first_epoch()) - detail::kHandoverYears);
  const std::array<Boundary, 15> boundaries = {{
      {"-500", epoch_of_julian_year(-500.0), 20.0},
      {"500", epoch_of_julian_year(500.0), 11.0},
      {"1600", epoch_of_julian_year(1600.0), 2.0},
      {"1700", epoch_of_julian_year(1700.0), 1.0},
      {"1800", epoch_of_julian_year(1800.0), 1.0},
      {"1860", epoch_of_julian_year(1860.0), 1.0},
      {"1900", epoch_of_julian_year(1900.0), 2.0},
      {"1920", epoch_of_julian_year(1920.0), 2.0},
      {"1941", epoch_of_julian_year(1941.0), 1.0},
      {"1961", epoch_of_julian_year(1961.0), 1.0},
      {"table handover start", handover_start, 2.0},
      {"table start", table_first_epoch(), 2.0},
      {"table end", table_last_epoch(), 1.0},
      {"long-term blend start",
       epoch_of_julian_year(detail::kLongTermBlendStart), 1.0},
      {"long-term blend end", epoch_of_julian_year(detail::kLongTermBlendEnd),
       3.0},
  }};
  for (const auto& item : boundaries) {
    const auto tolerance = (item.max_rate * 2.0 / kSecondsPerJulianYear) + 1e-9;
    EXPECT_NEAR(fetch_delta_time(item.epoch - 1.0),
                fetch_delta_time(item.epoch + 1.0), tolerance)
        << "Boundary: " << item.name;
  }
}

// No jump anywhere between -1000 and 2300: day-to-day changes stay below a
// bound of the steepest slope of the model in each era (18.7 s/yr near -500,
// 1.5 s/yr between 1600 and 2050, 3.9 s/yr while blending into the parabola).
TEST(DeltaT, NoJumps) {
  const auto end = epoch_of_julian_year(2300.0);
  auto epoch = epoch_of_julian_year(-1000.0);
  auto previous = fetch_delta_time(epoch);
  auto failures = 0;
  while (epoch < end && failures < 10) {
    const auto year = epoch_to_year(epoch);
    const auto max_rate = year < 1600.0 ? 20.0 : (year < 2050.0 ? 2.0 : 5.0);
    epoch += 86400.0;
    const auto current = fetch_delta_time(epoch);
    const auto tolerance = (max_rate * 86400.0 / kSecondsPerJulianYear) + 1e-9;
    if (std::abs(current - previous) > tolerance) {
      ADD_FAILURE() << "Jump of " << current - previous << " s at year "
                    << year;
      ++failures;
    }
    previous = current;
  }
}

// Espenak & Meeus (2006) polynomials, away from any handover. The expected
// values are the formulas of
// https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html, parsed from the
// page and evaluated independently of this implementation.
TEST(DeltaT, EspenakMeeusPolynomials) {
  constexpr std::array<YearValue, 32> kReferences = {{
      {-1000.0, 25427.68},    {-600.0, 18720.48},     {-250.0, 13416.78461},
      {0.0, 10583.6},         {250.0, 8163.063848},   {750.0, 3373.762365},
      {1000.0, 1574.2},       {1250.0, 602.7189981},  {1500.0, 198.3211828},
      {1625.0, 88.096752},    {1650.0, 50.19401599},  {1690.0, 9.89438126},
      {1725.0, 10.88320752},  {1750.0, 13.37007027},  {1790.0, 16.56972969},
      {1810.0, 12.50122},     {1830.0, 7.67338},      {1855.0, 7.250280391},
      {1870.0, 0.9435202816}, {1885.0, -5.649034999}, {1895.0, -6.188384593},
      {1905.0, 3.8346975},    {1915.0, 17.0860575},   {1925.0, 23.78385},
      {1935.0, 23.81735},     {1945.0, 26.87862652},  {1950.0, 29.07},
      {1955.0, 31.04678121},  {1965.0, 35.78814228},  {1970.0, 40.19294086},
      {2200.0, 442.08},       {2500.0, 1459.68},
  }};
  for (const auto& item : kReferences) {
    // The expected values are rounded to 10 significant digits.
    EXPECT_NEAR(fetch_delta_time(epoch_of_julian_year(item.year)), item.delta_t,
                1e-6 + (5e-10 * std::abs(item.delta_t)))
        << "Year: " << item.year;
  }
}

// USNO historic record (historic_deltat.data, as distributed with timescale
// 0.1.3) before the IERS table. The tolerance of each era is the largest
// residual of the published polynomial of that era against this record, plus
// the largest mismatch absorbed by the handovers bounding that era: 0.55 +
// 0.09 s (1860-1900), 0.29 + 0.09 s (1900-1941), 0.17 + 0.03 s (1941-1961),
// 0.07 + 0.07 s (1961-1973).
TEST(DeltaT, HistoricRecord) {
  constexpr std::array<DatedValue, 46> kReferences = {{
      {1860, 1, 1, 7.35},   {1862, 7, 1, 7.18},   {1865, 1, 1, 5.92},
      {1867, 7, 1, 3.52},   {1870, 1, 1, 1.04},   {1872, 7, 1, -1.25},
      {1875, 1, 1, -3.19},  {1877, 7, 1, -4.59},  {1880, 1, 1, -5.36},
      {1882, 7, 1, -5.33},  {1885, 1, 1, -5.74},  {1887, 7, 1, -5.68},
      {1890, 1, 1, -5.86},  {1892, 7, 1, -6.41},  {1895, 1, 1, -6.41},
      {1897, 7, 1, -5.13},  {1900, 1, 1, -2.70},  {1902, 7, 1, 0.62},
      {1905, 1, 1, 3.92},   {1907, 7, 1, 7.00},   {1910, 1, 1, 10.38},
      {1912, 7, 1, 14.00},  {1915, 1, 1, 17.19},  {1917, 7, 1, 19.69},
      {1920, 1, 1, 21.41},  {1922, 7, 1, 22.79},  {1925, 1, 1, 23.63},
      {1927, 7, 1, 24.42},  {1930, 1, 1, 24.02},  {1932, 7, 1, 23.95},
      {1935, 1, 1, 23.91},  {1937, 7, 1, 23.95},  {1940, 1, 1, 24.35},
      {1942, 7, 1, 25.56},  {1945, 1, 1, 26.76},  {1947, 7, 1, 28.03},
      {1950, 1, 1, 29.15},  {1952, 7, 1, 30.19},  {1955, 1, 1, 31.07},
      {1957, 7, 1, 31.923}, {1960, 1, 1, 33.15},  {1962, 7, 1, 34.24},
      {1965, 1, 1, 35.738}, {1967, 7, 1, 37.879}, {1970, 1, 1, 40.182},
      {1972, 7, 1, 42.825},
  }};
  for (const auto& item : kReferences) {
    const auto tolerance = item.year < 1900   ? 0.64
                           : item.year < 1941 ? 0.38
                           : item.year < 1961 ? 0.20
                                              : 0.14;
    EXPECT_NEAR(fetch_delta_time(epoch_of(item)), item.delta_t, tolerance)
        << "Date: " << item.year << "-" << item.month << "-" << item.day;
  }
}

// Independent reference since 1973: merged_deltat.data of timescale 0.1.3
// (USNO monthly values until 2004, IERS Bulletin A daily values after).
//
// The table holds calendar-month means interpolated linearly. Against the
// daily series derived from finals.all over 1973-2026, this representation
// errs by 1.7 ms RMS and 6.8 ms at most: seasonal and intraseasonal UT1
// variations (annual ~20 ms, semi-annual ~8 ms, fortnightly/monthly zonal
// tides ~1 ms) are not fully resolved by monthly nodes. finals.all and
// timescale agree to 0.5 ms. The tolerance is therefore 10 ms.
TEST(DeltaT, AgreesWithTimescaleSince1973) {
  constexpr std::array<DatedValue, 227> kReferences = {{
      {1973, 2, 1, 43.4724},  {1973, 6, 1, 43.8763},  {1973, 10, 1, 44.1982},
      {1974, 2, 1, 44.5646},  {1974, 6, 1, 44.9302},  {1974, 10, 1, 45.2064},
      {1975, 2, 1, 45.5632},  {1975, 6, 1, 45.9133},  {1975, 10, 1, 46.1825},
      {1976, 2, 1, 46.5445},  {1976, 6, 1, 46.9247},  {1976, 10, 1, 47.2362},
      {1977, 2, 1, 47.6049},  {1977, 6, 1, 47.9687},  {1977, 10, 1, 48.2460},
      {1978, 2, 1, 48.6325},  {1978, 6, 1, 49.0319},  {1978, 10, 1, 49.3070},
      {1979, 2, 1, 49.6805},  {1979, 6, 1, 50.0347},  {1979, 10, 1, 50.2968},
      {1980, 2, 1, 50.6160},  {1980, 6, 1, 50.9187},  {1980, 10, 1, 51.1538},
      {1981, 2, 1, 51.4526},  {1981, 6, 1, 51.7573},  {1981, 10, 1, 51.9603},
      {1982, 2, 1, 52.2316},  {1982, 6, 1, 52.5180},  {1982, 10, 1, 52.7340},
      {1983, 2, 1, 53.0445},  {1983, 6, 1, 53.3747},  {1983, 10, 1, 53.5845},
      {1984, 2, 1, 53.8367},  {1984, 6, 1, 54.0536},  {1984, 10, 1, 54.1914},
      {1985, 2, 1, 54.3911},  {1985, 6, 1, 54.5977},  {1985, 10, 1, 54.7174},
      {1986, 2, 1, 54.9161},  {1986, 6, 1, 55.0912},  {1986, 10, 1, 55.1898},
      {1987, 2, 1, 55.3613},  {1987, 6, 1, 55.5524},  {1987, 10, 1, 55.6656},
      {1988, 2, 1, 55.8615},  {1988, 6, 1, 56.0700},  {1988, 10, 1, 56.1611},
      {1989, 2, 1, 56.3399},  {1989, 6, 1, 56.5352},  {1989, 10, 1, 56.6739},
      {1990, 2, 1, 56.9111},  {1990, 6, 1, 57.1738},  {1990, 10, 1, 57.3643},
      {1991, 2, 1, 57.6333},  {1991, 6, 1, 57.9058},  {1991, 10, 1, 58.1043},
      {1992, 2, 1, 58.3833},  {1992, 6, 1, 58.6917},  {1992, 10, 1, 58.8986},
      {1993, 2, 1, 59.2003},  {1993, 6, 1, 59.5242},  {1993, 10, 1, 59.7588},
      {1994, 2, 1, 60.0564},  {1994, 6, 1, 60.3530},  {1994, 10, 1, 60.5578},
      {1995, 2, 1, 60.8664},  {1995, 6, 1, 61.1870},  {1995, 10, 1, 61.4036},
      {1996, 2, 1, 61.6846},  {1996, 6, 1, 61.9497},  {1996, 10, 1, 62.1202},
      {1997, 2, 1, 62.3506},  {1997, 6, 1, 62.6136},  {1997, 10, 1, 62.7926},
      {1998, 2, 1, 63.0217},  {1998, 6, 1, 63.2599},  {1998, 10, 1, 63.3422},
      {1999, 2, 1, 63.4979},  {1999, 6, 1, 63.6444},  {1999, 10, 1, 63.7147},
      {2000, 2, 1, 63.8557},  {2000, 6, 1, 63.9691},  {2000, 10, 1, 64.0093},
      {2001, 2, 1, 64.1068},  {2001, 6, 1, 64.2094},  {2001, 10, 1, 64.2223},
      {2002, 2, 1, 64.3192},  {2002, 6, 1, 64.4151},  {2002, 10, 1, 64.4168},
      {2003, 2, 1, 64.4893},  {2003, 6, 1, 64.5597},  {2003, 10, 1, 64.5415},
      {2004, 2, 1, 64.5891},  {2004, 6, 1, 64.6549},  {2004, 10, 1, 64.6400},
      {2005, 1, 15, 64.6965}, {2005, 3, 17, 64.7488}, {2005, 5, 17, 64.7936},
      {2005, 7, 17, 64.7911}, {2005, 9, 16, 64.7852}, {2005, 11, 16, 64.8217},
      {2006, 1, 16, 64.8499}, {2006, 3, 18, 64.9009}, {2006, 5, 18, 64.9669},
      {2006, 7, 18, 65.0002}, {2006, 9, 17, 65.0251}, {2006, 11, 17, 65.0972},
      {2007, 1, 17, 65.1618}, {2007, 3, 19, 65.2333}, {2007, 5, 19, 65.3152},
      {2007, 7, 19, 65.3469}, {2007, 9, 18, 65.3587}, {2007, 11, 18, 65.4107},
      {2008, 1, 18, 65.4728}, {2008, 3, 19, 65.5309}, {2008, 5, 19, 65.6030},
      {2008, 7, 19, 65.6321}, {2008, 9, 18, 65.6627}, {2008, 11, 18, 65.7279},
      {2009, 1, 18, 65.7954}, {2009, 3, 20, 65.8443}, {2009, 5, 20, 65.9188},
      {2009, 7, 20, 65.9499}, {2009, 9, 19, 65.9760}, {2009, 11, 19, 66.0305},
      {2010, 1, 19, 66.0836}, {2010, 3, 21, 66.1542}, {2010, 5, 21, 66.2250},
      {2010, 7, 21, 66.2380}, {2010, 9, 20, 66.2405}, {2010, 11, 20, 66.2957},
      {2011, 1, 20, 66.3321}, {2011, 3, 22, 66.3847}, {2011, 5, 22, 66.4534},
      {2011, 7, 22, 66.4761}, {2011, 9, 21, 66.4924}, {2011, 11, 21, 66.5602},
      {2012, 1, 21, 66.6240}, {2012, 3, 22, 66.6811}, {2012, 5, 22, 66.7498},
      {2012, 7, 22, 66.7707}, {2012, 9, 21, 66.8001}, {2012, 11, 21, 66.8653},
      {2013, 1, 21, 66.9301}, {2013, 3, 23, 67.0098}, {2013, 5, 23, 67.1009},
      {2013, 7, 23, 67.1292}, {2013, 9, 22, 67.1644}, {2013, 11, 22, 67.2344},
      {2014, 1, 22, 67.3022}, {2014, 3, 24, 67.3761}, {2014, 5, 24, 67.4607},
      {2014, 7, 24, 67.4971}, {2014, 9, 23, 67.5289}, {2014, 11, 23, 67.5975},
      {2015, 1, 23, 67.6663}, {2015, 3, 25, 67.7489}, {2015, 5, 25, 67.8336},
      {2015, 7, 25, 67.8787}, {2015, 9, 24, 67.9413}, {2015, 11, 24, 68.0409},
      {2016, 1, 24, 68.1456}, {2016, 3, 25, 68.2533}, {2016, 5, 25, 68.3593},
      {2016, 7, 25, 68.4060}, {2016, 9, 24, 68.4542}, {2016, 11, 24, 68.5439},
      {2017, 1, 24, 68.6218}, {2017, 3, 26, 68.7023}, {2017, 5, 26, 68.7962},
      {2017, 7, 26, 68.8338}, {2017, 9, 25, 68.8654}, {2017, 11, 25, 68.9283},
      {2018, 1, 25, 68.9806}, {2018, 3, 27, 69.0436}, {2018, 5, 27, 69.1043},
      {2018, 7, 27, 69.1143}, {2018, 9, 26, 69.1312}, {2018, 11, 26, 69.1909},
      {2019, 1, 26, 69.2391}, {2019, 3, 28, 69.3001}, {2019, 5, 28, 69.3518},
      {2019, 7, 28, 69.3464}, {2019, 9, 27, 69.3352}, {2019, 11, 27, 69.3538},
      {2020, 1, 27, 69.3717}, {2020, 3, 28, 69.4080}, {2020, 5, 28, 69.4376},
      {2020, 7, 28, 69.3959}, {2020, 9, 27, 69.3590}, {2020, 11, 27, 69.3632},
      {2021, 1, 27, 69.3520}, {2021, 3, 29, 69.3562}, {2021, 5, 29, 69.3691},
      {2021, 7, 29, 69.3289}, {2021, 9, 28, 69.2914}, {2021, 11, 28, 69.2895},
      {2022, 1, 28, 69.2931}, {2022, 3, 30, 69.2830}, {2022, 5, 30, 69.2817},
      {2022, 7, 30, 69.2232}, {2022, 9, 29, 69.1894}, {2022, 11, 29, 69.2032},
      {2023, 1, 29, 69.1999}, {2023, 3, 31, 69.2084}, {2023, 5, 31, 69.2297},
      {2023, 7, 31, 69.1996}, {2023, 9, 30, 69.1717}, {2023, 11, 30, 69.1730},
      {2024, 1, 30, 69.1785}, {2024, 3, 31, 69.1985}, {2024, 5, 31, 69.2039},
      {2024, 7, 31, 69.1600}, {2024, 9, 30, 69.1250}, {2024, 11, 30, 69.1346},
      {2025, 1, 30, 69.1360}, {2025, 4, 1, 69.1471},  {2025, 6, 1, 69.1550},
      {2025, 8, 1, 69.1219},  {2025, 10, 1, 69.0909}, {2025, 12, 1, 69.1042},
      {2026, 1, 31, 69.1133}, {2026, 4, 2, 69.1340},  {2026, 6, 2, 69.1662},
      {2026, 8, 2, 69.1717},  {2026, 8, 27, 69.1775},
  }};
  for (const auto& item : kReferences) {
    EXPECT_NEAR(fetch_delta_time(epoch_of(item)), item.delta_t, 0.010)
        << "Date: " << item.year << "-" << item.month << "-" << item.day;
  }
}

// 2027-2040: beyond the IERS data, Delta T must stay within a stated margin
// of the linear extrapolation of timescale 0.1.3 (its merged_deltat.data ends
// on 2026-08-27 and is extended with the slope of its last two days).
//
// The margin is the envelope of the historical error of such an
// extrapolation: over 1973-2026, a forecast continuing the last daily slope
// missed the observed Delta T by at most 0.57 s after 1 yr, 3.6 s after 5 yr
// and 10.0 s after 13 yr, all below 0.3 s + 0.8 s/yr x horizon. A plausible
// Delta T lies within that envelope; the former table end jumped 48 s above.
TEST(DeltaT, PlausibleUntil2040) {
  constexpr std::array<DatedValue, 29> kReferences = {{
      {2027, 1, 1, 69.2283}, {2027, 7, 1, 69.3007},   {2028, 1, 1, 69.3743},
      {2028, 7, 1, 69.4471}, {2029, 1, 1, 69.5207},   {2029, 7, 1, 69.5931},
      {2030, 1, 1, 69.6667}, {2030, 7, 1, 69.7391},   {2031, 1, 1, 69.8127},
      {2031, 7, 1, 69.8851}, {2032, 1, 1, 69.9587},   {2032, 7, 1, 70.0315},
      {2033, 1, 1, 70.1051}, {2033, 7, 1, 70.1775},   {2034, 1, 1, 70.2511},
      {2034, 7, 1, 70.3235}, {2035, 1, 1, 70.3971},   {2035, 7, 1, 70.4695},
      {2036, 1, 1, 70.5431}, {2036, 7, 1, 70.6159},   {2037, 1, 1, 70.6895},
      {2037, 7, 1, 70.7619}, {2038, 1, 1, 70.8355},   {2038, 7, 1, 70.9079},
      {2039, 1, 1, 70.9815}, {2039, 7, 1, 71.0539},   {2040, 1, 1, 71.1275},
      {2040, 7, 1, 71.2003}, {2040, 12, 31, 71.2735},
  }};
  const auto origin = epoch_of(2026, 8, 27);
  for (const auto& item : kReferences) {
    const auto epoch = epoch_of(item);
    const auto horizon = (epoch - origin) / kSecondsPerJulianYear;
    EXPECT_NEAR(fetch_delta_time(epoch), item.delta_t, 0.3 + (0.8 * horizon))
        << "Date: " << item.year << "-" << item.month << "-" << item.day;
  }
}

// Far from the IERS data, the Morrison & Stephenson (2004) parabola is used
// as is; between 2050 and 2150 Delta T moves from the extrapolated trend to
// the parabola without overshooting either.
TEST(DeltaT, LongTermModel) {
  for (const auto year : {-2000.0, -1000.0, -502.0, 2151.0, 2200.0, 3000.0}) {
    EXPECT_NEAR(fetch_delta_time(epoch_of_julian_year(year)),
                detail::morrison_stephenson_parabola(year), 1e-9)
        << "Year: " << year;
  }
  const auto last = IERS_TABLE.back();
  for (auto year = 2050.0; year <= 2150.0; year += 10.0) {
    const auto epoch = epoch_of_julian_year(year);
    const auto delta_t = fetch_delta_time(epoch);
    const auto trend =
        last.delta_t +
        (detail::kTableTrend * ((epoch - table_last_epoch()) / 86400.0));
    const auto parabola = detail::morrison_stephenson_parabola(year);
    EXPECT_GE(delta_t, std::min(trend, parabola) - 1e-9) << "Year: " << year;
    EXPECT_LE(delta_t, std::max(trend, parabola) + 1e-9) << "Year: " << year;
  }
}

// Test monotonicity in modern era
TEST(DeltaT, Monotonicity) {
  // Delta T generally increases over time in modern era
  double epoch_2000 = 946684800.0;
  double epoch_2010 = 1262304000.0;
  double epoch_2020 = 1577836800.0;

  double dt_2000 = fetch_delta_time(epoch_2000);
  double dt_2010 = fetch_delta_time(epoch_2010);
  double dt_2020 = fetch_delta_time(epoch_2020);

  EXPECT_LT(dt_2000, dt_2010);
  EXPECT_LT(dt_2010, dt_2020);
}

// Test constexpr evaluation
TEST(DeltaT, ConstexprEvaluation) {
#if defined(__GNUC__) && (__GNUC__ >= 11) || \
    defined(__clang__) && (__cplusplus >= 202303L)
  // FES_MATH_CONSTEXPR expands to constexpr: evaluate at compile time.
  static constexpr double kHistorical = fetch_delta_time(0.0);
  static constexpr double kTable = fetch_delta_time(946684800.0);
  static constexpr double kFuture = fetch_delta_time(2208988800.0);
#else
  static const double kHistorical = fetch_delta_time(0.0);
  static const double kTable = fetch_delta_time(946684800.0);
  static const double kFuture = fetch_delta_time(2208988800.0);
#endif
  // 1970-01-01 (Julian year 1970.0): Espenak & Meeus 1961-1986 polynomial
  EXPECT_NEAR(kHistorical, 40.19294086, 1e-6);
  // 2000-01-01: IERS table
  EXPECT_NEAR(kTable, 63.8285, 0.010);
  // 2040-01-01: extrapolated trend
  EXPECT_GT(kFuture, 60.0);
  EXPECT_LT(kFuture, 80.0);
}

// Verification against external reference (astropy)
// Reference values computed by astropy library (version 6.1+)
// Delta T = TT - UT1 (difference between Terrestrial Time and UT1)
// Generated on 2026-01-27
TEST(DeltaT, AstropyVerification) {
  struct AstropyReference {
    double epoch;      // Unix epoch (seconds since 1970-01-01)
    double delta_t;    // Delta T from astropy (TT - UT1, in seconds)
    const char* date;  // ISO date for reference
  };

  constexpr std::array<AstropyReference, 9> ASTROPY_REFERENCES = {{
      {94694400.0, 43.376148, "1973-01-01T00:00:00"},
      {331300800.0, 50.977314, "1980-07-01T12:00:00"},
      {631152000.0, 56.855214, "1990-01-01T00:00:00"},
      {946684800.0, 63.828541, "2000-01-01T00:00:00"},
      {1276626600.0, 66.241886, "2010-06-15T18:30:00"},
      {1451606399.0, 68.102467, "2015-12-31T23:59:59"},
      {1577836800.0, 69.361161, "2020-01-01T00:00:00"},
      {1686830400.0, 69.228512, "2023-06-15T12:00:00"},
      {1735689600.0, 69.137706, "2025-01-01T00:00:00"},
  }};

  // Test each reference value
  // Note: astropy interpolates the daily IERS series; these values agree with
  // finals.all to 0.6 ms. Within the table the tolerance is the 10 ms derived
  // in AgreesWithTimescaleSince1973; 1973-01-01 precedes the first monthly
  // mean and uses the tolerance of the 1961-1973 polynomial (HistoricRecord).
  for (const auto& ref : ASTROPY_REFERENCES) {
    double computed = fetch_delta_time(ref.epoch);
    double error = std::abs(computed - ref.delta_t);
    double tolerance = ref.epoch < table_first_epoch() ? 0.14 : 0.010;
    EXPECT_LT(error, tolerance)
        << "Date: " << ref.date << ", Expected (astropy): " << ref.delta_t
        << ", Computed: " << computed << ", Error: " << error << " seconds";
  }
}

}  // namespace fes
