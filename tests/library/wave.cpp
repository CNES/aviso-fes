// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave.hpp"

#include <gtest/gtest.h>

#include "fes/wave/table.hpp"

namespace detail = fes::detail;

class AstronomicAngleForNodalG : public fes::angle::Astronomic {
 public:
  // -2208988800 = 1900-01-01 00:00:00 UTC
  AstronomicAngleForNodalG()
      : fes::angle::Astronomic(fes::angle::Formulae::kMeeus, -2208988800.0) {
    t_ = detail::math::radians(1.0);
    h_ = detail::math::radians(1.0);
    s_ = detail::math::radians(1.0);
    p1_ = detail::math::radians(1.0);
    p_ = detail::math::radians(1.0);
    i_ = detail::math::radians(1.0);
    xi_ = detail::math::radians(1.0);
    nu_ = detail::math::radians(1.0);
    x1ra_ = detail::math::radians(1.0);
    r_ = detail::math::radians(1.0);
    nuprim_ = detail::math::radians(1.0);
    nusec_ = detail::math::radians(1.0);
  }
};

class AstronomicAngleForNodalA : public fes::angle::Astronomic {
 public:
  // -2208988800 = 1900-01-01 00:00:00 UTC
  AstronomicAngleForNodalA()
      : fes::angle::Astronomic(fes::angle::Formulae::kMeeus, -2208988800.0) {
    t_ = 1;
    h_ = 1;
    s_ = 1;
    p1_ = 1;
    p_ = 1;
    i_ = 1;
    xi_ = 1;
    nu_ = 1;
    x1ra_ = 1;
    r_ = 1;
    nuprim_ = 1;
    nusec_ = 1;
  }
};

// NOLINTBEGIN(readability-function-cognitive-complexity)
// Used to test all wave components in a single function
inline auto check_nodal_g(const fes::wave::Table& table,
                          const fes::angle::Astronomic& angle) -> void {
  for (auto&& item : table) {
    item->nodal_g(angle);
    switch (item->ident()) {
      case fes::kO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), +91.0, 1e-8);
        break;
      case fes::kP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case fes::kK1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::k2N2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kNu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kL2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 181.0, 1e-8);
        break;
      case fes::kT2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kM4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kS1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case fes::kQ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::kMm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kMf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kMtm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-12);
        break;
      case fes::kMSqm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kEps2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kLambda2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::kEta2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::k2Q1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::kSigma1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::kRho1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::kM1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.51674239, 1e-8);
        break;
      case fes::kM11:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kM12:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kM13:
        // EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kChi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kPi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case fes::kPhi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -86.0, 1e-8);
        break;
      case fes::kTheta1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kJ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kOO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::kM3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 3.0, 1e-8);
        break;
      case fes::kM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::kMN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kMS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kR2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::kR4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kMNS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kMK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::kSK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::k2MN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::k2MS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::k2MK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::kMSN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::k2SM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::kMSK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::kMP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::k2SM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kPsi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88.0, 1e-8);
        break;
      case fes::k2MS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kMKS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::k2MN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::kMSN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kMO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case fes::k2MK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case fes::kMK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87.0, 1e-8);
        break;
      case fes::kS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::kM8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8.0, 1e-8);
        break;
      case fes::kMSf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kSsa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kSa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case fes::kA5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kSa1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kSta:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::kMm1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180.0, 1e-8);
        break;
      case fes::kMm2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case fes::kMf1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kMf2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case fes::kM0:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::kL2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88, 1e-8);
        break;
      case fes::kN2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 92, 1e-8);
        break;
      case fes::kMSK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::kSKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::kOQ2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180, 1e-8);
        break;
      case fes::k3MS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::kMNu4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::k2MSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::k2NS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::kMNuS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::k2MK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::kNKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::kML4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::kSO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89, 1e-8);
        break;
      case fes::kSO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93, 1e-8);
        break;
      case fes::kNK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::kMNK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case fes::k2NM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case fes::k3MS8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8, 1e-8);
        break;
      case fes::kSK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87, 1e-8);
        break;
      case fes::k2MNS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::k2SMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::k2MP5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 94, 1e-8);
        break;
      default:
        throw std::runtime_error(item->name());
    }
  }
}

inline auto check_nodal_a(const fes::wave::Table& table,
                          const fes::angle::Astronomic& angle) {
  for (auto&& item : table) {
    item->nodal_a(angle);
    switch (item->ident()) {
      case fes::kO1:
      case fes::kQ1:
      case fes::k2Q1:
      case fes::kSigma1:
      case fes::kRho1:
      case fes::kM11:
      case fes::kSO1:
      case fes::kSO3:
        EXPECT_NEAR(item->f(), 1.705420655553602, 1e-8);
        break;
      case fes::kP1:
      case fes::kT2:
      case fes::kS2:
      case fes::kS1:
      case fes::kPi1:
      case fes::kPhi1:
      case fes::kR2:
      case fes::kR4:
      case fes::kS4:
      case fes::kPsi1:
      case fes::kS6:
      case fes::kSsa:
      case fes::kSa:
      case fes::kSa1:
      case fes::kSta:
        EXPECT_NEAR(item->f(), 1.0, 1e-8);
        break;
      case fes::kK1:
      case fes::kSK3:
        EXPECT_NEAR(item->f(), 1.0661482919660317, 1e-8);
        break;
      case fes::k2N2:
      case fes::kMu2:
      case fes::kN2:
      case fes::kNu2:
      case fes::kEps2:
      case fes::kM2:
      case fes::kSN4:
      case fes::kMS4:
      case fes::kLambda2:
      case fes::k2SM6:
      case fes::k2SM2:
      case fes::kMSf:
      case fes::k2SMu2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case fes::kL2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case fes::kK2:
      case fes::kSK4:
        EXPECT_NEAR(item->f(), 2.9715447147317122, 1e-8);
        break;
      case fes::kEta2:
        EXPECT_NEAR(item->f(), 4.5244307876905507, 1e-8);
        break;
      case fes::kM12:
      case fes::kChi1:
      case fes::kTheta1:
      case fes::kJ1:
      case fes::kMP1:
        EXPECT_NEAR(item->f(), 1.2604621941026914, 1e-8);
        break;
      case fes::kOO1:
        EXPECT_NEAR(item->f(), 11.793361932776087, 1e-8);
        break;
      case fes::kM4:
      case fes::kMN4:
      case fes::kMNS2:
      case fes::kN4:
      case fes::k2MS6:
      case fes::kMSN6:
      case fes::k2MS2:
      case fes::kMSN2:
      case fes::kMNu4:
      case fes::kMNuS2:
      case fes::k2MP5:
      case fes::k2NS2:
        EXPECT_NEAR(item->f(), 0.4198383083176937, 1e-8);
        break;
      case fes::kMf:
      case fes::kMtm:
      case fes::kMSqm:
      case fes::kMm1:
        EXPECT_NEAR(item->f(), 4.4871572767653438, 1e-8);
        break;
      case fes::kA5:
      case fes::kM0:
      case fes::kMf1:
      case fes::kMm:
        EXPECT_NEAR(item->f(), -0.08246714122068223, 1e-8);
        break;
      case fes::kM3:
        EXPECT_NEAR(item->f(), 0.5215824486233587, 1e-8);
        break;
      case fes::kM6:
      case fes::k2MN6:
      case fes::k2MN2:
      case fes::k3MS4:
      case fes::k2MSN4:
      case fes::k3MS8:
      case fes::k2MNS4:
        EXPECT_NEAR(item->f(), 0.2720339421821998, 1e-8);
        break;
      case fes::kM1:
        EXPECT_NEAR(item->f(), 3.300330475634, 1e-8);
        break;
      case fes::kMK4:
      case fes::kMSK6:
      case fes::kMSK2:
      case fes::kSKM2:
      case fes::kNK4:
      case fes::kMKS2:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 2.9715447147317122, 1e-8);
        break;
      case fes::k2MK6:
        EXPECT_NEAR(item->f(), 0.2720339421821998 * 2.9715447147317122, 1e-8);
        break;
      case fes::kMO3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.705420655553602, 1e-8);
        break;
      case fes::k2MK3:
        EXPECT_NEAR(item->f(), 0.4198383083176937 * 1.0661482919660317, 1e-8);
        break;
      case fes::kMK3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.0661482919660317, 1e-8);
        break;
      case fes::kM8:
        EXPECT_NEAR(item->f(), detail::math::pow<4>(0.64794930999090794), 1e-8);
        break;
      case fes::kMm2:
      case fes::kMf2:
        EXPECT_NEAR(item->f(), 0.302919608612788327, 1e-8);
        break;
      case fes::kM13:
        break;
      case fes::kL2P:
        EXPECT_NEAR(item->f(), 5.4040577532657145809, 1e-8);
        break;
      case fes::kN2P:
        EXPECT_NEAR(item->f(), 1.3644178238453521512, 1e-8);
        break;
      case fes::kOQ2:
        EXPECT_NEAR(item->f(), detail::math::pow<2>(1.705420655553602), 1e-8);
        break;
      case fes::k2MK2:
      case fes::kNKM2:
      case fes::kMNK6:
        EXPECT_NEAR(
            item->f(),
            detail::math::pow<2>(0.64794930999090794) * 2.9715447147317122,
            1e-8);
        break;
      case fes::kML4:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 0.64794930999090794, 1e-8);
        break;
      case fes::k2NM6:
        EXPECT_NEAR(
            item->f(),
            detail::math::pow<4>(0.64794930999090794) * 0.64794930999090794,
            1e-8);
        break;
      default:
        throw std::runtime_error(std::to_string(item->ident()));
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity)

TEST(Wave, NodalG) {
  check_nodal_g(fes::wave::Table(), AstronomicAngleForNodalG());
}

TEST(Wave, NodalA) {
  check_nodal_a(fes::wave::Table(), AstronomicAngleForNodalA());
}

TEST(Wave, DoodsonNumber) {
  ASSERT_EQ(fes::wave::_2SM2().xdo_numerical(), "2915555");
  ASSERT_EQ(fes::wave::_2SM2().xdo_alphabetical(), "BDVZZZZ");
  ASSERT_EQ(fes::wave::O1().xdo_numerical(), "1455554");
  ASSERT_EQ(fes::wave::O1().xdo_alphabetical(), "AYZZZZY");
}
