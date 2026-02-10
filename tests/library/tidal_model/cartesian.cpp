// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/cartesian.hpp"

#include <gtest/gtest.h>

#include <complex>

#include "fes/constituent.hpp"

namespace fes {

using Cartesian = tidal_model::Cartesian<double>;

namespace {

constexpr double kTolerance = 1e-12;

auto expected_value(const double lon, const double lat)
    -> std::complex<double> {
  return {lon + 2.0 * lat, lon - 2.0 * lat};
}

auto make_wave(const Axis& lon, const Axis& lat)
    -> Vector<std::complex<double>> {
  auto wave = Vector<std::complex<double>>(lon.size() * lat.size());
  for (int64_t i = 0; i < lon.size(); ++i) {
    for (int64_t j = 0; j < lat.size(); ++j) {
      const auto idx = static_cast<Eigen::Index>(i * lat.size() + j);
      wave(idx) = expected_value(lon(i), lat(j));
    }
  }
  return wave;
}

}  // namespace

TEST(TidalModelCartesian, Constructor) {
  auto points = Eigen::VectorXd(5);
  points << 0, 1, 2, 3, 4;
  auto matrix = Eigen::VectorXcd::Zero(25);
  auto axis = Axis(points);
  auto model = Cartesian(axis, axis, kTide);
  model.add_constituent(kM2, matrix);
  model.add_constituent(kK2, matrix);

  EXPECT_EQ(model.lon().size(), 5);
  EXPECT_EQ(model.lat().size(), 5);
  EXPECT_EQ(model.tide_type(), kTide);
}

TEST(TidalModelCartesian, GetSetState) {
  auto points = Eigen::VectorXd(5);
  points << 0, 1, 2, 3, 4;
  auto matrix = Eigen::VectorXcd::Zero(25);
  auto axis = Axis(points);
  auto model = Cartesian(axis, axis, kTide);
  model.add_constituent(kM2, matrix);
  model.add_constituent(kK2, matrix);
}

TEST(TidalModelCartesian, Interpolate) {
  auto lon = Axis(0.0, 2.0, 1.0);
  auto lat = Axis(0.0, 2.0, 1.0);
  auto model = Cartesian(lon, lat, kTide);
  model.add_constituent(kM2, make_wave(lon, lat));

  auto acc = std::unique_ptr<Accelerator>(
      model.accelerator(angle::Formulae::kMeeus, 0.0));
  Quality quality = 0;
  const auto& values = model.interpolate({0.5, 0.5}, quality, *acc);
  ASSERT_EQ(values.size(), 1);
  EXPECT_EQ(values.front().first, kM2);

  const auto expected = expected_value(0.5, 0.5);
  EXPECT_NEAR(values.front().second.real(), expected.real(), kTolerance);
  EXPECT_NEAR(values.front().second.imag(), expected.imag(), kTolerance);
  EXPECT_EQ(quality, 4);
}

TEST(TidalModelCartesian, ResampleMorePoints) {
  auto lon = Axis(0.0, 2.0, 1.0);
  auto lat = Axis(0.0, 2.0, 1.0);
  auto model = Cartesian(lon, lat, kTide);

  auto new_lon = Axis(0.0, 2.0, 0.5);
  auto new_lat = Axis(0.0, 2.0, 0.5);
  auto wave = make_wave(new_lon, new_lat);
  auto resampled = model.resample(new_lon, new_lat, wave, true, 1);
  ASSERT_EQ(resampled.size(), lon.size() * lat.size());

  for (int64_t i = 0; i < lon.size(); ++i) {
    for (int64_t j = 0; j < lat.size(); ++j) {
      const auto idx = static_cast<Eigen::Index>(i * lat.size() + j);
      const auto expected = expected_value(lon(i), lat(j));
      EXPECT_NEAR(resampled(idx).real(), expected.real(), kTolerance);
      EXPECT_NEAR(resampled(idx).imag(), expected.imag(), kTolerance);
    }
  }
}

TEST(TidalModelCartesian, ResampleLessPoints) {
  auto lon = Axis(0.0, 2.0, 1.0);
  auto lat = Axis(0.0, 2.0, 1.0);
  auto model = Cartesian(lon, lat, kTide);

  auto new_lon = Axis(0.0, 2.0, 2.0);
  auto new_lat = Axis(0.0, 2.0, 2.0);
  auto wave = make_wave(new_lon, new_lat);
  auto resampled = model.resample(new_lon, new_lat, wave, true, 1);
  ASSERT_EQ(resampled.size(), lon.size() * lat.size());

  for (int64_t i = 0; i < lon.size(); ++i) {
    for (int64_t j = 0; j < lat.size(); ++j) {
      const auto idx = static_cast<Eigen::Index>(i * lat.size() + j);
      const auto expected = expected_value(lon(i), lat(j));
      EXPECT_NEAR(resampled(idx).real(), expected.real(), kTolerance);
      EXPECT_NEAR(resampled(idx).imag(), expected.imag(), kTolerance);
    }
  }
}

}  // namespace fes
