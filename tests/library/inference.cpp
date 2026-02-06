#include "fes/inference.hpp"

#include <gtest/gtest.h>

#include <boost/range/algorithm/find.hpp>

#include "fes/darwin//wave_table.hpp"
#include "fes/interface/wave_table.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/wave_table.hpp"

namespace fes {

static void admittance(const double r, const ConstituentId ident,
                       WaveTableInterface& table) {
  EXPECT_NEAR(table[ident]->tide().real(), r, 1e-6);
  EXPECT_NEAR(table[ident]->tide().imag(), r, 1e-6);
}

static void admittance(const Complex& c, const ConstituentId ident,
                       WaveTableInterface& table) {
  EXPECT_NEAR(table[ident]->tide().real(), c.real(), 1e-6)
      << "Admittance real part mismatch for constituent "
      << constituents::name(ident);
  EXPECT_NEAR(table[ident]->tide().imag(), c.imag(), 1e-6)
      << "Admittance imaginary part mismatch for constituent "
      << constituents::name(ident);
}

TEST(InferenceTest, SplineInference) {
  auto wt = fes::darwin::WaveTable();
  for (auto& item : wt) {
    item.value()->set_tide({1, 1});
  }

  auto inference = inference_factory(wt, InferenceType::kSpline);
  inference->apply(wt, 45.0);
  admittance(0.2378, k2Q1, wt);
  admittance(0.2706, kSigma1, wt);
  admittance(0.1688, kRho1, wt);
  admittance(0.0241, kM11, wt);
  admittance(0.0671, kM12, wt);
  admittance(0.0124, kChi1, wt);
  admittance(0.0201, kPi1, wt);
  admittance(0.0137, kPhi1, wt);
  admittance(0.009, kTheta1, wt);
  admittance(0.0447, kJ1, wt);
  admittance(0.0182, kOO1, wt);
  admittance(0.0796782, kEta2, wt);
  admittance(0.374697218357, kMu2, wt);
  admittance(0.157529811402, kNu2, wt);
  admittance(0.010949128375, kLambda2, wt);
  admittance(0.053354227357, kL2, wt);
  admittance(0.16871051505, kT2, wt);
  admittance(0.2387, k2N2, wt);
  admittance(0.094151295, kEps2, wt);
  // disable 2N2 from the admittance interpolation, and check that it falls back
  // to the default value
  wt[k2N2]->set_is_modeled(true);
  wt[k2N2]->set_tide({1, 1});
  inference->apply(wt, 45.0);
  admittance(0.499810, kEps2, wt);
}

TEST(InferenceTest, ZeroInference) {
  auto wt = fes::perth::WaveTable();
  auto inference = inference_factory(wt, InferenceType::kZero);
  auto major_ones = {kQ1, kO1, kK1, kN2, kM2, kS2, kNode};
  for (const auto& ident : major_ones) {
    wt[ident]->set_tide({1, 1});
    wt[ident]->set_is_modeled(true);
  }
  inference->apply(wt, 0.0);
  for (const auto& ident : perth::kAllConstituents) {
    if (boost::range::find(major_ones, ident) != major_ones.end()) {
      admittance({1, 1}, ident, wt);
    } else {
      admittance({0, 0}, ident, wt);
    }
  }
}

TEST(InferenceTest, LinearInference) {
  auto wt = fes::perth::WaveTable();
  wt[kQ1]->set_tide({2.044581413269043, -2.3776917457580566});
  wt[kQ1]->set_is_modeled(true);
  wt[kO1]->set_tide({7.7220735549926758, 1.2257133722305298});
  wt[kO1]->set_is_modeled(true);
  wt[kK1]->set_tide({-7.9487228393554688, 5.2526679039001465});
  wt[kK1]->set_is_modeled(true);
  wt[kN2]->set_tide({-18.638496398925781, 3.0053455829620361});
  wt[kN2]->set_is_modeled(true);
  wt[kM2]->set_tide({-90.521110534667969, -20.603012084960938});
  wt[kM2]->set_is_modeled(true);
  wt[kS2]->set_tide({-24.581066131591797, -25.664165496826172});
  wt[kS2]->set_is_modeled(true);

  auto inference = inference_factory(wt, InferenceType::kLinear);
  inference->apply(wt, 0.0);
  admittance({0.00600120762531, 0}, kNode, wt);
  admittance({0.000981803582639, 0}, kSa1, wt);
  admittance({0, 0}, kSa, wt);
  admittance({0.00567685497335, 0}, kSsa, wt);
  admittance({0.000301956157344, 0}, kSta, wt);
  admittance({0.000194261376302, 0}, kMSm, wt);
  admittance({0, 0}, kMm, wt);
  admittance({0, 0}, kMSf, wt);
  admittance({0, 0}, kMf, wt);
  admittance({0, 0}, kMStm, wt);
  admittance({0, 0}, kMtm, wt);
  admittance({0, 0}, kMSqm, wt);
  admittance({0, 0}, kMqm, wt);
  admittance({0.345374235884, -0.659828970603}, k2Q1, wt);
  admittance({0.405314874809, -0.741651143471}, kSigma1, wt);
  admittance({2.04458141327, -2.37769174576}, kQ1, wt);
  admittance({0.374264648975, -0.385564739613}, kRho1, wt);
  admittance({7.72207355499, 1.22571337223}, kO1, wt);
  admittance({0.0882534141843, 0.0182976221098}, kTau1, wt);
  admittance({0.0156418121897, 0.0164328671073}, kBeta1, wt);
  admittance({0, 0}, kM13, wt);
  admittance({0.08914731553, 0.188103950836}, kM1, wt);
  admittance({0.00397972781174, 0.0381488829608}, kChi1, wt);
  admittance({-0.106640940812, 0.0899846841932}, kPi1, wt);
  admittance({-2.06538415994, 1.5886264166}, kP1, wt);
  admittance({0, 0}, kS1, wt);
  admittance({-7.94872283936, 5.2526679039}, kK1, wt);
  admittance({-0.0433308448862, 0.0268763797534}, kPsi1, wt);
  admittance({-0.120850838546, 0.070814557071}, kPhi1, wt);
  admittance({-0.164708540354, 0.0679976111201}, kTheta1, wt);
  admittance({-0.924644508205, 0.365708596346}, kJ1, wt);
  admittance({-0.226401006119, 0.0735919848588}, kSO1, wt);
  admittance({-0.790318753751, 0.250664951998}, kOO1, wt);
  admittance({-0.204197172722, 0.0572931156696}, kUps1, wt);
  admittance({-0.813128850069, 0.616440554245}, kEps2, wt);
  admittance({-2.63883376133, 1.31731995837}, k2N2, wt);
  admittance({0, 0}, k2MS2, wt);
  admittance({-3.15566536866, 1.44075712248}, kMu2, wt);
  admittance({-18.6384963989, 3.00534558296}, kN2, wt);
  admittance({0, 0}, kN2P, wt);
  admittance({-3.50689007867, 0.394139957692}, kNu2, wt);
  admittance({-0.275197534713, -0.047390464698}, kGamma2, wt);
  admittance({-0.31376004794, -0.061572319504}, kAlpha2, wt);
  admittance({-90.5211105347, -20.603012085}, kM2, wt);
  admittance({-0.270542665049, -0.0668773504935}, kBeta2, wt);
  admittance({-0.0988340926886, -0.0264355582152}, kDelta2, wt);
  admittance({0, 0}, kMKS2, wt);
  admittance({-0.538754773015, -0.270323962669}, kLambda2, wt);
  admittance({0, 0}, k2MN2, wt);
  admittance({-1.98778866605, -1.105767707}, kL2, wt);
  admittance({0, 0}, kL2P, wt);
  admittance({-1.47773122408, -1.46162079689}, kT2, wt);
  admittance({-24.5810661316, -25.6641654968}, kS2, wt);
  admittance({-0.199977639752, -0.220433568779}, kR2, wt);
  admittance({-6.29653421465, -7.32973157639}, kK2, wt);
  admittance({0, 0}, kMSN2, wt);
  admittance({-0.20915463906, -0.540559080546}, kEta2, wt);
  admittance({0, 0}, k2SM2, wt);
  admittance({0, 0}, k2MK3, wt);
  admittance({0, 0}, kMO3, wt);
  admittance({0, 0}, kM3, wt);
  admittance({0, 0}, kMK3, wt);
  admittance({0, 0}, kN4, wt);
  admittance({0, 0}, kMN4, wt);
  admittance({0, 0}, kM4, wt);
  admittance({0, 0}, kSN4, wt);
  admittance({0, 0}, kMS4, wt);
  admittance({0, 0}, kMK4, wt);
  admittance({0, 0}, kS4, wt);
  admittance({0, 0}, kR4, wt);
  admittance({0, 0}, kSK4, wt);
  admittance({0, 0}, k2MN6, wt);
  admittance({0, 0}, kM6, wt);
  admittance({0, 0}, kMSN6, wt);
  admittance({0, 0}, k2MS6, wt);
  admittance({0, 0}, k2MK6, wt);
  admittance({0, 0}, k2SM6, wt);
  admittance({0, 0}, kMSK6, wt);
  admittance({0, 0}, kS6, wt);
  admittance({0, 0}, kM8, wt);
}

TEST(InferenceTest, FourierInference) {
  auto wt = fes::perth::WaveTable();
  wt[kQ1]->set_tide({2.044581413269043, -2.3776917457580566});
  wt[kQ1]->set_is_modeled(true);
  wt[kO1]->set_tide({7.7220735549926758, 1.2257133722305298});
  wt[kO1]->set_is_modeled(true);
  wt[kK1]->set_tide({-7.9487228393554688, 5.2526679039001465});
  wt[kK1]->set_is_modeled(true);
  wt[kN2]->set_tide({-18.638496398925781, 3.0053455829620361});
  wt[kN2]->set_is_modeled(true);
  wt[kM2]->set_tide({-90.521110534667969, -20.603012084960938});
  wt[kM2]->set_is_modeled(true);
  wt[kS2]->set_tide({-24.581066131591797, -25.664165496826172});
  wt[kS2]->set_is_modeled(true);

  auto inference = inference_factory(wt, InferenceType::kFourier);
  inference->apply(wt, 0.0);
  admittance({0.00600120762531, 0}, kNode, wt);
  admittance({0.00600120762531, 0}, kNode, wt);
  admittance({0.000981803582639, 0}, kSa1, wt);
  admittance({0, 0}, kSa, wt);
  admittance({0.00567685497335, 0}, kSsa, wt);
  admittance({0.000301956157344, 0}, kSta, wt);
  admittance({0.000194261376302, 0}, kMSm, wt);
  admittance({0, 0}, kMm, wt);
  admittance({0, 0}, kMSf, wt);
  admittance({0, 0}, kMf, wt);
  admittance({0, 0}, kMStm, wt);
  admittance({0, 0}, kMtm, wt);
  admittance({0, 0}, kMSqm, wt);
  admittance({0, 0}, kMqm, wt);
  admittance({0.26113937288, -0.788991134857}, k2Q1, wt);
  admittance({0.323168609247, -0.871859877843}, kSigma1, wt);
  admittance({2.04458141327, -2.37769174576}, kQ1, wt);
  admittance({0.381064988784, -0.372084409643}, kRho1, wt);
  admittance({7.72207355499, 1.22571337223}, kO1, wt);
  admittance({0.0917974187631, 0.0346666852856}, kTau1, wt);
  admittance({0.0216076132588, 0.0499001475428}, kBeta1, wt);
  admittance({0, 0}, kM13, wt);
  admittance({0.151023765145, 0.551338008051}, kM1, wt);
  admittance({0.0150734707365, 0.106061525993}, kChi1, wt);
  admittance({-0.100100088135, 0.140308589893}, kPi1, wt);
  admittance({-1.98971438869, 2.18905220309}, kP1, wt);
  admittance({0, 0}, kS1, wt);
  admittance({-7.94872283936, 5.2526679039}, kK1, wt);
  admittance({-0.0439319460782, 0.0216125871599}, kPsi1, wt);
  admittance({-0.12393619815, 0.0428195905118}, kPhi1, wt);
  admittance({-0.176421462816, -0.0948122579781}, kTheta1, wt);
  admittance({-0.990241370417, -0.649399034595}, kJ1, wt);
  admittance({-0.23220830358, -0.304973258533}, kSO1, wt);
  admittance({-0.799090351476, -1.13414306355}, kOO1, wt);
  admittance({-0.178800866348, -0.38166285634}, kUps1, wt);
  admittance({-0.586694887587, 0.638241937544}, kEps2, wt);
  admittance({-2.34171109573, 1.40479853767}, k2N2, wt);
  admittance({0, 0}, k2MS2, wt);
  admittance({-2.86559578342, 1.53506465835}, kMu2, wt);
  admittance({-18.6384963989, 3.00534558296}, kN2, wt);
  admittance({0, 0}, kN2P, wt);
  admittance({-3.53138974791, 0.380295598159}, kNu2, wt);
  admittance({-0.277164690769, -0.0488564447249}, kGamma2, wt);
  admittance({-0.315111399116, -0.0626023724529}, kAlpha2, wt);
  admittance({-90.5211105347, -20.603012085}, kM2, wt);
  admittance({-0.272239298733, -0.0698684018494}, kBeta2, wt);
  admittance({-0.100028710871, -0.0285822429918}, kDelta2, wt);
  admittance({0, 0}, kMKS2, wt);
  admittance({-0.561849456394, -0.320704261924}, kLambda2, wt);
  admittance({0, 0}, k2MN2, wt);
  admittance({-2.07392993428, -1.30093867249}, kL2, wt);
  admittance({0, 0}, kL2P, wt);
  admittance({-1.48842651628, -1.49241386152}, kT2, wt);
  admittance({-24.5810661316, -25.6641654968}, kS2, wt);
  admittance({-0.198384962578, -0.215614093522}, kR2, wt);
  admittance({-6.19152797309, -7.00377094904}, kK2, wt);
  admittance({0, 0}, kMSN2, wt);
  admittance({-0.163024886947, -0.328267289138}, kEta2, wt);
  admittance({0, 0}, k2SM2, wt);
  admittance({0, 0}, k2MK3, wt);
  admittance({0, 0}, kMO3, wt);
  admittance({0, 0}, kM3, wt);
  admittance({0, 0}, kMK3, wt);
  admittance({0, 0}, kN4, wt);
  admittance({0, 0}, kMN4, wt);
  admittance({0, 0}, kM4, wt);
  admittance({0, 0}, kSN4, wt);
  admittance({0, 0}, kMS4, wt);
  admittance({0, 0}, kMK4, wt);
  admittance({0, 0}, kS4, wt);
  admittance({0, 0}, kR4, wt);
  admittance({0, 0}, kSK4, wt);
  admittance({0, 0}, k2MN6, wt);
  admittance({0, 0}, kM6, wt);
  admittance({0, 0}, kMSN6, wt);
  admittance({0, 0}, k2MS6, wt);
  admittance({0, 0}, k2MK6, wt);
  admittance({0, 0}, k2SM6, wt);
  admittance({0, 0}, kMSK6, wt);
  admittance({0, 0}, kS6, wt);
  admittance({0, 0}, kM8, wt);
}

}  // namespace fes
