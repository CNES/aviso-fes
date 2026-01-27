// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/wave.hpp"

#include <gtest/gtest.h>

#include "fes/darwin/table.hpp"

namespace fes {
namespace darwin {

class AstronomicAngleForNodalG : public angle::Astronomic {
 public:
  // -2208988800 = 1900-01-01 00:00:00 UTC
  AstronomicAngleForNodalG()
      : angle::Astronomic(angle::Formulae::kMeeus, -2208988800.0) {
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
inline auto check_nodal_g(const WaveTable& table,
                          const fes::angle::Astronomic& angle) -> void {
  for (auto&& item : table) {
    item->nodal_g(angle);
    switch (item->ident()) {
      case kO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), +91.0, 1e-8);
        break;
      case kP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case kK1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case k2N2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kNu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kL2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 181.0, 1e-8);
        break;
      case kT2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kM4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kS1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case kQ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case kMm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kMf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kMtm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-12);
        break;
      case kMSqm:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kEps2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kLambda2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case kEta2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case k2Q1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case kSigma1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case kRho1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 91.0, 1e-8);
        break;
      case kM1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.51674239, 1e-8);
        break;
      case kM11:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kM12:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kM13:
        // EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kChi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kPi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 90.0, 1e-8);
        break;
      case kPhi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -86.0, 1e-8);
        break;
      case kTheta1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kJ1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kOO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case kM3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 3.0, 1e-8);
        break;
      case kM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case kMN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kMS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kR2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case kR4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kMNS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kMK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case kSK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4.0, 1e-8);
        break;
      case k2MN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case k2MS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case k2MK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case kMSN6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case k2SM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case kMSK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case kMP1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89.0, 1e-8);
        break;
      case k2SM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kPsi1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88.0, 1e-8);
        break;
      case k2MS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kMKS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case k2MN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 182.0, 1e-8);
        break;
      case kMSN2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kMO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case k2MK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93.0, 1e-8);
        break;
      case kMK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87.0, 1e-8);
        break;
      case kS6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6.0, 1e-8);
        break;
      case kM8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8.0, 1e-8);
        break;
      case kMSf:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kSsa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kSa:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 1.0, 1e-8);
        break;
      case kA5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kSa1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kSta:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2.0, 1e-8);
        break;
      case kMm1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180.0, 1e-8);
        break;
      case kMm2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case kMf1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kMf2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -90.0, 1e-8);
        break;
      case kM0:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 0.0, 1e-8);
        break;
      case kL2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -88, 1e-8);
        break;
      case kN2P:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 92, 1e-8);
        break;
      case kMSK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case kSKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case kOQ2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 180, 1e-8);
        break;
      case k3MS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case kMNu4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case k2MSN4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case k2NS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case kMNuS2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case k2MK2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case kNKM2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case kML4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case kSO1:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -89, 1e-8);
        break;
      case kSO3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 93, 1e-8);
        break;
      case kNK4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case kMNK6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case k2NM6:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 6, 1e-8);
        break;
      case k3MS8:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 8, 1e-8);
        break;
      case kSK3:
        EXPECT_NEAR(detail::math::degrees(item->vu()), -87, 1e-8);
        break;
      case k2MNS4:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 4, 1e-8);
        break;
      case k2SMu2:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 2, 1e-8);
        break;
      case k2MP5:
        EXPECT_NEAR(detail::math::degrees(item->vu()), 94, 1e-8);
        break;
      default:
        throw std::runtime_error(item->name());
    }
  }
}

inline auto check_nodal_a(const WaveTable& table,
                          const fes::angle::Astronomic& angle) {
  for (auto&& item : table) {
    item->nodal_a(angle);
    switch (item->ident()) {
      case kO1:
      case kQ1:
      case k2Q1:
      case kSigma1:
      case kRho1:
      case kM11:
      case kSO1:
      case kSO3:
        EXPECT_NEAR(item->f(), 1.705420655553602, 1e-8);
        break;
      case kP1:
      case kT2:
      case kS2:
      case kS1:
      case kPi1:
      case kPhi1:
      case kR2:
      case kR4:
      case kS4:
      case kPsi1:
      case kS6:
      case kSsa:
      case kSa:
      case kSa1:
      case kSta:
        EXPECT_NEAR(item->f(), 1.0, 1e-8);
        break;
      case kK1:
      case kSK3:
        EXPECT_NEAR(item->f(), 1.0661482919660317, 1e-8);
        break;
      case k2N2:
      case kMu2:
      case kN2:
      case kNu2:
      case kEps2:
      case kM2:
      case kSN4:
      case kMS4:
      case kLambda2:
      case k2SM6:
      case k2SM2:
      case kMSf:
      case k2SMu2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case kL2:
        EXPECT_NEAR(item->f(), 0.64794930999090794, 1e-8);
        break;
      case kK2:
      case kSK4:
        EXPECT_NEAR(item->f(), 2.9715447147317122, 1e-8);
        break;
      case kEta2:
        EXPECT_NEAR(item->f(), 4.5244307876905507, 1e-8);
        break;
      case kM12:
      case kChi1:
      case kTheta1:
      case kJ1:
      case kMP1:
        EXPECT_NEAR(item->f(), 1.2604621941026914, 1e-8);
        break;
      case kOO1:
        EXPECT_NEAR(item->f(), 11.793361932776087, 1e-8);
        break;
      case kM4:
      case kMN4:
      case kMNS2:
      case kN4:
      case k2MS6:
      case kMSN6:
      case k2MS2:
      case kMSN2:
      case kMNu4:
      case kMNuS2:
      case k2MP5:
      case k2NS2:
        EXPECT_NEAR(item->f(), 0.4198383083176937, 1e-8);
        break;
      case kMf:
      case kMtm:
      case kMSqm:
      case kMm1:
        EXPECT_NEAR(item->f(), 4.4871572767653438, 1e-8);
        break;
      case kA5:
      case kM0:
      case kMf1:
      case kMm:
        EXPECT_NEAR(item->f(), -0.08246714122068223, 1e-8);
        break;
      case kM3:
        EXPECT_NEAR(item->f(), 0.5215824486233587, 1e-8);
        break;
      case kM6:
      case k2MN6:
      case k2MN2:
      case k3MS4:
      case k2MSN4:
      case k3MS8:
      case k2MNS4:
        EXPECT_NEAR(item->f(), 0.2720339421821998, 1e-8);
        break;
      case kM1:
        EXPECT_NEAR(item->f(), 3.300330475634, 1e-8);
        break;
      case kMK4:
      case kMSK6:
      case kMSK2:
      case kSKM2:
      case kNK4:
      case kMKS2:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 2.9715447147317122, 1e-8);
        break;
      case k2MK6:
        EXPECT_NEAR(item->f(), 0.2720339421821998 * 2.9715447147317122, 1e-8);
        break;
      case kMO3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.705420655553602, 1e-8);
        break;
      case k2MK3:
        EXPECT_NEAR(item->f(), 0.4198383083176937 * 1.0661482919660317, 1e-8);
        break;
      case kMK3:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 1.0661482919660317, 1e-8);
        break;
      case kM8:
        EXPECT_NEAR(item->f(), detail::math::pow<4>(0.64794930999090794), 1e-8);
        break;
      case kMm2:
      case kMf2:
        EXPECT_NEAR(item->f(), 0.302919608612788327, 1e-8);
        break;
      case kM13:
        break;
      case kL2P:
        EXPECT_NEAR(item->f(), 5.4040577532657145809, 1e-8);
        break;
      case kN2P:
        EXPECT_NEAR(item->f(), 1.3644178238453521512, 1e-8);
        break;
      case kOQ2:
        EXPECT_NEAR(item->f(), detail::math::pow<2>(1.705420655553602), 1e-8);
        break;
      case k2MK2:
      case kNKM2:
      case kMNK6:
        EXPECT_NEAR(
            item->f(),
            detail::math::pow<2>(0.64794930999090794) * 2.9715447147317122,
            1e-8);
        break;
      case kML4:
        EXPECT_NEAR(item->f(), 0.64794930999090794 * 0.64794930999090794, 1e-8);
        break;
      case k2NM6:
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

TEST(Wave, NodalG) { check_nodal_g(WaveTable(), AstronomicAngleForNodalG()); }

TEST(Wave, NodalA) { check_nodal_a(WaveTable(), AstronomicAngleForNodalA()); }

TEST(Wave, DoodsonNumber) {
  ASSERT_EQ(wave::_2SM2().xdo_numerical(), "2915555");
  ASSERT_EQ(wave::_2SM2().xdo_alphabetical(), "BDVZZZZ");
  ASSERT_EQ(wave::O1().xdo_numerical(), "1455554");
  ASSERT_EQ(wave::O1().xdo_alphabetical(), "AYZZZZY");
}

}  // namespace darwin
}  // namespace fes
