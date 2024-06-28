// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/tidal_model/lgp.hpp"

TEST(InterpolatorLGP1, Constructor) {
  auto lon = Eigen::VectorXd(19);
  auto lat = Eigen::VectorXd(19);
  auto triangles = Eigen::Matrix<int, -1, 3>(24, 3);
  auto codes = Eigen::Matrix<int, -1, 3>(24, 3);
  auto values = Eigen::VectorXcd(24 * 3);

  lon << 0.004, -0.175, -0.273, -0.11, 0.183, 0.256, 0.183, -0.428, -0.501,
      -0.371, 0.46, 0.622, 0.451, 0.313, -0.021, -0.289, -0.175, 0.077, 0.321;

  lat << 0.004, 0.264, 0.004, -0.192, -0.232, 0.012, 0.264, 0.256, -0.013,
      -0.24, -0.281, -0.004, 0.289, 0.459, 0.443, 0.435, -0.354, -0.363, -0.428;

  triangles << 0, 2, 3,  // 0
      0, 3, 4,           // 1
      0, 4, 5,           // 2
      0, 5, 6,           // 3
      0, 6, 1,           // 4
      0, 1, 2,           // 5
      1, 6, 14,          // 6
      1, 14, 15,         // 7
      1, 15, 7,          // 8
      1, 7, 2,           // 9
      2, 7, 8,           // 10
      2, 8, 9,           // 11
      2, 9, 3,           // 12
      3, 9, 16,          // 13
      3, 16, 17,         // 14
      3, 17, 4,          // 15
      4, 17, 18,         // 16
      4, 18, 10,         // 17
      4, 10, 5,          // 18
      5, 10, 11,         // 19
      5, 11, 12,         // 20
      5, 12, 6,          // 21
      6, 12, 13,         // 22
      6, 13, 14;         // 23

  codes << 0, 1, 2,  // 0
      3, 4, 5,       // 1
      6, 7, 8,       // 2
      9, 10, 11,     // 3
      12, 13, 14,    // 4
      15, 16, 17,    // 5
      18, 19, 20,    // 6
      21, 22, 23,    // 7
      24, 25, 26,    // 8
      27, 28, 29,    // 9
      30, 31, 32,    // 10
      33, 34, 35,    // 11
      36, 37, 38,    // 12
      39, 40, 41,    // 13
      42, 43, 44,    // 14
      45, 46, 47,    // 15
      48, 49, 50,    // 16
      51, 52, 53,    // 17
      54, 55, 56,    // 18
      57, 58, 59,    // 19
      60, 61, 62,    // 20
      63, 64, 65,    // 21
      66, 67, 68,    // 22
      69, 70, 71;    // 23

  values.setOnes();

  auto index = std::make_shared<fes::mesh::Index>(lon, lat, triangles);

  fes::tidal_model::LGP1<double> lgp1(std::move(index), std::move(codes),
                                      fes::kTide);
  lgp1.add_constituent(fes::kS2, values);
  auto acc = std::unique_ptr<fes::Accelerator>(
      lgp1.accelerator(fes::angle::Formulae::kMeeus, 0.0));
  fes::Quality quality;
  auto x = lgp1.interpolate({0.0, 0.0}, quality, acc.get());

  auto state = lgp1.getstate();
  auto other = fes::tidal_model::LGP1<double>::setstate(
      fes::string_view(state.data(), state.size()));
  auto y = other.interpolate({0.0, 0.0}, quality, acc.get());
  EXPECT_EQ(x, y);
}
