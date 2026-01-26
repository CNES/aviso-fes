// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/mesh/index.hpp"

#include <gtest/gtest.h>

namespace fes {
namespace mesh {

static auto make_data()
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Eigen::Matrix<int, -1, 3>> {
  auto lon = Eigen::VectorXd(19);
  auto lat = Eigen::VectorXd(19);
  auto triangles = Eigen::Matrix<int, -1, 3>(24, 3);

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

  return std::make_tuple(lon, lat, triangles);
}

TEST(Index, Search) {
  auto lon = Eigen::VectorXd();
  auto lat = Eigen::VectorXd();
  auto triangles = Eigen::Matrix<int, -1, 3>();
  std::tie(lon, lat, triangles) = make_data();

  auto index = Index(lon, lat, triangles);

  auto query =
      index.search({-0.16067459068705148, 0.09857747238454806}, 50'000);
  EXPECT_TRUE(query.is_inside());
  EXPECT_EQ(query.index, 5);

  query = index.search({-0.4057, 0.0717}, 50'000);
  EXPECT_TRUE(query.is_inside());
  EXPECT_EQ(query.index, 10);

  query = index.search({0.2562, 0.0101}, 50'000);
  EXPECT_TRUE(query.is_inside());
  EXPECT_EQ(query.index, 18);

  // This point is outside the convex hull of the mesh
  query = index.search({0.5741, -0.1029}, 50'000);
  EXPECT_FALSE(query.is_inside());
  EXPECT_EQ(query.index, -1);

  // This point is outside the convex hull of the mesh, and too far away to be
  // found.
  query = index.search({1, 1}, 50'000);
  EXPECT_FALSE(query.is_inside());
  EXPECT_EQ(query.index, -1);
}

}  // namespace mesh
}  // namespace fes
