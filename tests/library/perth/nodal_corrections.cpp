#include "fes/perth/nodal_corrections.hpp"

#include <gtest/gtest.h>

namespace fes {
namespace perth {

TEST(NodalCorrectionsTest, ComputeNodalCorrections) {
  constexpr double omega = -266.13901453365702;
  constexpr double p = 111.5928443590156;

  // Create vector with all constituents
  std::vector<Constituent> constituents{
      k2Q1,  kSigma1, kQ1,    kRho1, kO1,    kTau1,   kBeta1,   kM1,
      kChi1, kPi1,    kP1,    kS1,   kK1,    kPsi1,   kPhi1,    kTheta1,
      kJ1,   kSO1,    kOO1,   kUps1, kEps2,  k2N2,    kMu2,     kN2,
      kNu2,  kGamma2, kAlpa2, kM2,   kBeta2, kDelta2, kLambda2, kL2,
      kT2,   kS2,     kR2,    kK2,   kEta2,  kM4,     kMS4,
  };

  // Compute nodal corrections
  auto results = compute_nodal_corrections(omega, p, constituents);
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
      default:
        EXPECT_EQ(item.f, 1.0);
        EXPECT_EQ(item.u, 0.0);
        break;
    }
  }
}

TEST(NodalCorrectionsTest, ComputeGroupNodalCorrections) {
  double perihelion = 282.64503464175243;
  double omega = -266.13901453365702;
  double perigee = 111.5928443590156;
  double hsolar = 280.08979471465113;

  // Create vector with all constituents
  std::vector<Constituent> constituents{kNode, kSa, kSsa,  kSta, kMSm,  kMm,
                                        kMSf,  kMf, kMStm, kMtm, kMSqm, kMqm};

  // Compute nodal corrections
  auto results = compute_nodal_corrections(perihelion, omega, perigee, hsolar,
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
      default:
        FAIL() << "Unexpected constituent: " << static_cast<int>(constituent);
        break;
    }
  }
}

}  // namespace perth
}  // namespace fes
