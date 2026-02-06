// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/wave_table.hpp"

#include <memory>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"
#include "fes/darwin/constituent.hpp"
#include "fes/darwin/wave.hpp"
#include "fes/interface/wave.hpp"
#include "fes/interface/wave_table.hpp"

namespace fes {
namespace darwin {

static auto wave_factory(ConstituentId ident)
    -> std::unique_ptr<WaveInterface> {
  switch (ident) {
    case kO1:
      return std::unique_ptr<WaveInterface>(new wave::O1());
    case kP1:
      return std::unique_ptr<WaveInterface>(new wave::P1());
    case kK1:
      return std::unique_ptr<WaveInterface>(new wave::K1());
    case k2N2:
      return std::unique_ptr<WaveInterface>(new wave::_2N2());
    case kMu2:
      return std::unique_ptr<WaveInterface>(new wave::Mu2());
    case kN2:
      return std::unique_ptr<WaveInterface>(new wave::N2());
    case kNu2:
      return std::unique_ptr<WaveInterface>(new wave::Nu2());
    case kM2:
      return std::unique_ptr<WaveInterface>(new wave::M2());
    case kL2:
      return std::unique_ptr<WaveInterface>(new wave::L2());
    case kT2:
      return std::unique_ptr<WaveInterface>(new wave::T2());
    case kS2:
      return std::unique_ptr<WaveInterface>(new wave::S2());
    case kK2:
      return std::unique_ptr<WaveInterface>(new wave::K2());
    case kM4:
      return std::unique_ptr<WaveInterface>(new wave::M4());
    case kS1:
      return std::unique_ptr<WaveInterface>(new wave::S1());
    case kQ1:
      return std::unique_ptr<WaveInterface>(new wave::Q1());
    case kMm:
      return std::unique_ptr<WaveInterface>(new wave::Mm());
    case kMf:
      return std::unique_ptr<WaveInterface>(new wave::Mf());
    case kMtm:
      return std::unique_ptr<WaveInterface>(new wave::Mtm());
    case kMSqm:
      return std::unique_ptr<WaveInterface>(new wave::MSqm());
    case kEps2:
      return std::unique_ptr<WaveInterface>(new wave::Eps2());
    case kLambda2:
      return std::unique_ptr<WaveInterface>(new wave::Lambda2());
    case kEta2:
      return std::unique_ptr<WaveInterface>(new wave::Eta2());
    case k2Q1:
      return std::unique_ptr<WaveInterface>(new wave::_2Q1());
    case kSigma1:
      return std::unique_ptr<WaveInterface>(new wave::Sigma1());
    case kRho1:
      return std::unique_ptr<WaveInterface>(new wave::Rho1());
    case kM1:
      return std::unique_ptr<WaveInterface>(new wave::M1());
    case kM11:
      return std::unique_ptr<WaveInterface>(new wave::M11());
    case kM12:
      return std::unique_ptr<WaveInterface>(new wave::M12());
    case kM13:
      return std::unique_ptr<WaveInterface>(new wave::M13());
    case kChi1:
      return std::unique_ptr<WaveInterface>(new wave::Chi1());
    case kPi1:
      return std::unique_ptr<WaveInterface>(new wave::Pi1());
    case kPhi1:
      return std::unique_ptr<WaveInterface>(new wave::Phi1());
    case kTheta1:
      return std::unique_ptr<WaveInterface>(new wave::Theta1());
    case kJ1:
      return std::unique_ptr<WaveInterface>(new wave::J1());
    case kOO1:
      return std::unique_ptr<WaveInterface>(new wave::OO1());
    case kM3:
      return std::unique_ptr<WaveInterface>(new wave::M3());
    case kM6:
      return std::unique_ptr<WaveInterface>(new wave::M6());
    case kMN4:
      return std::unique_ptr<WaveInterface>(new wave::MN4());
    case kMS4:
      return std::unique_ptr<WaveInterface>(new wave::MS4());
    case kN4:
      return std::unique_ptr<WaveInterface>(new wave::N4());
    case kR2:
      return std::unique_ptr<WaveInterface>(new wave::R2());
    case kR4:
      return std::unique_ptr<WaveInterface>(new wave::R4());
    case kS4:
      return std::unique_ptr<WaveInterface>(new wave::S4());
    case kMNS2:
      return std::unique_ptr<WaveInterface>(new wave::MNS2());
    case kMK4:
      return std::unique_ptr<WaveInterface>(new wave::MK4());
    case kSN4:
      return std::unique_ptr<WaveInterface>(new wave::SN4());
    case kSK4:
      return std::unique_ptr<WaveInterface>(new wave::SK4());
    case k2MN6:
      return std::unique_ptr<WaveInterface>(new wave::_2MN6());
    case k2MS6:
      return std::unique_ptr<WaveInterface>(new wave::_2MS6());
    case k2MK6:
      return std::unique_ptr<WaveInterface>(new wave::_2MK6());
    case kMSN6:
      return std::unique_ptr<WaveInterface>(new wave::MSN6());
    case k2SM6:
      return std::unique_ptr<WaveInterface>(new wave::_2SM6());
    case kMSK6:
      return std::unique_ptr<WaveInterface>(new wave::MSK6());
    case kMP1:
      return std::unique_ptr<WaveInterface>(new wave::MP1());
    case k2SM2:
      return std::unique_ptr<WaveInterface>(new wave::_2SM2());
    case kPsi1:
      return std::unique_ptr<WaveInterface>(new wave::Psi1());
    case k2MS2:
      return std::unique_ptr<WaveInterface>(new wave::_2MS2());
    case kMKS2:
      return std::unique_ptr<WaveInterface>(new wave::MKS2());
    case k2MN2:
      return std::unique_ptr<WaveInterface>(new wave::_2MN2());
    case kMSN2:
      return std::unique_ptr<WaveInterface>(new wave::MSN2());
    case kMO3:
      return std::unique_ptr<WaveInterface>(new wave::MO3());
    case k2MK3:
      return std::unique_ptr<WaveInterface>(new wave::_2MK3());
    case kMK3:
      return std::unique_ptr<WaveInterface>(new wave::MK3());
    case kS6:
      return std::unique_ptr<WaveInterface>(new wave::S6());
    case kM8:
      return std::unique_ptr<WaveInterface>(new wave::M8());
    case kMSf:
      return std::unique_ptr<WaveInterface>(new wave::MSf());
    case kSsa:
      return std::unique_ptr<WaveInterface>(new wave::Ssa());
    case kSa:
      return std::unique_ptr<WaveInterface>(new wave::Sa());
    case kA5:
      return std::unique_ptr<WaveInterface>(new wave::A5());
    case kSa1:
      return std::unique_ptr<WaveInterface>(new wave::Sa1());
    case kSta:
      return std::unique_ptr<WaveInterface>(new wave::Sta());
    case kMm2:
      return std::unique_ptr<WaveInterface>(new wave::Mm2());
    case kMm1:
      return std::unique_ptr<WaveInterface>(new wave::Mm1());
    case kMf1:
      return std::unique_ptr<WaveInterface>(new wave::Mf1());
    case kMf2:
      return std::unique_ptr<WaveInterface>(new wave::Mf2());
    case kM0:
      return std::unique_ptr<WaveInterface>(new wave::M0());
    case kL2P:
      return std::unique_ptr<WaveInterface>(new wave::L2P());
    case kN2P:
      return std::unique_ptr<WaveInterface>(new wave::N2P());
    case kMSK2:
      return std::unique_ptr<WaveInterface>(new wave::MSK2());
    case kSKM2:
      return std::unique_ptr<WaveInterface>(new wave::SKM2());
    case kOQ2:
      return std::unique_ptr<WaveInterface>(new wave::OQ2());
    case k3MS4:
      return std::unique_ptr<WaveInterface>(new wave::_3MS4());
    case kMNu4:
      return std::unique_ptr<WaveInterface>(new wave::MNu4());
    case k2MSN4:
      return std::unique_ptr<WaveInterface>(new wave::_2MSN4());
    case k2NS2:
      return std::unique_ptr<WaveInterface>(new wave::_2NS2());
    case kMNuS2:
      return std::unique_ptr<WaveInterface>(new wave::MNuS2());
    case k2MK2:
      return std::unique_ptr<WaveInterface>(new wave::_2MK2());
    case kNKM2:
      return std::unique_ptr<WaveInterface>(new wave::NKM2());
    case kML4:
      return std::unique_ptr<WaveInterface>(new wave::ML4());
    case kSO1:
      return std::unique_ptr<WaveInterface>(new wave::SO1());
    case kSO3:
      return std::unique_ptr<WaveInterface>(new wave::SO3());
    case kNK4:
      return std::unique_ptr<WaveInterface>(new wave::NK4());
    case kMNK6:
      return std::unique_ptr<WaveInterface>(new wave::MNK6());
    case k2NM6:
      return std::unique_ptr<WaveInterface>(new wave::_2NM6());
    case k3MS8:
      return std::unique_ptr<WaveInterface>(new wave::_3MS8());
    case kSK3:
      return std::unique_ptr<WaveInterface>(new wave::SK3());
    case k2MNS4:
      return std::unique_ptr<WaveInterface>(new wave::_2MNS4());
    case k2SMu2:
      return std::unique_ptr<WaveInterface>(new wave::_2SMu2());
    case k2MP5:
      return std::unique_ptr<WaveInterface>(new wave::_2MP5());
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
                   darwin::constituents::parse);
    return result;
  };

  return waves.empty() ? to_ids(darwin::constituents::known()) : to_ids(waves);
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
  for (auto& item : *this) {
    item.value()->compute_nodal_corrections(angles, group_modulations);
  }
}

}  // namespace darwin
}  // namespace fes
