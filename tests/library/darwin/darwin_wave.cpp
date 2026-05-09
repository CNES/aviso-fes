// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/darwin/wave_table.hpp"
#include "fes/interface/wave.hpp"

namespace fes {
namespace darwin {
namespace {

// Reference nodal phase / amplitude values for the Meeus formulation evaluated
// at 1900-01-01 with all of the angles forced to 1 rad (or 1°) by the
// ``AstronomicAngleForNodalA``/``...G`` fixtures below. The two columns of
// each ternary are (Schureman, IERS-2010); the right column is selected when
// ``include/fes/numbers.hpp`` is built with ``FES_USE_IERS_CONSTANTS``.
constexpr bool kSchureman = numbers::kUseSchuremanConstants;

constexpr double kFO1 = kSchureman ? 1.705420655553 : 1.706183948373;
constexpr double kFK1 = kSchureman ? 1.066148291966 : 1.066697288438;
constexpr double kFM2 = kSchureman ? 0.647949309991 : 0.647889857568;
constexpr double kFK2 = kSchureman ? 2.971544714732 : 2.978532446798;
constexpr double kFEta2 = kSchureman ? 4.524430787691 : 4.529698552141;
constexpr double kFJ1 = kSchureman ? 1.260462194103 : 1.260986585530;
constexpr double kFOO1 = kSchureman ? 11.793361932776 : 11.832322017468;
constexpr double kFM4 = kSchureman ? 0.41983830831770 : 0.419761267539;
constexpr double kFMf = kSchureman ? 4.48715727676534 : 4.493136736300;
constexpr double kFMm = kSchureman ? -0.08246714122068 : -0.082433320739;
constexpr double kFM3 = kSchureman ? 0.52158244862336 : 0.521496703561;
constexpr double kFM6 = kSchureman ? 0.27203394218220 : 0.271959067839;
constexpr double kFM1 = kSchureman ? 3.300330475634 : 3.302230768603;
constexpr double kFMm2 = kSchureman ? 0.302919608612788 : 0.303011689233;
constexpr double kFL2P = kSchureman ? 0.60201203371380 : 0.602082294664;
constexpr double kFN2P = kSchureman ? 1.36441782384535 : 1.365052211150;

constexpr double kVuM1 = kSchureman ? 270.48325761 : 270.48332383;

}  // namespace

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
inline auto check_nodal_phase(const WaveTable& table) -> void {
  for (auto&& item : table) {
    auto& wave = item.value();
    switch (wave->ident()) {
      case kO1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), +91.0, 1e-8);
        break;
      case kP1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 90.0, 1e-8);
        break;
      case kK1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case k2N2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kMu2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kN2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kNu2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kM2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kL2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 181.0, 1e-8);
        break;
      case kT2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kK2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kM4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kS1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 1.0, 1e-8);
        break;
      case kQ1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 91.0, 1e-8);
        break;
      case kMm:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kMf:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kMtm:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-12);
        break;
      case kMSqm:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kEps2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kLambda2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 182.0, 1e-8);
        break;
      case kEta2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case k2Q1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 91.0, 1e-8);
        break;
      case kSigma1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 91.0, 1e-8);
        break;
      case kRho1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 91.0, 1e-8);
        break;
      case kM1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), kVuM1, 1e-8);
        break;
      case kM11:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kM12:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kM13:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 1.0, 1e-8);
        break;
      case kChi1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kPi1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 90.0, 1e-8);
        break;
      case kPhi1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 274.0, 1e-8);
        break;
      case kTheta1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kJ1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kOO1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case kM3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 3.0, 1e-8);
        break;
      case kM6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case kMN4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kMS4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kN4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kR2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 182.0, 1e-8);
        break;
      case kR4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kS4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kMNS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kMK4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kSN4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case kSK4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4.0, 1e-8);
        break;
      case k2MN6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case k2MS6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case k2MK6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case kMSN6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case k2SM6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case kMSK6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case kMP1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271.0, 1e-8);
        break;
      case k2SM2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kPsi1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 272.0, 1e-8);
        break;
      case k2MS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kMKS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case k2MN2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 182.0, 1e-8);
        break;
      case kMSN2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kMO3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 93.0, 1e-8);
        break;
      case k2MK3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 93.0, 1e-8);
        break;
      case kMK3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 273.0, 1e-8);
        break;
      case kS6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6.0, 1e-8);
        break;
      case kM8:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 8.0, 1e-8);
        break;
      case kMSf:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kSsa:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kSa:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 1.0, 1e-8);
        break;
      case kA5:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kSa1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kSta:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2.0, 1e-8);
        break;
      case kMm1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 180.0, 1e-8);
        break;
      case kMm2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 270.0, 1e-8);
        break;
      case kMf1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kMf2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 270.0, 1e-8);
        break;
      case kM0:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 0.0, 1e-8);
        break;
      case kL2P:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 272, 1e-8);
        break;
      case kN2P:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 92, 1e-8);
        break;
      case kMSK2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case kSKM2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case kOQ2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 180, 1e-8);
        break;
      case k3MS4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case kMNu4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case k2MSN4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case k2NS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case kMNuS2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case k2MK2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case kNKM2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case kML4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case kSO1:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 271, 1e-8);
        break;
      case kSO3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 93, 1e-8);
        break;
      case kNK4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case kMNK6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6, 1e-8);
        break;
      case k2NM6:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 6, 1e-8);
        break;
      case k3MS8:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 8, 1e-8);
        break;
      case kSK3:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 273, 1e-8);
        break;
      case k2MNS4:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 4, 1e-8);
        break;
      case k2SMu2:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 2, 1e-8);
        break;
      case k2MP5:
        EXPECT_NEAR(detail::math::degrees(wave->vu()), 94, 1e-8);
        break;
      default:
        throw std::runtime_error(wave->name());
    }
  }
}

inline auto check_nodal_amplitude(const WaveTable& table) {
  for (auto&& item : table) {
    const auto& wave = item.value();
    switch (wave->ident()) {
      case kO1:
      case kQ1:
      case k2Q1:
      case kSigma1:
      case kRho1:
      case kM11:
      case kSO1:
      case kSO3:
        EXPECT_NEAR(wave->f(), kFO1, 1e-8);
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
        EXPECT_NEAR(wave->f(), 1.0, 1e-8);
        break;
      case kK1:
      case kSK3:
        EXPECT_NEAR(wave->f(), kFK1, 1e-8);
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
      case kL2:
        EXPECT_NEAR(wave->f(), kFM2, 1e-8);
        break;
      case kK2:
      case kSK4:
        EXPECT_NEAR(wave->f(), kFK2, 1e-8);
        break;
      case kEta2:
        EXPECT_NEAR(wave->f(), kFEta2, 1e-8);
        break;
      case kM12:
      case kChi1:
      case kTheta1:
      case kJ1:
      case kMP1:
        EXPECT_NEAR(wave->f(), kFJ1, 1e-8);
        break;
      case kOO1:
        EXPECT_NEAR(wave->f(), kFOO1, 1e-8);
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
        EXPECT_NEAR(wave->f(), kFM4, 1e-8);
        break;
      case kMf:
      case kMtm:
      case kMSqm:
      case kMm1:
        EXPECT_NEAR(wave->f(), kFMf, 1e-8);
        break;
      case kA5:
      case kM0:
      case kMf1:
      case kMm:
        EXPECT_NEAR(wave->f(), kFMm, 1e-8);
        break;
      case kM3:
        EXPECT_NEAR(wave->f(), kFM3, 1e-8);
        break;
      case kM6:
      case k2MN6:
      case k2MN2:
      case k3MS4:
      case k2MSN4:
      case k3MS8:
      case k2MNS4:
        EXPECT_NEAR(wave->f(), kFM6, 1e-8);
        break;
      case kM1:
        EXPECT_NEAR(wave->f(), kFM1, 1e-8);
        break;
      case kMK4:
      case kMSK6:
      case kMSK2:
      case kSKM2:
      case kNK4:
      case kMKS2:
        EXPECT_NEAR(wave->f(), kFM2 * kFK2, 1e-8);
        break;
      case k2MK6:
        EXPECT_NEAR(wave->f(), kFM6 * kFK2, 1e-8);
        break;
      case kMO3:
        EXPECT_NEAR(wave->f(), kFM2 * kFO1, 1e-8);
        break;
      case k2MK3:
        EXPECT_NEAR(wave->f(), kFM4 * kFK1, 1e-8);
        break;
      case kMK3:
        EXPECT_NEAR(wave->f(), kFM2 * kFK1, 1e-8);
        break;
      case kM8:
        EXPECT_NEAR(wave->f(), detail::math::pow<4>(kFM2), 1e-8);
        break;
      case kMm2:
      case kMf2:
        EXPECT_NEAR(wave->f(), kFMm2, 1e-8);
        break;
      case kM13:
        break;
      case kL2P:
        EXPECT_NEAR(wave->f(), kFL2P, 1e-8);
        break;
      case kN2P:
        EXPECT_NEAR(wave->f(), kFN2P, 1e-8);
        break;
      case kOQ2:
        EXPECT_NEAR(wave->f(), detail::math::pow<2>(kFO1), 1e-8);
        break;
      case k2MK2:
      case kNKM2:
      case kMNK6:
        EXPECT_NEAR(wave->f(), detail::math::pow<2>(kFM2) * kFK2, 1e-8);
        break;
      case kML4:
        EXPECT_NEAR(wave->f(), kFM2 * kFM2, 1e-8);
        break;
      case k2NM6:
        EXPECT_NEAR(wave->f(), detail::math::pow<4>(kFM2) * kFM2, 1e-8);
        break;
      default:
        throw std::runtime_error(wave->name());
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity)

TEST(Wave, NodalPhase) {
  auto table = WaveTable();
  table.compute_nodal_corrections(AstronomicAngleForNodalG(), false);
  check_nodal_phase(table);
}

TEST(Wave, NodalAmplitude) {
  auto table = WaveTable();
  table.compute_nodal_corrections(AstronomicAngleForNodalA(), false);
  check_nodal_amplitude(table);
}

}  // namespace darwin
}  // namespace fes
