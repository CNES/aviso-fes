// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/geometry/ecef.hpp"

#include <gtest/gtest.h>

namespace geometry = fes::geometry;

TEST(EarthCenteredEarthFixed, Interface) {
  auto p = geometry::EarthCenteredEarthFixed(1, 2, 3);
  EXPECT_EQ(1, p.x());
  EXPECT_EQ(2, p.y());
  EXPECT_EQ(3, p.z());

  EXPECT_EQ("ECEF(1 2 3)", std::string(p));

  auto q = p;
  EXPECT_EQ(p, q);

  q.x(4);
  q.y(5);
  q.z(6);

  EXPECT_EQ(4, q.x());
  EXPECT_EQ(5, q.y());
  EXPECT_EQ(6, q.z());

  EXPECT_EQ("ECEF(4 5 6)", std::string(q));
  EXPECT_FALSE(p == q);
}
