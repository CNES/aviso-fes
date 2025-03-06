// Copyright (c) 2025 CNES

//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/geometry/triangle.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace geometry = fes::geometry;

TEST(Triangle, Interface) {
  auto v1 = geometry::Point(2, 0);
  auto v2 = geometry::Point(1, 1);
  auto v3 = geometry::Point(0, 0);
  auto triangle = geometry::Triangle(v1, v2, v3);
  EXPECT_TRUE(triangle.is_valid());
  EXPECT_EQ(v1, triangle.v1());
  EXPECT_EQ(v2, triangle.v2());
  EXPECT_EQ(v3, triangle.v3());
  EXPECT_EQ("POLYGON((2 0,1 1,0 0,2 0))", static_cast<std::string>(triangle));

  EXPECT_TRUE(triangle.covered_by(v1));
  EXPECT_TRUE(triangle.covered_by(v2));
  EXPECT_TRUE(triangle.covered_by(v3));
  EXPECT_TRUE(triangle.covered_by(geometry::Point(0.5, 0.5)));
  EXPECT_FALSE(triangle.covered_by(geometry::Point(0, 1)));
}

TEST(Triangle, ReferenceRightAngled) {
  double x = NAN;
  double y = NAN;
  auto v1 = geometry::Point(2, 0);
  auto v2 = geometry::Point(1, 1);
  auto v3 = geometry::Point(0, 0);
  auto triangle = geometry::Triangle(v1, v2, v3);
  std::tie(x, y) = triangle.reference_right_angled({0.5, 0.5});
  EXPECT_DOUBLE_EQ(0.5, x);
  EXPECT_DOUBLE_EQ(0.5, y);

  v1 = geometry::Point(0, 0);
  v2 = geometry::Point(1, 0);
  v3 = geometry::Point(0, 1);
  triangle = geometry::Triangle(v1, v2, v3);
  std::tie(x, y) = triangle.reference_right_angled({0.5, 0.5});
  EXPECT_DOUBLE_EQ(0.5, x);
  EXPECT_DOUBLE_EQ(0.5, y);

  v1 = geometry::Point(0.004, 0.004);
  v2 = geometry::Point(-0.273, 0.004);
  v3 = geometry::Point(-0.11, -0.192);
  triangle = geometry::Triangle(v1, v2, v3);
  std::tie(x, y) = triangle.reference_right_angled({0.001, 0.001});
  EXPECT_NEAR(0.00453105, x, 1e-6);
  EXPECT_NEAR(0.0153061, y, 1e-6);

  std::tie(x, y) = triangle.reference_right_angled({-0.11, -0.192});
  EXPECT_NEAR(0.0, x, 1e-6);
  EXPECT_NEAR(1.0, y, 1e-6);

  std::tie(x, y) = triangle.reference_right_angled({0.004, 0.004});
  EXPECT_NEAR(0.0, x, 1e-6);
  EXPECT_NEAR(0.0, y, 1e-6);

  std::tie(x, y) = triangle.reference_right_angled({-0.273, 0.004});
  EXPECT_NEAR(1.0, x, 1e-6);
  EXPECT_NEAR(0.0, y, 1e-6);
}

TEST(Triangle, ReferenceRightAngledDegradedCase) {
  double x = NAN;
  double y = NAN;
  auto v1 =
      geometry::Point(-122.3500060000000075888238, 38.00500499999999703959475);
  auto v2 =
      geometry::Point(-122.4700000000000272848411, 38.11000400000000354339136);
  auto v3 =
      geometry::Point(-122.4700000000000272848411, 37.97500200000000347699824);
  auto triangle = geometry::Triangle(v1, v2, v3);
  auto pt = triangle.project({-122.5, 38.0});
  std::tie(x, y) = triangle.reference_right_angled(pt);
  EXPECT_NEAR(x, 0.18519769401644656, 1e-6);
  EXPECT_NEAR(y, 0.81480230598367187, 1e-6);
}
