// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/xdo.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace fes {

// Test the code function with standard values
TEST(XdoCode, StandardValues) {
  EXPECT_EQ(code(-1), '*');
  EXPECT_EQ(code(0), '0');
  EXPECT_EQ(code(1), '1');
  EXPECT_EQ(code(2), '2');
  EXPECT_EQ(code(3), '3');
  EXPECT_EQ(code(4), '4');
  EXPECT_EQ(code(5), '5');
  EXPECT_EQ(code(6), '6');
  EXPECT_EQ(code(7), '7');
  EXPECT_EQ(code(8), '8');
  EXPECT_EQ(code(9), '9');
  EXPECT_EQ(code(10), 'X');
  EXPECT_EQ(code(11), 'E');
  EXPECT_EQ(code(12), 'T');
}

// Test the code function with edge cases
TEST(XdoCode, EdgeCases) {
  // Test negative values (only -1 is special)
  EXPECT_EQ(code(-2), static_cast<char>(-2 + 48));    // -2 + 48 = 46 = '.'
  EXPECT_EQ(code(-10), static_cast<char>(-10 + 48));  // -10 + 48 = 38 = '&'

  // Test values beyond 12 (should use default case)
  EXPECT_EQ(code(13), static_cast<char>(13 + 48));    // 13 + 48 = 61 = '='
  EXPECT_EQ(code(20), static_cast<char>(20 + 48));    // 20 + 48 = 68 = 'D'
  EXPECT_EQ(code(100), static_cast<char>(100 + 48));  // 100 + 48 = 148

  // Test int8_t range limits
  EXPECT_EQ(code(127), static_cast<char>(127 + 48));    // 127 + 48 = 175
  EXPECT_EQ(code(-128), static_cast<char>(-128 + 48));  // -128 + 48 = -80
}

// Test xdo_numerical with known doodson numbers
TEST(XdoNumerical, KnownConstituents) {
  // M2: doodson = [2, 0, 0, 0, 0, 0, 0]
  const Eigen::Vector<int8_t, 7> m2_doodson{2, 0, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_numerical(m2_doodson), "2555555");

  // O1: doodson = [1, -1, 0, 0, 0, 0, -1]
  const Eigen::Vector<int8_t, 7> o1_doodson{1, -1, 0, 0, 0, 0, -1};
  EXPECT_EQ(xdo_numerical(o1_doodson), "1455554");

  // _2SM2: doodson = [2, 4, -4, 0, 0, 0, 0]
  const Eigen::Vector<int8_t, 7> sm2_doodson{2, 4, -4, 0, 0, 0, 0};
  EXPECT_EQ(xdo_numerical(sm2_doodson), "2915555");

  // S2: doodson = [2, 2, -2, 0, 0, 0, 0]
  const Eigen::Vector<int8_t, 7> s2_doodson{2, 2, -2, 0, 0, 0, 0};
  EXPECT_EQ(xdo_numerical(s2_doodson), "2735555");

  // K2: doodson = [2, 2, 0, 0, 0, 0, 0]
  const Eigen::Vector<int8_t, 7> k2_doodson{2, 2, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_numerical(k2_doodson), "2755555");
}

// Test xdo_numerical with special code values
TEST(XdoNumerical, SpecialCodes) {
  // Test with -1 (should become '*')
  const Eigen::Vector<int8_t, 7> special_doodson{-1, 10, 11, 12, 13, 14, 15};
  EXPECT_EQ(xdo_numerical(special_doodson), "*?@ABCD");

  // Test with all zeros
  const Eigen::Vector<int8_t, 7> zero_doodson{0, 0, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_numerical(zero_doodson), "0555555");

  // Test with negative values
  const Eigen::Vector<int8_t, 7> negative_doodson{0, -1, -2, -3, -4, -5, -6};
  EXPECT_EQ(xdo_numerical(negative_doodson), "043210*");

  // Test with int8_t limits
  const Eigen::Vector<int8_t, 7> limits_doodson{127, -128, 127, -128,
                                                127, -128, 127};
  // This will produce characters based on the code function
  const std::string result = xdo_numerical(limits_doodson);
  EXPECT_EQ(result.length(), 7U);
}

// Test xdo_alphabetical with known constituents
TEST(XdoAlphabetical, KnownConstituents) {
  // M2: doodson = [2, 0, 0, 0, 0, 0, 0] -> "BZZZZZZ"
  const Eigen::Vector<int8_t, 7> m2_doodson{2, 0, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_alphabetical(m2_doodson), "BZZZZZZ");

  // O1: doodson = [1, -1, 0, 0, 0, 0, -1] -> "AYZZZZY"
  const Eigen::Vector<int8_t, 7> o1_doodson{1, -1, 0, 0, 0, 0, -1};
  EXPECT_EQ(xdo_alphabetical(o1_doodson), "AYZZZZY");

  // _2SM2: doodson = [2, 4, -4, 0, 0, 0, 0] -> "BDVZZZZ"
  const Eigen::Vector<int8_t, 7> sm2_doodson{2, 4, -4, 0, 0, 0, 0};
  EXPECT_EQ(xdo_alphabetical(sm2_doodson), "BDVZZZZ");

  // S2: doodson = [2, 2, -2, 0, 0, 0, 0] -> "BBXZZZZ"
  const Eigen::Vector<int8_t, 7> s2_doodson{2, 2, -2, 0, 0, 0, 0};
  EXPECT_EQ(xdo_alphabetical(s2_doodson), "BBXZZZZ");

  // K2: doodson = [2, 2, 0, 0, 0, 0, 0] -> "BBZZZZZ"
  const Eigen::Vector<int8_t, 7> k2_doodson{2, 2, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_alphabetical(k2_doodson), "BBZZZZZ");
}

// Test xdo_alphabetical with edge cases
TEST(XdoAlphabetical, EdgeCases) {
  // Test with all zeros -> all 'Z's
  const Eigen::Vector<int8_t, 7> zero_doodson{0, 0, 0, 0, 0, 0, 0};
  EXPECT_EQ(xdo_alphabetical(zero_doodson), "ZZZZZZZ");

  // Test with negative values
  const Eigen::Vector<int8_t, 7> negative_doodson{-1, -2, -3, -4, -5, -6, -7};
  EXPECT_EQ(xdo_alphabetical(negative_doodson), "YXWVUTS");

  // Test with positive values
  const Eigen::Vector<int8_t, 7> positive_doodson{5, 6, 7, 8, 9, 10, 11};
  EXPECT_EQ(xdo_alphabetical(positive_doodson), "EFGHIJK");

  // Test with int8_t limits
  const Eigen::Vector<int8_t, 7> limits_doodson{127, -128, 127, -128,
                                                127, -128, 127};
  EXPECT_THROW(xdo_alphabetical(limits_doodson), std::out_of_range);
}

// Test xdo_alphabetical array bounds
TEST(XdoAlphabetical, ArrayBounds) {
  // The xdo array has 25 elements (indices 0-24)
  // Values + 8 should be in range [-8, 16] to stay within bounds
  // But the function doesn't check bounds, so test edge cases

  // Test with value that gives index 0: value = -8
  const Eigen::Vector<int8_t, 7> min_doodson{-8, -8, -8, -8, -8, -8, -8};
  EXPECT_EQ(xdo_alphabetical(min_doodson), "RRRRRRR");

  // Test with value that gives index 24: value = 16
  const Eigen::Vector<int8_t, 7> max_doodson{16, 16, 16, 16, 16, 16, 16};
  EXPECT_EQ(xdo_alphabetical(max_doodson), "PPPPPPP");

  // Test with value that gives index beyond 24: value = 17 -> index 25 (out of
  // bounds!)
  const Eigen::Vector<int8_t, 7> overflow_doodson{17, 17, 17, 17, 17, 17, 17};
  EXPECT_THROW(xdo_alphabetical(overflow_doodson), std::out_of_range);
}

// Test mathematical correctness of XDO transformations
TEST(XdoMathematical, Correctness) {
  // Test that xdo_numerical and xdo_alphabetical produce consistent results
  // by verifying the transformation logic manually

  // Test case: [1, 2, 3, 4, 5, 6, 7]
  const Eigen::Vector<int8_t, 7> test_doodson{1, 2, 3, 4, 5, 6, 7};

  // For xdo_numerical:
  // First element: code(1) = '1'
  // Other elements: code(value + 5)
  // code(2+5) = code(7) = '7'
  // code(3+5) = code(8) = '8'
  // code(4+5) = code(9) = '9'
  // code(5+5) = code(10) = 'X'
  // code(6+5) = code(11) = 'E'
  // code(7+5) = code(12) = 'T'
  EXPECT_EQ(xdo_numerical(test_doodson), "1789XET");

  // For xdo_alphabetical:
  // Each value + 8, then lookup in xdo array
  // 1+8 = 9 -> 'A'
  // 2+8 = 10 -> 'B'
  // 3+8 = 11 -> 'C'
  // 4+8 = 12 -> 'D'
  // 5+8 = 13 -> 'E'
  // 6+8 = 14 -> 'F'
  // 7+8 = 15 -> 'G'
  EXPECT_EQ(xdo_alphabetical(test_doodson), "ABCDEFG");

  // Test case with negative values: [-1, -2, 0, 1, 2, 3, 4]
  const Eigen::Vector<int8_t, 7> negative_doodson{-1, -2, 0, 1, 2, 3, 4};

  // xdo_numerical: ['*', code(-2+5)=code(3)='3', '5', '6', '7', '8', '9']
  EXPECT_EQ(xdo_numerical(negative_doodson), "*356789");

  // xdo_alphabetical: values + 8 -> [7, 6, 8, 9, 10, 11, 12] -> ['Y', 'X', 'Z',
  // 'A', 'B', 'C', 'D']
  EXPECT_EQ(xdo_alphabetical(negative_doodson), "YXZABCD");
}

// Test consistency between numerical and alphabetical representations
TEST(XdoConsistency, NumericalVsAlphabetical) {
  // Test that both functions produce strings of the same length
  const Eigen::Vector<int8_t, 7> test_doodson{1, 2, 3, 4, 5, 6, 7};

  const std::string numerical = xdo_numerical(test_doodson);
  const std::string alphabetical = xdo_alphabetical(test_doodson);

  EXPECT_EQ(numerical.length(), 7U);
  EXPECT_EQ(alphabetical.length(), 7U);
  EXPECT_EQ(numerical.length(), alphabetical.length());
}

// Test that functions handle empty or invalid input gracefully
TEST(XdoRobustness, InputValidation) {
  // The functions take Eigen::Ref<const Vector7b> which should be size 7
  // Test with a properly sized vector
  const Eigen::Vector<int8_t, 7> valid_doodson{0, 1, 2, 3, 4, 5, 6};

  const std::string numerical = xdo_numerical(valid_doodson);
  const std::string alphabetical = xdo_alphabetical(valid_doodson);

  EXPECT_EQ(numerical.length(), 7U);
  EXPECT_EQ(alphabetical.length(), 7U);

  const Eigen::Vector<int8_t, 7> extreme_doodson{127, -128, 0, 0, 0, 0, 0};

  EXPECT_NO_THROW(xdo_numerical(extreme_doodson));
  EXPECT_THROW(xdo_alphabetical(extreme_doodson), std::out_of_range);
}

// Test constexpr evaluation capability
TEST(XdoConstexpr, ConstexprEvaluation) {
  // Test that code function can be evaluated at compile time
  static_assert(code(-1) == '*', "code(-1) should be '*'");
  static_assert(code(0) == '0', "code(0) should be '0'");
  static_assert(code(10) == 'X', "code(10) should be 'X'");
  static_assert(code(11) == 'E', "code(11) should be 'E'");
  static_assert(code(12) == 'T', "code(12) should be 'T'");
  static_assert(code(5) == '5', "code(5) should be '5'");

  // Note: xdo_numerical and xdo_alphabetical are inline functions, not
  // constexpr because they use std::string which is not constexpr until C++20
}

}  // namespace fes
