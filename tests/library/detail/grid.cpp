// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/detail/grid.hpp"

#include <gtest/gtest.h>

#include <Eigen/Core>

namespace fes {
namespace detail {

TEST(Grid, Interface) {
  auto points = Eigen::VectorXd(8);
  points << 0, 1, 2, 3, 10, 11, 12, 13;

  auto grid = Grid<double>(points.data(), 2, 4);
  EXPECT_EQ(grid.nx(), 2);
  EXPECT_EQ(grid.ny(), 4);
  EXPECT_EQ(grid.size(), 8);
  EXPECT_EQ(grid(0, 0), 0);
  EXPECT_EQ(grid(0, 1), 1);
  EXPECT_EQ(grid(0, 2), 2);
  EXPECT_EQ(grid(0, 3), 3);
  EXPECT_EQ(grid(1, 0), 10);
  EXPECT_EQ(grid(1, 1), 11);
  EXPECT_EQ(grid(1, 2), 12);
  EXPECT_EQ(grid(1, 3), 13);

  points << 0, 10, 1, 11, 2, 12, 3, 13;
  grid = Grid<double>(points.data(), 2, 4, false);
  EXPECT_EQ(grid.nx(), 2);
  EXPECT_EQ(grid.ny(), 4);
  EXPECT_EQ(grid.size(), 8);
  EXPECT_EQ(grid(0, 0), 0);
  EXPECT_EQ(grid(0, 1), 1);
  EXPECT_EQ(grid(0, 2), 2);
  EXPECT_EQ(grid(0, 3), 3);
  EXPECT_EQ(grid(1, 0), 10);
  EXPECT_EQ(grid(1, 1), 11);
  EXPECT_EQ(grid(1, 2), 12);
  EXPECT_EQ(grid(1, 3), 13);
}

}  // namespace detail
}  // namespace fes
