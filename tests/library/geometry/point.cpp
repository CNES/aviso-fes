// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/geometry/point.hpp"

#include <gtest/gtest.h>

#include "fes/geometry/ecef.hpp"

namespace geometry = fes::geometry;

TEST(Point, Constructor) {
  auto p = geometry::Point(1, 2);
  EXPECT_EQ(1, p.lon());
  EXPECT_EQ(2, p.lat());

  EXPECT_EQ("POINT(1 2)", std::string(p));
}

TEST(Point, Geographic2Ecef) {
  auto longlat = geometry::Point(0, 0);
  auto geocent = static_cast<geometry::EarthCenteredEarthFixed>(longlat);
  EXPECT_EQ(6378137.0, geocent.x());
  EXPECT_EQ(0.0, geocent.y());
  EXPECT_EQ(0.0, geocent.z());

  longlat = geometry::Point(78.042068, 27.173891);
  geocent = static_cast<geometry::EarthCenteredEarthFixed>(longlat);
  EXPECT_NEAR(1176467.80296241, geocent.x(), 1e-5);
  EXPECT_NEAR(5554897.69179168, geocent.y(), 1e-5);
  EXPECT_NEAR(2895370.16579709, geocent.z(), 1e-4);
}
