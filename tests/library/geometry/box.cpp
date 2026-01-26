// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/geometry/box.hpp"

#include <gtest/gtest.h>

#include <boost/geometry.hpp>
#include <sstream>

#include "fes/geometry/triangle.hpp"

namespace fes {
namespace geometry {

// Test fixture for Box tests
class BoxTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Standard test points
    p1 = Point{10.0, 20.0};    // Standard point
    p2 = Point{30.0, 40.0};    // Standard point
    p3 = Point{-10.0, -20.0};  // Negative coordinates
    p4 = Point{-30.0, -40.0};  // Negative coordinates

    // Dateline crossing points
    p_dateline1 = Point{170.0, 10.0};   // West of dateline
    p_dateline2 = Point{-170.0, 30.0};  // East of dateline

    // Edge case points
    p_north_pole = Point{-0.1, 90.0};
    p_south_pole = Point{0.1, -90.0};
    p_equator = Point{0.0, 0.0};
    p_prime_meridian = Point{0.0, 45.0};

    // Test triangles for intersection testing
    triangle_inside =
        Triangle{Point{15.0, 25.0}, Point{20.0, 25.0}, Point{17.5, 30.0}};

    triangle_outside =
        Triangle{Point{50.0, 50.0}, Point{60.0, 50.0}, Point{55.0, 60.0}};

    triangle_intersecting =
        Triangle{Point{5.0, 15.0}, Point{35.0, 15.0}, Point{20.0, 45.0}};
  }

  Point p1, p2, p3, p4;
  Point p_dateline1, p_dateline2;
  Point p_north_pole, p_south_pole, p_equator, p_prime_meridian;
  Triangle triangle_inside, triangle_outside, triangle_intersecting;
};

// Test default constructor
TEST_F(BoxTest, DefaultConstructor) {
  Box box;
  // Default box should be valid but empty
  EXPECT_NO_THROW(box.min_corner());
  EXPECT_NO_THROW(box.max_corner());
}

// Test parameterized constructor with normal coordinates
TEST_F(BoxTest, ParameterizedConstructorNormal) {
  Box box{p1, p2};

  EXPECT_DOUBLE_EQ(box.min_corner().lon(), 10.0);
  EXPECT_DOUBLE_EQ(box.min_corner().lat(), 20.0);
  EXPECT_DOUBLE_EQ(box.max_corner().lon(), 30.0);
  EXPECT_DOUBLE_EQ(box.max_corner().lat(), 40.0);
}

// Test constructor with dateline crossing
TEST_F(BoxTest, ParameterizedConstructorDatelineCrossing) {
  Box box{p_dateline1, p_dateline2};  // 170° to -170°

  // Should normalize angles and detect dateline crossing
  EXPECT_DOUBLE_EQ(box.min_corner().lon(), 170.0);
  EXPECT_DOUBLE_EQ(box.min_corner().lat(), 10.0);
  EXPECT_DOUBLE_EQ(box.max_corner().lon(), 190.0);
  EXPECT_DOUBLE_EQ(box.max_corner().lat(), 30.0);
}

// Test string conversion
TEST_F(BoxTest, StringConversion) {
  Box box{p1, p2};

  std::string str = static_cast<std::string>(box);
  EXPECT_FALSE(str.empty());

  // Should contain WKT format
  EXPECT_NE(str.find("POLYGON"), std::string::npos);
}

// Test stream output
TEST_F(BoxTest, StreamOutput) {
  Box box{p1, p2};

  std::ostringstream oss;
  oss << box;

  std::string output = oss.str();
  EXPECT_FALSE(output.empty());
  EXPECT_NE(output.find("POLYGON"), std::string::npos);
}

// Test intersection with triangle - triangle inside box
TEST_F(BoxTest, IntersectsTriangleInside) {
  Box box{p1, p2};

  EXPECT_TRUE(box.intersects(triangle_inside));
}

// Test intersection with triangle - triangle outside box
TEST_F(BoxTest, IntersectsTriangleOutside) {
  Box box{p1, p2};

  EXPECT_FALSE(box.intersects(triangle_outside));
}

// Test intersection with triangle - triangle intersecting box
TEST_F(BoxTest, IntersectsTriangleIntersecting) {
  Box box{p1, p2};

  EXPECT_TRUE(box.intersects(triangle_intersecting));
}

// Test intersection with dateline crossing box
TEST_F(BoxTest, IntersectsDatelineCrossingBox) {
  Box box{p_dateline1, p_dateline2};

  // Create triangles that would intersect with dateline-crossing box
  Triangle triangle_west{Point{160.0, 15.0}, Point{175.0, 15.0},
                         Point{167.5, 25.0}};

  Triangle triangle_east{Point{-175.0, 15.0}, Point{-160.0, 15.0},
                         Point{-167.5, 25.0}};

  EXPECT_TRUE(box.intersects(triangle_west));
  EXPECT_TRUE(box.intersects(triangle_east));
}

// Test with extreme coordinates
TEST_F(BoxTest, ExtremeCoordinates) {
  Box polar_box{p_south_pole, p_north_pole};

  EXPECT_DOUBLE_EQ(polar_box.min_corner().lat(), -90.0);
  EXPECT_DOUBLE_EQ(polar_box.max_corner().lat(), 90.0);
}

// Test angle normalization in constructor
TEST_F(BoxTest, AngleNormalization) {
  Point p_over_180{190.0, -45.0};      // > 180° = -170°
  Point p_under_neg180{-190.0, 45.0};  // < -180° = 170°

  Box box{p_over_180, p_under_neg180};

  EXPECT_EQ(box.min_corner().lon(), 190.0);
  EXPECT_LE(box.max_corner().lon(), 530.0);
}

// Test intersection with a dateline-crossing box using triangles
TEST_F(BoxTest, DatelineCrossingInvertedBoundaries) {
  // Create box: min={100, -66}, max={266, 66} -> normalized max={-94, 66}
  // This creates a dateline-crossing box split into:
  // Box 1: [100, -66, 180, 66] and Box 2: [-180, -66, -94, 66]
  Box box{Point{100, -66}, Point{266, 66}};

  // Test with large triangles that span each box region to ensure intersection

  // Triangle that clearly intersects the first box (100° to 180° longitude)
  Triangle triangle_spanning_first_box{Point{110, -30}, Point{170, -30},
                                       Point{140, 30}};
  boost::geometry::correct(triangle_spanning_first_box);
  auto intersect = box.intersects(triangle_spanning_first_box);
  EXPECT_TRUE(intersect);

  // Triangle that clearly intersects the second box (-180° to -94° longitude)
  Triangle triangle_spanning_second_box{Point{-170, -30}, Point{-100, -30},
                                        Point{-135, 30}};
  boost::geometry::correct(triangle_spanning_second_box);
  intersect = box.intersects(triangle_spanning_second_box);
  EXPECT_TRUE(intersect);

  // Triangle outside both boxes (in the gap between -94° and 100°)
  Triangle triangle_in_gap{Point{-50, 30}, Point{50, 30}, Point{0, 60}};
  boost::geometry::correct(triangle_in_gap);
  intersect = box.intersects(triangle_in_gap);
  EXPECT_FALSE(intersect);

  // Triangle that spans across the valid regions (should intersect both boxes)
  Triangle triangle_spanning_both{
      Point{150, 30},   // In first box
      Point{-150, 30},  // In second box
      Point{0, -30}     // In the gap, but triangle spans both
  };
  boost::geometry::correct(triangle_spanning_both);
  intersect = box.intersects(triangle_spanning_both);
  EXPECT_TRUE(intersect);
}

}  // namespace geometry
}  // namespace fes
