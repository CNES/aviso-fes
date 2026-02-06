// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/perth/wave_table.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "fes/interface/wave.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/perth/nodal_corrections.hpp"
#include "fes/perth/wave.hpp"

namespace fes {
namespace perth {

static auto wave_factory(ConstituentId ident)
    -> std::unique_ptr<WaveInterface> {
  switch (ident) {
    case kNode:
      return std::unique_ptr<WaveInterface>(new wave::Node());
    case kSa1:
      return std::unique_ptr<WaveInterface>(new wave::Sa1());
    case kSa:
      return std::unique_ptr<WaveInterface>(new wave::Sa());
    case kSsa:
      return std::unique_ptr<WaveInterface>(new wave::Ssa());
    case kSta:
      return std::unique_ptr<WaveInterface>(new wave::Sta());
    case kMSm:
      return std::unique_ptr<WaveInterface>(new wave::MSm());
    case kMm:
      return std::unique_ptr<WaveInterface>(new wave::Mm());
    case kMSf:
      return std::unique_ptr<WaveInterface>(new wave::MSf());
    case kMf:
      return std::unique_ptr<WaveInterface>(new wave::Mf());
    case kMStm:
      return std::unique_ptr<WaveInterface>(new wave::MStm());
    case kMtm:
      return std::unique_ptr<WaveInterface>(new wave::Mtm());
    case kMSqm:
      return std::unique_ptr<WaveInterface>(new wave::MSqm());
    case kMqm:
      return std::unique_ptr<WaveInterface>(new wave::Mqm());
    case k2Q1:
      return std::unique_ptr<WaveInterface>(new wave::_2Q1());
    case kSigma1:
      return std::unique_ptr<WaveInterface>(new wave::Sigma1());
    case kQ1:
      return std::unique_ptr<WaveInterface>(new wave::Q1());
    case kRho1:
      return std::unique_ptr<WaveInterface>(new wave::Rho1());
    case kO1:
      return std::unique_ptr<WaveInterface>(new wave::O1());
    case kTau1:
      return std::unique_ptr<WaveInterface>(new wave::Tau1());
    case kBeta1:
      return std::unique_ptr<WaveInterface>(new wave::Beta1());
    case kM13:
      return std::unique_ptr<WaveInterface>(new wave::M13());
    case kM1:
      return std::unique_ptr<WaveInterface>(new wave::M1());
    case kChi1:
      return std::unique_ptr<WaveInterface>(new wave::Chi1());
    case kPi1:
      return std::unique_ptr<WaveInterface>(new wave::Pi1());
    case kP1:
      return std::unique_ptr<WaveInterface>(new wave::P1());
    case kS1:
      return std::unique_ptr<WaveInterface>(new wave::S1());
    case kK1:
      return std::unique_ptr<WaveInterface>(new wave::K1());
    case kPsi1:
      return std::unique_ptr<WaveInterface>(new wave::Psi1());
    case kPhi1:
      return std::unique_ptr<WaveInterface>(new wave::Phi1());
    case kTheta1:
      return std::unique_ptr<WaveInterface>(new wave::Theta1());
    case kJ1:
      return std::unique_ptr<WaveInterface>(new wave::J1());
    case kSO1:
      return std::unique_ptr<WaveInterface>(new wave::SO1());
    case kOO1:
      return std::unique_ptr<WaveInterface>(new wave::OO1());
    case kUps1:
      return std::unique_ptr<WaveInterface>(new wave::Ups1());
    case kEps2:
      return std::unique_ptr<WaveInterface>(new wave::Eps2());
    case k2N2:
      return std::unique_ptr<WaveInterface>(new wave::_2N2());
    case k2MS2:
      return std::unique_ptr<WaveInterface>(new wave::_2MS2());
    case kMu2:
      return std::unique_ptr<WaveInterface>(new wave::Mu2());
    case kN2:
      return std::unique_ptr<WaveInterface>(new wave::N2());
    case kN2P:
      return std::unique_ptr<WaveInterface>(new wave::N2P());
    case kNu2:
      return std::unique_ptr<WaveInterface>(new wave::Nu2());
    case kGamma2:
      return std::unique_ptr<WaveInterface>(new wave::Gamma2());
    case kAlpha2:
      return std::unique_ptr<WaveInterface>(new wave::Alpha2());
    case kM2:
      return std::unique_ptr<WaveInterface>(new wave::M2());
    case kBeta2:
      return std::unique_ptr<WaveInterface>(new wave::Beta2());
    case kDelta2:
      return std::unique_ptr<WaveInterface>(new wave::Delta2());
    case kMKS2:
      return std::unique_ptr<WaveInterface>(new wave::MKS2());
    case kLambda2:
      return std::unique_ptr<WaveInterface>(new wave::Lambda2());
    case k2MN2:
      return std::unique_ptr<WaveInterface>(new wave::_2MN2());
    case kL2:
      return std::unique_ptr<WaveInterface>(new wave::L2());
    case kL2P:
      return std::unique_ptr<WaveInterface>(new wave::L2P());
    case kT2:
      return std::unique_ptr<WaveInterface>(new wave::T2());
    case kS2:
      return std::unique_ptr<WaveInterface>(new wave::S2());
    case kR2:
      return std::unique_ptr<WaveInterface>(new wave::R2());
    case kK2:
      return std::unique_ptr<WaveInterface>(new wave::K2());
    case kMSN2:
      return std::unique_ptr<WaveInterface>(new wave::MSN2());
    case kEta2:
      return std::unique_ptr<WaveInterface>(new wave::Eta2());
    case k2SM2:
      return std::unique_ptr<WaveInterface>(new wave::_2SM2());
    case k2MK3:
      return std::unique_ptr<WaveInterface>(new wave::_2MK3());
    case kMO3:
      return std::unique_ptr<WaveInterface>(new wave::MO3());
    case kM3:
      return std::unique_ptr<WaveInterface>(new wave::M3());
    case kMK3:
      return std::unique_ptr<WaveInterface>(new wave::MK3());
    case kN4:
      return std::unique_ptr<WaveInterface>(new wave::N4());
    case kMN4:
      return std::unique_ptr<WaveInterface>(new wave::MN4());
    case kM4:
      return std::unique_ptr<WaveInterface>(new wave::M4());
    case kSN4:
      return std::unique_ptr<WaveInterface>(new wave::SN4());
    case kMS4:
      return std::unique_ptr<WaveInterface>(new wave::MS4());
    case kMK4:
      return std::unique_ptr<WaveInterface>(new wave::MK4());
    case kS4:
      return std::unique_ptr<WaveInterface>(new wave::S4());
    case kR4:
      return std::unique_ptr<WaveInterface>(new wave::R4());
    case kSK4:
      return std::unique_ptr<WaveInterface>(new wave::SK4());
    case k2MN6:
      return std::unique_ptr<WaveInterface>(new wave::_2MN6());
    case kM6:
      return std::unique_ptr<WaveInterface>(new wave::M6());
    case kMSN6:
      return std::unique_ptr<WaveInterface>(new wave::MSN6());
    case k2MS6:
      return std::unique_ptr<WaveInterface>(new wave::_2MS6());
    case k2MK6:
      return std::unique_ptr<WaveInterface>(new wave::_2MK6());
    case k2SM6:
      return std::unique_ptr<WaveInterface>(new wave::_2SM6());
    case kMSK6:
      return std::unique_ptr<WaveInterface>(new wave::MSK6());
    case kS6:
      return std::unique_ptr<WaveInterface>(new wave::S6());
    case kM8:
      return std::unique_ptr<WaveInterface>(new wave::M8());
    default:
      throw std::invalid_argument("wave identifier not recognized: " +
                                  std::to_string(ident));
  }
}

// Builds the list of constituent identifiers from the optional list of names
static auto build_constituent_ids(const std::vector<std::string>& waves = {})
    -> std::vector<ConstituentId> {
  auto to_ids = [](const auto& names) {
    std::vector<ConstituentId> result;
    result.reserve(names.size());
    std::transform(names.begin(), names.end(), std::back_inserter(result),
                   perth::constituents::parse);
    return result;
  };

  return waves.empty() ? to_ids(perth::constituents::known()) : to_ids(waves);
}

WaveTable::WaveTable() : WaveTableInterface() {
  populate_map(build_constituent_ids(), wave_factory);
}

WaveTable::WaveTable(const std::vector<std::string>& names)
    : WaveTableInterface() {
  populate_map(build_constituent_ids(names), wave_factory);
}

auto WaveTable::compute_nodal_corrections(const angle::Astronomic& angles,
                                          const bool group_modulations)
    -> void {
  auto constituent_ids = this->constituent_ids();
  auto nodal_corrections = NodalCorrectionProcessor(
      NodalCorrectionsArgs(angles, group_modulations))(constituent_ids);

  for (size_t ix = 0; ix < constituent_ids.size(); ++ix) {
    auto& component = (*this)[constituent_ids[ix]];
    auto tidal_argument = calculate_doodson_argument(
        angles, component->doodson_numbers().template cast<double>());
    component->set_nodal_corrections(nodal_corrections[ix].f,
                                     nodal_corrections[ix].u, tidal_argument,
                                     AngleUnit::kDegree);
  }
}

}  // namespace perth
}  // namespace fes
