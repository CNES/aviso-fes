// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/cartesian.hpp"

#include <gtest/gtest.h>

#include "fes/darwin/constituent.hpp"

namespace fes {

using Cartesian = tidal_model::Cartesian<double>;

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

}  // namespace fes
