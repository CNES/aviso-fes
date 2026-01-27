// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin.hpp"

#include <gtest/gtest.h>

namespace fes {

// Test Darwin constructor with all parameters
TEST(Darwin, Constructor) {
  constexpr Darwin darwin(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

  EXPECT_EQ(darwin.t, 1);
  EXPECT_EQ(darwin.s, 2);
  EXPECT_EQ(darwin.h, 3);
  EXPECT_EQ(darwin.p, 4);
  EXPECT_EQ(darwin.n, 5);
  EXPECT_EQ(darwin.p1, 6);
  EXPECT_EQ(darwin.shift, 7);
  EXPECT_EQ(darwin.eps, 8);
  EXPECT_EQ(darwin.nu, 9);
  EXPECT_EQ(darwin.nuprim, 10);
  EXPECT_EQ(darwin.nusec, 11);
}

// Test Darwin constructor with zero values
TEST(Darwin, ConstructorZero) {
  constexpr Darwin darwin(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Darwin constructor with negative values
TEST(Darwin, ConstructorNegative) {
  constexpr Darwin darwin(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11);

  EXPECT_EQ(darwin.t, -1);
  EXPECT_EQ(darwin.s, -2);
  EXPECT_EQ(darwin.h, -3);
  EXPECT_EQ(darwin.p, -4);
  EXPECT_EQ(darwin.n, -5);
  EXPECT_EQ(darwin.p1, -6);
  EXPECT_EQ(darwin.shift, -7);
  EXPECT_EQ(darwin.eps, -8);
  EXPECT_EQ(darwin.nu, -9);
  EXPECT_EQ(darwin.nuprim, -10);
  EXPECT_EQ(darwin.nusec, -11);
}

// Test Darwin constructor with extreme int8_t values
TEST(Darwin, ConstructorInt8Range) {
  constexpr Darwin darwin(127, -128, 127, -128, 127, -128, 127, -128, 127, -128,
                          127);

  EXPECT_EQ(darwin.t, 127);
  EXPECT_EQ(darwin.s, -128);
  EXPECT_EQ(darwin.h, 127);
  EXPECT_EQ(darwin.p, -128);
  EXPECT_EQ(darwin.n, 127);
  EXPECT_EQ(darwin.p1, -128);
  EXPECT_EQ(darwin.shift, 127);
  EXPECT_EQ(darwin.eps, -128);
  EXPECT_EQ(darwin.nu, 127);
  EXPECT_EQ(darwin.nuprim, -128);
  EXPECT_EQ(darwin.nusec, 127);
}

// Test Builder default constructor
TEST(DarwinBuilder, DefaultConstructor) {
  constexpr Darwin::Builder builder;

  // Build with default values (all zeros)
  constexpr Darwin darwin = builder.build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder T method
TEST(DarwinBuilder, T) {
  constexpr Darwin darwin = Darwin::Builder().T(42).build();

  EXPECT_EQ(darwin.t, 42);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder s method
TEST(DarwinBuilder, s) {
  constexpr Darwin darwin = Darwin::Builder().s(-15).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, -15);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder h method
TEST(DarwinBuilder, h) {
  constexpr Darwin darwin = Darwin::Builder().h(7).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 7);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder p method
TEST(DarwinBuilder, p) {
  constexpr Darwin darwin = Darwin::Builder().p(3).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 3);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder n method
TEST(DarwinBuilder, n) {
  constexpr Darwin darwin = Darwin::Builder().n(-2).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, -2);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder p1 method
TEST(DarwinBuilder, p1) {
  constexpr Darwin darwin = Darwin::Builder().p1(1).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 1);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder shift method
TEST(DarwinBuilder, shift) {
  constexpr Darwin darwin = Darwin::Builder().shift(5).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 5);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder xi method (sets eps)
TEST(DarwinBuilder, xi) {
  constexpr Darwin darwin = Darwin::Builder().xi(-3).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, -3);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder nu method
TEST(DarwinBuilder, nu) {
  constexpr Darwin darwin = Darwin::Builder().nu(4).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 4);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder nuprim method
TEST(DarwinBuilder, nuprim) {
  constexpr Darwin darwin = Darwin::Builder().nuprim(-1).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, -1);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder nusec method
TEST(DarwinBuilder, nusec) {
  constexpr Darwin darwin = Darwin::Builder().nusec(2).build();

  EXPECT_EQ(darwin.t, 0);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 2);
}

// Test Builder method chaining
TEST(DarwinBuilder, MethodChaining) {
  constexpr Darwin darwin = Darwin::Builder()
                                .T(1)
                                .s(2)
                                .h(3)
                                .p(4)
                                .n(5)
                                .p1(6)
                                .shift(7)
                                .xi(8)
                                .nu(9)
                                .nuprim(10)
                                .nusec(11)
                                .build();

  EXPECT_EQ(darwin.t, 1);
  EXPECT_EQ(darwin.s, 2);
  EXPECT_EQ(darwin.h, 3);
  EXPECT_EQ(darwin.p, 4);
  EXPECT_EQ(darwin.n, 5);
  EXPECT_EQ(darwin.p1, 6);
  EXPECT_EQ(darwin.shift, 7);
  EXPECT_EQ(darwin.eps, 8);
  EXPECT_EQ(darwin.nu, 9);
  EXPECT_EQ(darwin.nuprim, 10);
  EXPECT_EQ(darwin.nusec, 11);
}

// Test Builder partial configuration
TEST(DarwinBuilder, PartialConfiguration) {
  constexpr Darwin darwin =
      Darwin::Builder().T(2).s(-2).h(2).xi(2).nu(-2).build();

  EXPECT_EQ(darwin.t, 2);
  EXPECT_EQ(darwin.s, -2);
  EXPECT_EQ(darwin.h, 2);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 2);
  EXPECT_EQ(darwin.nu, -2);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder with K2 constituent parameters
TEST(DarwinBuilder, K2Constituent) {
  constexpr Darwin darwin = Darwin::Builder().T(2).h(2).nusec(-2).build();

  EXPECT_EQ(darwin.t, 2);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 2);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, -2);
}

// Test Builder with S2 constituent parameters
TEST(DarwinBuilder, S2Constituent) {
  constexpr Darwin darwin = Darwin::Builder().T(2).build();

  EXPECT_EQ(darwin.t, 2);
  EXPECT_EQ(darwin.s, 0);
  EXPECT_EQ(darwin.h, 0);
  EXPECT_EQ(darwin.p, 0);
  EXPECT_EQ(darwin.n, 0);
  EXPECT_EQ(darwin.p1, 0);
  EXPECT_EQ(darwin.shift, 0);
  EXPECT_EQ(darwin.eps, 0);
  EXPECT_EQ(darwin.nu, 0);
  EXPECT_EQ(darwin.nuprim, 0);
  EXPECT_EQ(darwin.nusec, 0);
}

// Test Builder reusability
TEST(DarwinBuilder, Reusability) {
  Darwin::Builder builder;

  // First build
  builder.T(1).s(2);
  Darwin darwin1 = builder.build();

  EXPECT_EQ(darwin1.t, 1);
  EXPECT_EQ(darwin1.s, 2);

  // Modify and build again
  builder.h(3).p(4);
  Darwin darwin2 = builder.build();

  EXPECT_EQ(darwin2.t, 1);
  EXPECT_EQ(darwin2.s, 2);
  EXPECT_EQ(darwin2.h, 3);
  EXPECT_EQ(darwin2.p, 4);
}

// Test Builder return reference for chaining
TEST(DarwinBuilder, ReturnReference) {
  Darwin::Builder builder;

  // Test that methods return reference to builder
  const Darwin::Builder& ref1 = builder.T(1);
  const Darwin::Builder& ref2 = builder.s(2);
  const Darwin::Builder& ref3 = builder.h(3);

  EXPECT_EQ(&ref1, &builder);
  EXPECT_EQ(&ref2, &builder);
  EXPECT_EQ(&ref3, &builder);

  Darwin darwin = builder.build();
  EXPECT_EQ(darwin.t, 1);
  EXPECT_EQ(darwin.s, 2);
  EXPECT_EQ(darwin.h, 3);
}

// Test constexpr evaluation
TEST(Darwin, ConstexprEvaluation) {
  // Test that Darwin can be constructed at compile time
  constexpr Darwin darwin = Darwin::Builder()
                                .T(1)
                                .s(-1)
                                .h(2)
                                .p(1)
                                .n(-2)
                                .p1(3)
                                .shift(-1)
                                .xi(2)
                                .nu(-3)
                                .nuprim(1)
                                .nusec(-2)
                                .build();

  static_assert(darwin.t == 1, "t should be 1");
  static_assert(darwin.s == -1, "s should be -1");
  static_assert(darwin.h == 2, "h should be 2");
  static_assert(darwin.p == 1, "p should be 1");
  static_assert(darwin.n == -2, "n should be -2");
  static_assert(darwin.p1 == 3, "p1 should be 3");
  static_assert(darwin.shift == -1, "shift should be -1");
  static_assert(darwin.eps == 2, "eps should be 2");
  static_assert(darwin.nu == -3, "nu should be -3");
  static_assert(darwin.nuprim == 1, "nuprim should be 1");
  static_assert(darwin.nusec == -2, "nusec should be -2");
}

// Test Darwin equality comparison
TEST(Darwin, Equality) {
  constexpr Darwin darwin1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  constexpr Darwin darwin2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  constexpr Darwin darwin3(0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

  // Note: Darwin struct doesn't define operator==, so we compare manually
  EXPECT_EQ(darwin1.t, darwin2.t);
  EXPECT_EQ(darwin1.s, darwin2.s);
  EXPECT_EQ(darwin1.h, darwin2.h);
  EXPECT_EQ(darwin1.p, darwin2.p);
  EXPECT_EQ(darwin1.n, darwin2.n);
  EXPECT_EQ(darwin1.p1, darwin2.p1);
  EXPECT_EQ(darwin1.shift, darwin2.shift);
  EXPECT_EQ(darwin1.eps, darwin2.eps);
  EXPECT_EQ(darwin1.nu, darwin2.nu);
  EXPECT_EQ(darwin1.nuprim, darwin2.nuprim);
  EXPECT_EQ(darwin1.nusec, darwin2.nusec);

  EXPECT_NE(darwin1.t, darwin3.t);
}

}  // namespace fes
