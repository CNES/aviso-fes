#include <gtest/gtest.h>

#include "fes/constituent.hpp"
#include "fes/perth/nodal_corrections.hpp"

namespace fes {
namespace perth {

TEST(NodalCorrectionsTest, EvaluateNodalCorrections) {
  constexpr double omega = -266.13901453365702;
  constexpr double p = 111.5928443590156;

  // Create vector with all constituents
  std::vector<ConstituentId> constituents{
      k2MK3, k2MK6,   k2MN2,  k2MN6, k2MS2,   k2MS6,    k2N2,    k2Q1,
      k2SM2, k2SM6,   kBeta1, kChi1, kDelta2, kEps2,    kEta2,   kGamma2,
      kJ1,   kK1,     kK2,    kL2,   kL2P,    kLambda2, kM1,     kM13,
      kM2,   kM3,     kM4,    kM6,   kM8,     kMf,      kMK3,    kMK4,
      kMKS2, kMm,     kMN4,   kMO3,  kMqm,    kMS4,     kMSf,    kMSK6,
      kMSm,  kMSN2,   kMSN6,  kMSqm, kMStm,   kMtm,     kMu2,    kN2,
      kN2P,  kN4,     kNu2,   kO1,   kOO1,    kP1,      kQ1,     kRho1,
      kS2,   kSigma1, kSK4,   kSN4,  kSO1,    kTau1,    kTheta1, kUps1,
  };

  // Compute nodal corrections
  auto results = evaluate_nodal_corrections(omega, p, constituents);
  ASSERT_EQ(results.size(), constituents.size());

  for (size_t i = 0; i < constituents.size(); ++i) {
    const auto& constituent = constituents[i];
    const auto& item = results[i];
    // Expected values based on values obtained from Richard Ray's code
    switch (constituent) {
      case k2Q1:
      case kSigma1:
      case kQ1:
      case kRho1:
        EXPECT_NEAR(item.f, 1.0050726, 1e-6);
        EXPECT_NEAR(item.u, 10.790722, 1e-6);
        break;
      case kO1:
        EXPECT_NEAR(item.f, 1.0163576, 1e-6);
        EXPECT_NEAR(item.u, 10.96953434, 1e-6);
        break;
      case kSO1:
        EXPECT_NEAR(item.f, 1.0163576, 1e-6);
        EXPECT_NEAR(item.u, -10.96953434, 1e-6);
        break;
      case kTau1:
        EXPECT_NEAR(item.f, 1.038005, 1e-6);
        EXPECT_NEAR(item.u, 12.1518206, 1e-6);
        break;
      case kBeta1:
        EXPECT_NEAR(item.f, 1.0102674, 1e-6);
        EXPECT_NEAR(item.u, 12.896785, 1e-6);
        break;
      case kM1:
        EXPECT_NEAR(item.f, 0.6851316, 1e-6);
        EXPECT_NEAR(item.u, -2.8682318, 1e-5);
        break;
      case kChi1:
        EXPECT_NEAR(item.f, 1.0180343, 1e-6);
        EXPECT_NEAR(item.u, -14.1826575, 1e-6);
        break;
      case kP1:
        EXPECT_NEAR(item.f, 1.0008165, 1e-6);
        EXPECT_NEAR(item.u, -0.6397472, 1e-6);
        break;
      case kK1:
        EXPECT_NEAR(item.f, 1.0070494, 1e-6);
        EXPECT_NEAR(item.u, -8.8805836, 1e-6);
        break;
      case kTheta1:
      case kJ1:
        EXPECT_NEAR(item.f, 1.0142314, 1e-6);
        EXPECT_NEAR(item.u, -12.9033333, 1e-6);
        break;
      case kOO1:
      case kUps1:
        EXPECT_NEAR(item.f, 0.8826517, 1e-6);
        EXPECT_NEAR(item.u, -35.9260518, 1e-6);
        break;
      case kEps2:
      case k2N2:
      case kMu2:
      case kN2:
      case kNu2:
      case kM2:
      case kLambda2:
      case kMS4:
        EXPECT_NEAR(item.f, 1.00269, 1e-6);
        EXPECT_NEAR(item.u, -2.131614, 1e-6);
        break;
      case kGamma2:
        EXPECT_NEAR(item.f, 1.122972, 1e-6);
        EXPECT_NEAR(item.u, -4.355695, 1e-6);
        break;
      case kDelta2:
        EXPECT_NEAR(item.f, 1.251786, 1e-6);
        EXPECT_NEAR(item.u, 8.286902, 1e-5);
        break;
      case kL2:
        EXPECT_NEAR(item.f, 1.255444, 1e-6);
        EXPECT_NEAR(item.u, 2.240606, 1e-6);
        break;
      case kS2:
        EXPECT_NEAR(item.f, 0.999851, 1e-6);
        EXPECT_NEAR(item.u, 0.128642, 1e-6);
        break;
      case kK2:
        EXPECT_NEAR(item.f, 0.996733, 1e-6);
        EXPECT_NEAR(item.u, -17.8630714, 1e-6);
        break;
      case kEta2:
        EXPECT_NEAR(item.f, 1.063663, 1e-6);
        EXPECT_NEAR(item.u, -24.140393, 1e-6);
        break;
      case kM4:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263229, 1e-6);
        break;
      case k2MK3:
        EXPECT_NEAR(item.f, 1.012476, 1e-6);
        EXPECT_NEAR(item.u, 4.617356, 1e-6);
        break;
      case k2MK6:
        EXPECT_NEAR(item.f, 1.002104, 1e-6);
        EXPECT_NEAR(item.u, -22.126300, 1e-6);
        break;
      case k2MN2:
        EXPECT_NEAR(item.f, 1.008094, 1e-6);
        EXPECT_NEAR(item.u, -2.131614, 1e-6);
        break;
      case k2MN6:
        EXPECT_NEAR(item.f, 1.008094, 1e-6);
        EXPECT_NEAR(item.u, -6.394842, 1e-6);
        break;
      case k2MS2:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263228, 1e-6);
        break;
      case k2MS6:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263228, 1e-6);
        break;
      case k2SM2:
        EXPECT_NEAR(item.f, 1.002691, 1e-6);
        EXPECT_NEAR(item.u, 2.131614, 1e-6);
        break;
      case k2SM6:
        EXPECT_NEAR(item.f, 1.002691, 1e-6);
        EXPECT_NEAR(item.u, -2.131614, 1e-6);
        break;
      case kL2P:
        EXPECT_NEAR(item.f, 1.021928, 1e-6);
        EXPECT_NEAR(item.u, -14.098653, 1e-6);
        break;
      case kM13:
        EXPECT_NEAR(item.f, 1.018905, 1e-6);
        EXPECT_NEAR(item.u, -1.018360, 1e-6);
        break;
      case kM3:
        EXPECT_NEAR(item.f, 1.005379, 1e-6);
        EXPECT_NEAR(item.u, -3.210853, 1e-6);
        break;
      case kM6:
        EXPECT_NEAR(item.f, 1.008094, 1e-6);
        EXPECT_NEAR(item.u, -6.394842, 1e-6);
        break;
      case kM8:
        EXPECT_NEAR(item.f, 1.010807, 1e-6);
        EXPECT_NEAR(item.u, -8.526457, 1e-6);
        break;
      case kMf:
        EXPECT_NEAR(item.f, 0.978514, 1e-6);
        EXPECT_NEAR(item.u, -22.782435, 1e-6);
        break;
      case kMK3:
        EXPECT_NEAR(item.f, 1.009759, 1e-6);
        EXPECT_NEAR(item.u, -11.012198, 1e-6);
        break;
      case kMK4:
        EXPECT_NEAR(item.f, 0.999415, 1e-6);
        EXPECT_NEAR(item.u, -19.994686, 1e-6);
        break;
      case kMKS2:
        EXPECT_NEAR(item.f, 0.999415, 1e-6);
        EXPECT_NEAR(item.u, -19.994686, 1e-6);
        break;
      case kMm:
        EXPECT_NEAR(item.f, 1.061803, 1e-6);
        EXPECT_NEAR(item.u, 1.057907, 1e-6);
        break;
      case kMN4:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263228, 1e-6);
        break;
      case kMO3:
        EXPECT_NEAR(item.f, 1.019093, 1e-6);
        EXPECT_NEAR(item.u, 8.837919, 1e-6);
        break;
      case kMqm:
        EXPECT_NEAR(item.f, 0.978514, 1e-6);
        EXPECT_NEAR(item.u, -22.782435, 1e-6);
        break;
      case kMSf:
        EXPECT_NEAR(item.f, 1.009299, 1e-6);
        EXPECT_NEAR(item.u, 7.783470, 1e-6);
        break;
      case kMSK6:
        EXPECT_NEAR(item.f, 0.999415, 1e-6);
        EXPECT_NEAR(item.u, 15.731458, 1e-6);
        break;
      case kMSm:
        EXPECT_NEAR(item.f, 1.061803, 1e-6);
        EXPECT_NEAR(item.u, 1.057907, 1e-6);
        break;
      case kMSN2:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, 0.000000, 1e-6);
        break;
      case kMSN6:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263228, 1e-6);
        break;
      case kMSqm:
        EXPECT_NEAR(item.f, 0.978514, 1e-6);
        EXPECT_NEAR(item.u, -22.782435, 1e-6);
        break;
      case kMStm:
        EXPECT_NEAR(item.f, 0.674669, 1e-6);
        EXPECT_NEAR(item.u, -12.587631, 1e-6);
        break;
      case kMtm:
        EXPECT_NEAR(item.f, 1.000936, 1e-6);
        EXPECT_NEAR(item.u, -23.296901, 1e-6);
        break;
      case kN2P:
        EXPECT_NEAR(item.f, 1.021282, 1e-6);
        EXPECT_NEAR(item.u, 10.301177, 1e-6);
        break;
      case kN4:
        EXPECT_NEAR(item.f, 1.005389, 1e-6);
        EXPECT_NEAR(item.u, -4.263228, 1e-6);
        break;
      case kSK4:
        EXPECT_NEAR(item.f, 0.996733, 1e-6);
        EXPECT_NEAR(item.u, -17.863072, 1e-6);
        break;
      case kSN4:
        EXPECT_NEAR(item.f, 1.002691, 1e-6);
        EXPECT_NEAR(item.u, -2.131614, 1e-6);
        break;
      default:
        FAIL() << "Unexpected constituent: " << static_cast<int>(constituent);
        break;
    }
  }
}

TEST(NodalCorrectionsTest, EvaluateGroupNodalCorrections) {
  double perihelion = 282.64503464175243;
  double omega = -266.13901453365702;
  double perigee = 111.5928443590156;
  double hsolar = 280.08979471465113;

  // Create vector with all constituents
  std::vector<ConstituentId> constituents{
      k2N2, k2Q1, kEps2, kJ1,  kK1,   kL2,     kM1,  kM2, kMf,
      kMm,  kMqm, kMSf,  kMSm, kMSqm, kMtm,    kMu2, kN2, kNode,
      kO1,  kOO1, kQ1,   kS2,  kSa,   kSigma1, kSta};

  // Compute nodal corrections
  auto results = evaluate_nodal_corrections(perihelion, omega, perigee, hsolar,
                                            constituents);
  ASSERT_EQ(results.size(), constituents.size());

  for (size_t i = 0; i < constituents.size(); ++i) {
    const auto& constituent = constituents[i];
    const auto& item = results[i];
    // Expected values based on values obtained from Richard Ray's code
    switch (constituent) {
      case kNode:
      case kSa:
      case kSsa:
      case kSta:
        EXPECT_EQ(item.f, 1.0);
        EXPECT_EQ(item.u, 0.0);
        break;
      case kMSm:
        EXPECT_NEAR(item.f, 1.0618029, 1e-6);
        EXPECT_NEAR(item.u, 1.0579070, 1e-6);
        break;
      case kMm:
        EXPECT_NEAR(item.f, 1.2549187, 1e-6);
        EXPECT_NEAR(item.u, 4.4933152, 1e-5);
        break;
      case kMSf:
        EXPECT_NEAR(item.f, 1.0092987, 1e-6);
        EXPECT_NEAR(item.u, 7.7834698, 1e-6);
        break;
      case kMf:
        EXPECT_NEAR(item.f, 0.8911305, 1e-6);
        EXPECT_NEAR(item.u, -23.029193, 1e-5);
        break;
      case kMStm:
        EXPECT_NEAR(item.f, 0.6746692, 1e-6);
        EXPECT_NEAR(item.u, -12.5876318, 1e-6);
        break;
      case kMtm:
        EXPECT_NEAR(item.f, 1.1711231, 1e-6);
        EXPECT_NEAR(item.u, -19.3849117, 1e-5);
        break;
      case kMSqm:
        EXPECT_NEAR(item.f, 0.9785142, 1e-6);
        EXPECT_NEAR(item.u, -22.7824356, 1e-6);
        break;
      case kMqm:
        EXPECT_NEAR(item.f, 2.2833835, 1e-6);
        EXPECT_NEAR(item.u, -10.3654824, 1e-5);
        break;
      case kS2:
        EXPECT_NEAR(item.f, 0.780631, 1e-6);
        EXPECT_NEAR(item.u, -0.815940, 1e-6);
        break;
      case k2N2:
        EXPECT_NEAR(item.f, 2.275933, 1e-6);
        EXPECT_NEAR(item.u, -15.557717, 1e-6);
        break;
      case k2Q1:
        EXPECT_NEAR(item.f, 2.175457, 1e-6);
        EXPECT_NEAR(item.u, -1.8271054, 1e-6);
        break;
      case kEps2:
        EXPECT_NEAR(item.f, 1.362711, 1e-6);
        EXPECT_NEAR(item.u, 6.339470, 1e-6);
        break;
      case kJ1:
        EXPECT_NEAR(item.f, 1.202151, 1e-6);
        EXPECT_NEAR(item.u, -8.431590, 1e-6);
        break;
      case kK1:
        EXPECT_NEAR(item.f, 1.335916, 1e-6);
        EXPECT_NEAR(item.u, -11.983923, 1e-6);
        break;
      case kL2:
        EXPECT_NEAR(item.f, 1.554452, 1e-6);
        EXPECT_NEAR(item.u, 5.385221, 1e-6);
        break;
      case kM1:
        EXPECT_NEAR(item.f, 0.755623, 1e-6);
        EXPECT_NEAR(item.u, -6.503163, 1e-5);
        break;
      case kM2:
        EXPECT_NEAR(item.f, 0.999082, 1e-6);
        EXPECT_NEAR(item.u, -2.240433, 1e-6);
        break;
      case kMu2:
        EXPECT_NEAR(item.f, 1.886803, 1e-6);
        EXPECT_NEAR(item.u, 7.461294, 1e-6);
        break;
      case kN2:
        EXPECT_NEAR(item.f, 1.181877, 1e-6);
        EXPECT_NEAR(item.u, -5.775347, 1e-6);
        break;
      case kO1:
        EXPECT_NEAR(item.f, 1.029290, 1e-6);
        EXPECT_NEAR(item.u, 11.086248, 1e-6);
        break;
      case kOO1:
        EXPECT_NEAR(item.f, 0.580194, 1e-6);
        EXPECT_NEAR(item.u, -38.337098, 1e-6);
        break;
      case kQ1:
        EXPECT_NEAR(item.f, 1.183320, 1e-6);
        EXPECT_NEAR(item.u, 7.174645, 1e-6);
        break;
      case kSigma1:
        EXPECT_NEAR(item.f, 1.870445, 1e-6);
        EXPECT_NEAR(item.u, 19.853450, 1e-6);
        break;
      default:
        FAIL() << "Unexpected constituent: " << static_cast<int>(constituent);
        break;
    }
  }
}

}  // namespace perth
}  // namespace fes
