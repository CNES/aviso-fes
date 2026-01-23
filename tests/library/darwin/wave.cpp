// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/wave.hpp"

#include <gtest/gtest.h>

#include "fes/darwin/table.hpp"
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
inline auto check_nodal_g(const fes::darwin::WaveTable& table,
                          const fes::angle::Astronomic& angle) -> void {
  for (auto&& item : table) {
    item->nodal_g(angle);
    switch (item->ident()) {
      case fes::darwin::kO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), +91.0, 1e-8);
        break;
      case fes::darwin::kP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case fes::darwin::kK1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::k2N2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kNu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kL2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 181.0, 1e-8);
        break;
      case fes::darwin::kT2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kM4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kS1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case fes::darwin::kQ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::darwin::kMm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kMf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kMtm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-12);
        break;
      case fes::darwin::kMSqm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kEps2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kLambda2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::darwin::kEta2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::k2Q1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::darwin::kSigma1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::darwin::kRho1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case fes::darwin::kM1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.51674239, 1e-8);
        break;
      case fes::darwin::kM11:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kM12:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kM13:
        // EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kChi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kPi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case fes::darwin::kPhi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -86.0, 1e-8);
        break;
      case fes::darwin::kTheta1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kJ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kOO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::kM3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 3.0, 1e-8);
        break;
      case fes::darwin::kM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::kMN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kMS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kR2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::darwin::kR4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kMNS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kMK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::kSK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case fes::darwin::k2MN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::k2MS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::k2MK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::kMSN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::k2SM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::kMSK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::kMP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case fes::darwin::k2SM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kPsi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88.0, 1e-8);
        break;
      case fes::darwin::k2MS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kMKS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::k2MN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case fes::darwin::kMSN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kMO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case fes::darwin::k2MK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case fes::darwin::kMK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87.0, 1e-8);
        break;
      case fes::darwin::kS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case fes::darwin::kM8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8.0, 1e-8);
        break;
      case fes::darwin::kMSf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kSsa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kSa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case fes::darwin::kA5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kSa1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kSta:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case fes::darwin::kMm1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180.0, 1e-8);
        break;
      case fes::darwin::kMm2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case fes::darwin::kMf1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kMf2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case fes::darwin::kM0:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case fes::darwin::kL2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88, 1e-8);
        break;
      case fes::darwin::kN2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 92, 1e-8);
        break;
      case fes::darwin::kMSK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::kSKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::kOQ2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180, 1e-8);
        break;
      case fes::darwin::k3MS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::kMNu4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::k2MSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::k2NS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::kMNuS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::k2MK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::kNKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::kML4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::kSO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89, 1e-8);
        break;
      case fes::darwin::kSO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93, 1e-8);
        break;
      case fes::darwin::kNK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::kMNK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case fes::darwin::k2NM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case fes::darwin::k3MS8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8, 1e-8);
        break;
      case fes::darwin::kSK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87, 1e-8);
        break;
      case fes::darwin::k2MNS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case fes::darwin::k2SMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case fes::darwin::k2MP5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 94, 1e-8);
        break;
      default:
        throw std::runtime_error(item->name());
    }
  }
}

inline auto check_nodal_a(const fes::darwin::WaveTable& table,
                          const fes::angle::Astronomic& angle) {
  for (auto&& item : table) {
    item->nodal_a(angle);
    switch (item->ident()) {
      case fes::darwin::kO1:
      case fes::darwin::kQ1:
      case fes::darwin::k2Q1:
      case fes::darwin::kSigma1:
      case fes::darwin::kRho1:
      case fes::darwin::kM11:
      case fes::darwin::kSO1:
      case fes::darwin::kSO3:
        EXPECT_NEAR(item->f(), 1.705420655553602, 1e-8);
        break;
      case fes::darwin::kP1:
      case fes::darwin::kT2:
      case fes::darwin::kS2:
      case fes::darwin::kS1:
      case fes::darwin::kPi1:
      case fes::darwin::kPhi1:
      case fes::darwin::kR2:
      case fes::darwin::kR4:
      case fes::darwin::kS4:
      case fes::darwin::kPsi1:
      case fes::darwin::kS6:
      case fes::darwin::kSsa:
      case fes::darwin::kSa:
      case fes::darwin::kSa1:
      case fes::darwin::kSta:
        EXPECT_NEAR(item->f(), 1.0, 1e-8);
        break;
      case fes::darwin::kK1:
      case fes::darwin::kSK3:
        EXPECT_NEAR(item->f(), 1.0661482919660317, 1e-8);
        break;
      case fes::darwin::k2N2:
      case fes::darwin::kMu2:
      case fes::darwin::kN2:
      case fes::darwin::kNu2:
      case fes::darwin::kEps2:
      case fes::darwin::kM2:
      case fes::darwin::kSN4:
      case fes::darwin::kMS4:
      case fes::darwin::kLambda2:
      case fes::darwin::k2SM6:
      case fes::darwin::k2SM2:
      case fes::darwin::kMSf:
      case fes::darwin::k2SMu2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case fes::darwin::kL2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case fes::darwin::kK2:
      case fes::darwin::kSK4:
        EXPECT_NEAR(item->f(), 2.9715447147317122, 1e-8);
        break;
      case fes::darwin::kEta2:
        EXPECT_NEAR(item->f(), 4.5244307876905507, 1e-8);
        break;
      case fes::darwin::kM12:
      case fes::darwin::kChi1:
      case fes::darwin::kTheta1:
      case fes::darwin::kJ1:
      case fes::darwin::kMP1:
        EXPECT_NEAR(item->f(), 1.2604621941026914, 1e-8);
        break;
      case fes::darwin::kOO1:
        EXPECT_NEAR(item->f(), 11.793361932776087, 1e-8);
        break;
      case fes::darwin::kM4:
      case fes::darwin::kMN4:
      case fes::darwin::kMNS2:
      case fes::darwin::kN4:
      case fes::darwin::k2MS6:
      case fes::darwin::kMSN6:
      case fes::darwin::k2MS2:
      case fes::darwin::kMSN2:
      case fes::darwin::kMNu4:
      case fes::darwin::kMNuS2:
      case fes::darwin::k2MP5:
      case fes::darwin::k2NS2:
        EXPECT_NEAR(item->f(), 0.4198383083176937, 1e-8);
        break;
      case fes::darwin::kMf:
      case fes::darwin::kMtm:
      case fes::darwin::kMSqm:
      case fes::darwin::kMm1:
        EXPECT_NEAR(item->f(), 4.4871572767653438, 1e-8);
        break;
      case fes::darwin::kA5:
      case fes::darwin::kM0:
      case fes::darwin::kMf1:
      case fes::darwin::kMm:
        EXPECT_NEAR(item->f(), -0.08246714122068223, 1e-8);
        break;
      case fes::darwin::kM3:
        EXPECT_NEAR(item->f(), 0.5215824486233587, 1e-8);
        break;
      case fes::darwin::kM6:
      case fes::darwin::k2MN6:
      case fes::darwin::k2MN2:
      case fes::darwin::k3MS4:
      case fes::darwin::k2MSN4:
      case fes::darwin::k3MS8:
      case fes::darwin::k2MNS4:
        EXPECT_NEAR(item->f(), 0.2720339421821998, 1e-8);
        break;
      case fes::darwin::kM1:
        EXPECT_NEAR(item->f(), 3.300330475634, 1e-8);
        break;
      case fes::darwin::kMK4:
      case fes::darwin::kMSK6:
      case fes::darwin::kMSK2:
      case fes::darwin::kSKM2:
      case fes::darwin::kNK4:
      case fes::darwin::kMKS2:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 2.9715447147317122, 1e-8);
        break;
      case fes::darwin::k2MK6:
        EXPECT_NEAR(item->f(), 0.2720339421821998 * 2.9715447147317122, 1e-8);
        break;
      case fes::darwin::kMO3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.705420655553602, 1e-8);
        break;
      case fes::darwin::k2MK3:
        EXPECT_NEAR(item->f(), 0.4198383083176937 * 1.0661482919660317, 1e-8);
        break;
      case fes::darwin::kMK3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.0661482919660317, 1e-8);
        break;
      case fes::darwin::kM8:
        EXPECT_NEAR(item->f(), detail::math::pow<4>(0.64794930999090794), 1e-8);
        break;
      case fes::darwin::kMm2:
      case fes::darwin::kMf2:
        EXPECT_NEAR(item->f(), 0.302919608612788327, 1e-8);
        break;
      case fes::darwin::kM13:
        break;
      case fes::darwin::kL2P:
        EXPECT_NEAR(item->f(), 5.4040577532657145809, 1e-8);
        break;
      case fes::darwin::kN2P:
        EXPECT_NEAR(item->f(), 1.3644178238453521512, 1e-8);
        break;
      case fes::darwin::kOQ2:
        EXPECT_NEAR(item->f(), detail::math::pow<2>(1.705420655553602), 1e-8);
        break;
      case fes::darwin::k2MK2:
      case fes::darwin::kNKM2:
      case fes::darwin::kMNK6:
        EXPECT_NEAR(
            item->f(),
            detail::math::pow<2>(0.64794930999090794) * 2.9715447147317122,
            1e-8);
        break;
      case fes::darwin::kML4:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 0.64794930999090794, 1e-8);
        break;
      case fes::darwin::k2NM6:
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
  check_nodal_g(fes::darwin::WaveTable(), AstronomicAngleForNodalG());
}

TEST(Wave, NodalA) {
  check_nodal_a(fes::darwin::WaveTable(), AstronomicAngleForNodalA());
}

TEST(Wave, DoodsonNumber) {
  ASSERT_EQ(fes::darwin::wave::_2SM2().xdo_numerical(), "2915555");
  ASSERT_EQ(fes::darwin::wave::_2SM2().xdo_alphabetical(), "BDVZZZZ");
  ASSERT_EQ(fes::darwin::wave::O1().xdo_numerical(), "1455554");
  ASSERT_EQ(fes::darwin::wave::O1().xdo_alphabetical(), "AYZZZZY");
}
