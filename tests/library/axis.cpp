// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/axis.hpp"

#include <gtest/gtest.h>

namespace fes {

TEST(Axis, Constructor) {
  auto points =
      static_cast<Eigen::VectorXd>(Eigen::VectorXd::LinSpaced(360, 0.0, 359.0));
  auto axis = Axis(points);
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), 0);
  EXPECT_EQ(axis.end(), 359.0);
  EXPECT_EQ(axis.min_value(), 0);
  EXPECT_EQ(axis.max_value(), 359.0);
  EXPECT_EQ(axis.is_ascending(), true);
  EXPECT_EQ(axis.is_circular(), false);
  EXPECT_EQ(axis(0), 0);
  EXPECT_EQ(axis(1), 1.0);
  EXPECT_EQ(axis(359), 359.0);
  EXPECT_EQ(axis.find_index(180), 180);
  EXPECT_EQ(axis.find_index(360, false), -1);
  EXPECT_EQ(axis.find_index(360, true), 359);
  EXPECT_THROW({ axis(-1); }, std::out_of_range);
  EXPECT_THROW({ axis(360); }, std::out_of_range);

  points = Eigen::VectorXd::LinSpaced(360, 359.0, 0.0);
  axis = Axis(points);
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), 359.0);
  EXPECT_EQ(axis.end(), 0);
  EXPECT_EQ(axis.min_value(), 0);
  EXPECT_EQ(axis.max_value(), 359.0);
  EXPECT_EQ(axis.is_ascending(), false);
  EXPECT_EQ(axis.is_circular(), false);
  EXPECT_EQ(axis(0), 359.0);
  EXPECT_EQ(axis(1), 358.0);
  EXPECT_EQ(axis(359), 0);
  EXPECT_EQ(axis.find_index(180), 179);
  EXPECT_EQ(axis.find_index(360, false), -1);
  EXPECT_EQ(axis.find_index(360, true), 0);
  EXPECT_THROW({ axis(-1); }, std::out_of_range);
  EXPECT_THROW({ axis(360); }, std::out_of_range);

  points = Eigen::VectorXd(10);
  points << 0, 3, 12, 15, 18, 21, 24, 27, 30, 33;
  EXPECT_THROW({ Axis{points}; }, std::invalid_argument);

  points = Eigen::VectorXd(1);
  points << 0;
  EXPECT_THROW({ Axis{points}; }, std::invalid_argument);

  points = Eigen::VectorXd();
  EXPECT_THROW({ Axis{points}; }, std::invalid_argument);
}

TEST(Axis, WrapLongitude) {
  auto points =
      static_cast<Eigen::VectorXd>(Eigen::VectorXd::LinSpaced(360, 0.0, 359.0));
  auto axis = Axis(points, 1e-6, true);
  EXPECT_EQ(axis, Axis(points, 1e-6, true));
  EXPECT_TRUE(axis.is_circular());
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), 0);
  EXPECT_EQ(axis.end(), 359);
  EXPECT_EQ(axis.min_value(), 0);
  EXPECT_EQ(axis.max_value(), 359);
  EXPECT_TRUE(axis.is_ascending());
  EXPECT_EQ(axis(0), 0.0);
  EXPECT_EQ(axis(180), 180.0);
  EXPECT_THROW({ axis(759); }, std::out_of_range);
  auto i1 = axis.find_index(0.0, false);
  EXPECT_EQ(i1, 0);
  auto i2 = axis.find_index(360.0, false);
  EXPECT_EQ(i2, 0);
  auto indexes = axis.find_indices(360.0);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(0, 1));
  indexes = axis.find_indices(370.0);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(10, 11));
  indexes = axis.find_indices(-9.5);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(350, 351));

  points = Eigen::VectorXd::LinSpaced(360, 359.0, 0.0);
  axis = Axis(points, 1e-6, true);
  EXPECT_EQ(axis, Axis(points, 1e-6, true));
  EXPECT_TRUE(axis.is_circular());
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), 359);
  EXPECT_EQ(axis.end(), 0);
  EXPECT_EQ(axis.min_value(), 0);
  EXPECT_EQ(axis.max_value(), 359);
  EXPECT_FALSE(axis.is_ascending());
  EXPECT_EQ(axis(1), 358);
  EXPECT_EQ(axis(180), 179);
  EXPECT_THROW({ axis(759); }, std::out_of_range);
  i1 = axis.find_index(0.0, false);
  EXPECT_EQ(i1, 359);
  i2 = axis.find_index(360.0, false);
  EXPECT_EQ(i2, 359);
  i2 = axis.find_index(359.0, false);
  EXPECT_EQ(i2, 0);
  indexes = axis.find_indices(359.5);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(370.0);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(349, 350));
  indexes = axis.find_indices(-9.5);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(9, 8));

  points = Eigen::VectorXd::LinSpaced(360, -180, 179);
  axis = Axis(points, 1e-6, true);
  EXPECT_TRUE(axis.is_circular());
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), -180);
  EXPECT_EQ(axis.end(), 179);
  EXPECT_EQ(axis.min_value(), -180);
  EXPECT_EQ(axis.max_value(), 179);
  EXPECT_TRUE(axis.is_ascending());
  EXPECT_EQ(axis(0), -180.0);
  indexes = axis.find_indices(370.0);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(190, 191));
  EXPECT_EQ(axis(189), 9);

  points = Eigen::VectorXd::LinSpaced(360, 180, -179);
  axis = Axis(points, 1e-6, true);
  EXPECT_TRUE(axis.is_circular());
  EXPECT_EQ(axis.size(), 360);
  EXPECT_EQ(axis.start(), 180);
  EXPECT_EQ(axis.end(), -179);
  EXPECT_EQ(axis.min_value(), -179);
  EXPECT_EQ(axis.max_value(), 180);
  EXPECT_EQ(axis(0), 180.0);
  EXPECT_EQ(axis(180), 0);
}

TEST(Axis, FindIndices) {
  auto points =
      static_cast<Eigen::VectorXd>(Eigen::VectorXd::LinSpaced(360, 0.0, 359.0));
  auto axis = Axis(points, 1e-6, true);
  auto indexes = axis.find_indices(359.4);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(359.6);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(-0.1);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(359.9);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(0.1);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(0, 1));
  indexes = axis.find_indices(358.9);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(358, 359));

  points = Eigen::VectorXd::LinSpaced(360, 359.0, 0.0);
  axis = Axis(points, 1e-6, true);
  indexes = axis.find_indices(359.4);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(0, 359));
  indexes = axis.find_indices(359.6);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(-0.1);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(359.9);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 0));
  indexes = axis.find_indices(0.1);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(359, 358));
  indexes = axis.find_indices(358.9);
  ASSERT_TRUE(indexes.has_value());
  EXPECT_EQ(*indexes, std::make_tuple(1, 0));
}

}  // namespace fes
