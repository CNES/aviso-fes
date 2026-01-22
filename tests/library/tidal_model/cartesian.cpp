// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tidal_model/cartesian.hpp"

#include <gtest/gtest.h>

#include "fes/constituent.hpp"

using Cartesian = fes::tidal_model::Cartesian<double, fes::Constituent>;

TEST(TidalModelCartesian, Constructor) {
  auto points = Eigen::VectorXd(5);
  points << 0, 1, 2, 3, 4;
  auto matrix = Eigen::VectorXcd::Zero(25);
  auto axis = fes::Axis(points);
  auto model = Cartesian(axis, axis, fes::kTide);
  model.add_constituent(fes::kM2, matrix);
  model.add_constituent(fes::kK2, matrix);

  EXPECT_EQ(model.lon().size(), 5);
  EXPECT_EQ(model.lat().size(), 5);
  EXPECT_EQ(model.tide_type(), fes::kTide);

  auto state = model.getstate();
  auto other =
      Cartesian::setstate(fes::string_view(state.data(), state.size()));

  EXPECT_THROW(Cartesian::setstate("invalid"), std::invalid_argument);
}

TEST(TidalModelCartesian, GetSetState) {
  auto points = Eigen::VectorXd(5);
  points << 0, 1, 2, 3, 4;
  auto matrix = Eigen::VectorXcd::Zero(25);
  auto axis = fes::Axis(points);
  auto model = Cartesian(axis, axis, fes::kTide);
  model.add_constituent(fes::kM2, matrix);
  model.add_constituent(fes::kK2, matrix);

  auto state = model.getstate();
  auto other =
      Cartesian::setstate(fes::string_view(state.data(), state.size()));

  EXPECT_EQ(model.lon().size(), other.lon().size());
  EXPECT_EQ(model.lat().size(), other.lat().size());
  EXPECT_EQ(model.tide_type(), other.tide_type());
  const auto& model_data = model.data();
  const auto& other_data = other.data();
  EXPECT_EQ(model_data.size(), other_data.size());
  EXPECT_EQ(model_data.at(fes::kM2).size(), other_data.at(fes::kM2).size());
  EXPECT_EQ(model_data.at(fes::kK2).size(), other_data.at(fes::kK2).size());
  EXPECT_EQ(model_data.at(fes::kM2)(0), other_data.at(fes::kM2)(0));
  EXPECT_EQ(model_data.at(fes::kK2)(0), other_data.at(fes::kK2)(0));
  EXPECT_EQ(model_data.at(fes::kM2)(1), other_data.at(fes::kM2)(1));
  EXPECT_EQ(model_data.at(fes::kK2)(1), other_data.at(fes::kK2)(1));
  EXPECT_EQ(model_data.at(fes::kM2)(2), other_data.at(fes::kM2)(2));
  EXPECT_EQ(model_data.at(fes::kK2)(2), other_data.at(fes::kK2)(2));
  EXPECT_EQ(model_data.at(fes::kM2)(3), other_data.at(fes::kM2)(3));
  EXPECT_EQ(model_data.at(fes::kK2)(3), other_data.at(fes::kK2)(3));
  EXPECT_EQ(model_data.at(fes::kM2)(4), other_data.at(fes::kM2)(4));
  EXPECT_EQ(model_data.at(fes::kK2)(4), other_data.at(fes::kK2)(4));
}
