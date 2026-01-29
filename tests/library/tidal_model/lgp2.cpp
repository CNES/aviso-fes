// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/darwin/constituent.hpp"
#include "fes/tidal_model/lgp.hpp"

namespace fes {

TEST(InterpolatorLGP2, Constructor) {
  auto lon = Eigen::Vector<double, 19>();
  auto lat = Eigen::Vector<double, 19>();
  auto triangles = Eigen::Matrix<int, 24, 3>();
  auto codes = Eigen::Matrix<int, 24, 6>();
  auto values = Eigen::Matrix<std::complex<double>, 24 * 6, 1>();

  lon = {0.004,  -0.175, -0.273, -0.11, 0.183, 0.256, 0.183,
         -0.428, -0.501, -0.371, 0.46,  0.622, 0.451, 0.313,
         -0.021, -0.289, -0.175, 0.077, 0.321};

  lat = {0.004, 0.264,  0.004,  -0.192, -0.232, 0.012, 0.264,
         0.256, -0.013, -0.24,  -0.281, -0.004, 0.289, 0.459,
         0.443, 0.435,  -0.354, -0.363, -0.428};
  triangles.row(0) << 0, 2, 3;
  triangles.row(1) << 0, 3, 4;
  triangles.row(2) << 0, 4, 5;
  triangles.row(3) << 0, 5, 6;
  triangles.row(4) << 0, 6, 1;
  triangles.row(5) << 0, 1, 2;
  triangles.row(6) << 1, 6, 14;
  triangles.row(7) << 1, 14, 15;
  triangles.row(8) << 1, 15, 7;
  triangles.row(9) << 1, 7, 2;
  triangles.row(10) << 2, 7, 8;
  triangles.row(11) << 2, 8, 9;
  triangles.row(12) << 2, 9, 3;
  triangles.row(13) << 3, 9, 16;
  triangles.row(14) << 3, 16, 17;
  triangles.row(15) << 3, 17, 4;
  triangles.row(16) << 4, 17, 18;
  triangles.row(17) << 4, 18, 10;
  triangles.row(18) << 4, 10, 5;
  triangles.row(19) << 5, 10, 11;
  triangles.row(20) << 5, 11, 12;
  triangles.row(21) << 5, 12, 6;
  triangles.row(22) << 6, 12, 13;
  triangles.row(23) << 6, 13, 14;

  codes.row(0) << 0, 1, 2, 3, 4, 5;
  codes.row(1) << 6, 7, 8, 9, 10, 11;
  codes.row(2) << 12, 13, 14, 15, 16, 17;
  codes.row(3) << 18, 19, 20, 21, 22, 23;
  codes.row(4) << 24, 25, 26, 27, 28, 29;
  codes.row(5) << 30, 31, 32, 33, 34, 35;
  codes.row(6) << 36, 37, 38, 39, 40, 41;
  codes.row(7) << 42, 43, 44, 45, 46, 47;
  codes.row(8) << 48, 49, 50, 51, 52, 53;
  codes.row(9) << 54, 55, 56, 57, 58, 59;
  codes.row(10) << 60, 61, 62, 63, 64, 65;
  codes.row(11) << 66, 67, 68, 69, 70, 71;
  codes.row(12) << 72, 73, 74, 75, 76, 77;
  codes.row(13) << 78, 79, 80, 81, 82, 83;
  codes.row(14) << 84, 85, 86, 87, 88, 89;
  codes.row(15) << 90, 91, 92, 93, 94, 95;
  codes.row(16) << 96, 97, 98, 99, 100, 101;
  codes.row(17) << 102, 103, 104, 105, 106, 107;
  codes.row(18) << 108, 109, 110, 111, 112, 113;
  codes.row(19) << 114, 115, 116, 117, 118, 119;
  codes.row(20) << 120, 121, 122, 123, 124, 125;
  codes.row(21) << 126, 127, 128, 129, 130, 131;
  codes.row(22) << 132, 133, 134, 135, 136, 137;
  codes.row(23) << 138, 139, 140, 141, 142, 143;

  values.setOnes();

  auto index = std::make_shared<mesh::Index>(lon, lat, triangles);

  tidal_model::LGP2<double> lgp2(std::move(index), std::move(codes),
                                 darwin::constituents::map(), kTide);
  lgp2.add_constituent(darwin::kS2, values);
  Quality quality = 0;
  auto acc = std::unique_ptr<Accelerator>(
      lgp2.accelerator(angle::Formulae::kMeeus, 0.0));
  lgp2.interpolate({0.0, 0.0}, quality, acc.get());
  lgp2.interpolate({0.0, 0.0}, quality, acc.get());
}

}  // namespace fes
