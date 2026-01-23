// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/darwin/constituent.hpp"
#include "fes/tidal_model/lgp.hpp"

namespace fes {

TEST(InterpolatorLGP2, Constructor) {
  auto lon = Eigen::VectorXd(19);
  auto lat = Eigen::VectorXd(19);
  auto triangles = Eigen::Matrix<int, -1, 3>(24, 3);
  auto codes = Eigen::Matrix<int, -1, 6>(24, 6);
  auto values = Eigen::VectorXcd(24 * 6);

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

  codes << 0, 1, 2, 3, 4,
      5,                             // 0
      6, 7, 8, 9, 10, 11,            // 1
      12, 13, 14, 15, 16, 17,        // 2
      18, 19, 20, 21, 22, 23,        // 3
      24, 25, 26, 27, 28, 29,        // 4
      30, 31, 32, 33, 34, 35,        // 5
      36, 37, 38, 39, 40, 41,        // 6
      42, 43, 44, 45, 46, 47,        // 7
      48, 49, 50, 51, 52, 53,        // 8
      54, 55, 56, 57, 58, 59,        // 9
      60, 61, 62, 63, 64, 65,        // 10
      66, 67, 68, 69, 70, 71,        // 11
      72, 73, 74, 75, 76, 77,        // 12
      78, 79, 80, 81, 82, 83,        // 13
      84, 85, 86, 87, 88, 89,        // 14
      90, 91, 92, 93, 94, 95,        // 15
      96, 97, 98, 99, 100, 101,      // 16
      102, 103, 104, 105, 106, 107,  // 17
      108, 109, 110, 111, 112, 113,  // 18
      114, 115, 116, 117, 118, 119,  // 19
      120, 121, 122, 123, 124, 125,  // 20
      126, 127, 128, 129, 130, 131,  // 21
      132, 133, 134, 135, 136, 137,  // 22
      138, 139, 140, 141, 142, 143;  // 23

  values.setOnes();

  auto index = std::make_shared<mesh::Index>(lon, lat, triangles);

  tidal_model::LGP2<double, darwin::Constituent> lgp2(std::move(index),
                                                      std::move(codes), kTide);
  lgp2.add_constituent(darwin::kS2, values);
  Quality quality;
  auto acc = std::unique_ptr<Accelerator<darwin::Constituent>>(
      lgp2.accelerator(angle::Formulae::kMeeus, 0.0));
  lgp2.interpolate({0.0, 0.0}, quality, acc.get());
  lgp2.interpolate({0.0, 0.0}, quality, acc.get());
}

}  // namespace fes
