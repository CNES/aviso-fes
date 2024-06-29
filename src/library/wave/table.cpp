// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave/table.hpp"

#include <Eigen/Dense>
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"
#include "fes/detail/wave/name.hpp"

namespace fes {
namespace wave {

auto Table::wave_factory(const Constituent ident) -> std::shared_ptr<Wave> {
  switch (ident) {
    case kO1:
      return std::shared_ptr<Wave>(new wave::O1());
    case kP1:
      return std::shared_ptr<Wave>(new wave::P1());
    case kK1:
      return std::shared_ptr<Wave>(new wave::K1());
    case k2N2:
      return std::shared_ptr<Wave>(new wave::_2N2());
    case kMu2:
      return std::shared_ptr<Wave>(new wave::Mu2());
    case kN2:
      return std::shared_ptr<Wave>(new wave::N2());
    case kNu2:
      return std::shared_ptr<Wave>(new wave::Nu2());
    case kM2:
      return std::shared_ptr<Wave>(new wave::M2());
    case kL2:
      return std::shared_ptr<Wave>(new wave::L2());
    case kT2:
      return std::shared_ptr<Wave>(new wave::T2());
    case kS2:
      return std::shared_ptr<Wave>(new wave::S2());
    case kK2:
      return std::shared_ptr<Wave>(new wave::K2());
    case kM4:
      return std::shared_ptr<Wave>(new wave::M4());
    case kS1:
      return std::shared_ptr<Wave>(new wave::S1());
    case kQ1:
      return std::shared_ptr<Wave>(new wave::Q1());
    case kMm:
      return std::shared_ptr<Wave>(new wave::Mm());
    case kMf:
      return std::shared_ptr<Wave>(new wave::Mf());
    case kMtm:
      return std::shared_ptr<Wave>(new wave::Mtm());
    case kMsqm:
      return std::shared_ptr<Wave>(new wave::Msqm());
    case kEps2:
      return std::shared_ptr<Wave>(new wave::Eps2());
    case kLambda2:
      return std::shared_ptr<Wave>(new wave::Lambda2());
    case kEta2:
      return std::shared_ptr<Wave>(new wave::Eta2());
    case k2Q1:
      return std::shared_ptr<Wave>(new wave::_2Q1());
    case kSigma1:
      return std::shared_ptr<Wave>(new wave::Sigma1());
    case kRho1:
      return std::shared_ptr<Wave>(new wave::Rho1());
    case kM11:
      return std::shared_ptr<Wave>(new wave::M11());
    case kM12:
      return std::shared_ptr<Wave>(new wave::M12());
    case kChi1:
      return std::shared_ptr<Wave>(new wave::Chi1());
    case kPi1:
      return std::shared_ptr<Wave>(new wave::Pi1());
    case kPhi1:
      return std::shared_ptr<Wave>(new wave::Phi1());
    case kTheta1:
      return std::shared_ptr<Wave>(new wave::Theta1());
    case kJ1:
      return std::shared_ptr<Wave>(new wave::J1());
    case kOO1:
      return std::shared_ptr<Wave>(new wave::OO1());
    case kM3:
      return std::shared_ptr<Wave>(new wave::M3());
    case kM6:
      return std::shared_ptr<Wave>(new wave::M6());
    case kMN4:
      return std::shared_ptr<Wave>(new wave::MN4());
    case kMS4:
      return std::shared_ptr<Wave>(new wave::MS4());
    case kN4:
      return std::shared_ptr<Wave>(new wave::N4());
    case kR2:
      return std::shared_ptr<Wave>(new wave::R2());
    case kR4:
      return std::shared_ptr<Wave>(new wave::R4());
    case kS4:
      return std::shared_ptr<Wave>(new wave::S4());
    case kMNS2:
      return std::shared_ptr<Wave>(new wave::MNS2());
    case kM13:
      return std::shared_ptr<Wave>(new wave::M13());
    case kMK4:
      return std::shared_ptr<Wave>(new wave::MK4());
    case kSN4:
      return std::shared_ptr<Wave>(new wave::SN4());
    case kSK4:
      return std::shared_ptr<Wave>(new wave::SK4());
    case k2MN6:
      return std::shared_ptr<Wave>(new wave::_2MN6());
    case k2MS6:
      return std::shared_ptr<Wave>(new wave::_2MS6());
    case k2MK6:
      return std::shared_ptr<Wave>(new wave::_2MK6());
    case kMSN6:
      return std::shared_ptr<Wave>(new wave::MSN6());
    case k2SM6:
      return std::shared_ptr<Wave>(new wave::_2SM6());
    case kMSK6:
      return std::shared_ptr<Wave>(new wave::MSK6());
    case kMP1:
      return std::shared_ptr<Wave>(new wave::MP1());
    case k2SM2:
      return std::shared_ptr<Wave>(new wave::_2SM2());
    case kPsi1:
      return std::shared_ptr<Wave>(new wave::Psi1());
    case k2MS2:
      return std::shared_ptr<Wave>(new wave::_2MS2());
    case kMKS2:
      return std::shared_ptr<Wave>(new wave::MKS2());
    case k2MN2:
      return std::shared_ptr<Wave>(new wave::_2MN2());
    case kMSN2:
      return std::shared_ptr<Wave>(new wave::MSN2());
    case kMO3:
      return std::shared_ptr<Wave>(new wave::MO3());
    case k2MK3:
      return std::shared_ptr<Wave>(new wave::_2MK3());
    case kMK3:
      return std::shared_ptr<Wave>(new wave::MK3());
    case kS6:
      return std::shared_ptr<Wave>(new wave::S6());
    case kM8:
      return std::shared_ptr<Wave>(new wave::M8());
    case kMSf:
      return std::shared_ptr<Wave>(new wave::MSf());
    case kSsa:
      return std::shared_ptr<Wave>(new wave::Ssa());
    case kSa:
      return std::shared_ptr<Wave>(new wave::Sa());
    case kA5:
      return std::shared_ptr<Wave>(new wave::A5());
    case kSa1:
      return std::shared_ptr<Wave>(new wave::Sa1());
    case kSta:
      return std::shared_ptr<Wave>(new wave::Sta());
    case kMm2:
      return std::shared_ptr<Wave>(new wave::Mm2());
    case kMm1:
      return std::shared_ptr<Wave>(new wave::Mm1());
    case kMf1:
      return std::shared_ptr<Wave>(new wave::Mf1());
    case kMf2:
      return std::shared_ptr<Wave>(new wave::Mf2());
    case kM0:
      return std::shared_ptr<Wave>(new wave::M0());
    default:
      throw std::invalid_argument("wave identifier not recognized: " +
                                  std::to_string(ident));
  }
}

/// Create a table with all known constituents.
inline auto create_table(const std::vector<std::string>& known_constituents,
                         std::vector<std::shared_ptr<Wave>>& table) -> void {
  for (const auto& constituent : known_constituents) {
    table.emplace_back(Table::wave_factory(constituents::parse(constituent)));
  }
}

/// Create a table with the selected constituents.
inline auto create_sparse_table(
    const std::vector<std::string>& known_constituents,
    const std::vector<std::string>& waves,
    std::vector<std::shared_ptr<Wave>>& table) -> void {
  for (const auto& constituent : known_constituents) {
    auto selected =
        std::find(waves.begin(), waves.end(), constituent) != waves.end();
    table.emplace_back(
        selected ? Table::wave_factory(constituents::parse(constituent))
                 : nullptr);
  }
}

Table::Table(const std::vector<std::string>& waves) {
  const auto known_constituents = fes::constituents::known();
  waves_.reserve(known_constituents.size());
  waves.empty() ? create_table(known_constituents, waves_)
                : create_sparse_table(known_constituents, waves, waves_);
  getter_ =
      size() == waves_.size() ? &Table::direct_access : &Table::sparse_access;
}

Table::Table(const std::vector<Constituent>& waves) {
  auto wave_names = std::vector<std::string>{};
  wave_names.reserve(wave_names.size());
  for (const auto& constituent : waves) {
    wave_names.emplace_back(constituents::name(constituent));
  }
  *this = Table(wave_names);
}

void Table::admittance() {
  // Arrays who contains the spline coefficients needed to compute MU2, NU2,
  // L2, T2 and Lambda2 by admittance.
  constexpr auto mu2 =
      std::array<double, 3>{0.069439968323, 0.351535557706, -0.046278307672};
  constexpr auto nu2 =
      std::array<double, 3>{-0.006104695053, 0.156878802427, 0.006755704028};
  constexpr auto l2 =
      std::array<double, 3>{0.077137765667, -0.051653455134, 0.027869916824};
  constexpr auto t2 =
      std::array<double, 3>{0.180480173707, -0.020101177502, 0.008331518844};
  constexpr auto lda2 =
      std::array<double, 3>{0.016503557465, -0.013307812292, 0.007753383202};

  // infer additional constituents by admittance DIURNALS (from Richard Ray
  // perth2 program)

  // from Q1 and O1 (0-1)

  auto x = (*this)[kQ1];
  auto y = (*this)[kO1];
  auto z = (*this)[kK1];

  auto set_tide = [](std::shared_ptr<Wave>& wave,
                     const std::complex<double>& value) {
    if (wave->admittance()) {
      wave->tide(value);
    }
  };

  // 2Q1
  set_tide((*this)[k2Q1], 0.263 * x->tide() - 0.0252 * y->tide());

  // Sigma1
  set_tide((*this)[kSigma1], 0.297 * x->tide() - 0.0264 * y->tide());

  // rho1
  set_tide((*this)[kRho1], 0.164 * x->tide() + 0.0048 * y->tide());

  // from O1 and K1  (1-2)

  // M11
  set_tide((*this)[kM11], 0.0389 * y->tide() + 0.0282 * z->tide());

  // M12
  set_tide((*this)[kM12], 0.0140 * y->tide() + 0.0101 * z->tide());

  // CHI1
  set_tide((*this)[kChi1], 0.0064 * y->tide() + 0.0060 * z->tide());

  // pi1
  set_tide((*this)[kPi1], 0.0030 * y->tide() + 0.0171 * z->tide());

  // phi1
  set_tide((*this)[kPhi1], -0.0015 * y->tide() + 0.0152 * z->tide());

  // theta1
  set_tide((*this)[kTheta1], -0.0065 * y->tide() + 0.0155 * z->tide());

  // J1
  set_tide((*this)[kJ1], -0.0389 * y->tide() + 0.0836 * z->tide());

  // OO1
  set_tide((*this)[kOO1], -0.0431 * y->tide() + 0.0613 * z->tide());

  // infer additional constituents by admittance SEMI-DIURNALS
  // (from Richard Ray perth3 program)

  // from M2 - N2
  x = (*this)[kN2];
  y = (*this)[kM2];

  // 2N2
  set_tide((*this)[k2N2], 0.264 * x->tide() - 0.0253 * y->tide());

  // SEMI-DIURNAL (from Grenoble to take advantage of 2N2)

  // from 2N2 -N2 (3-4)
  x = (*this)[k2N2];
  y = (*this)[kN2];

  // eps2
  set_tide((*this)[kEps2], 0.53285 * x->tide() - 0.03304 * y->tide());

  // from M2 - K2 [5-6]
  x = (*this)[kN2];
  y = (*this)[kM2];
  z = (*this)[kK2];

  // eta2
  set_tide((*this)[kEta2], -0.0034925 * y->tide() + 0.0831707 * z->tide());

  // from N2 -M2- K2 by spline admittances [see GRL 18[5]:845-848,1991]

  // mu2
  set_tide((*this)[kMu2],
           mu2[0] * z->tide() + mu2[1] * x->tide() + mu2[2] * y->tide());
  // nu2
  set_tide((*this)[kNu2],
           nu2[0] * z->tide() + nu2[1] * x->tide() + nu2[2] * y->tide());
  // lambda2
  set_tide((*this)[kLambda2],
           lda2[0] * z->tide() + lda2[1] * x->tide() + lda2[2] * y->tide());
  // L2
  set_tide((*this)[kL2],
           l2[0] * z->tide() + l2[1] * x->tide() + l2[2] * y->tide());

  // T2
  set_tide((*this)[kT2],
           t2[0] * z->tide() + t2[1] * x->tide() + t2[2] * y->tide());
}

auto Table::harmonic_analysis(const Eigen::Ref<const Eigen::VectorXd>& h,
                              const Eigen::Ref<const Eigen::MatrixXd>& f,
                              const Eigen::Ref<const Eigen::MatrixXd>& vu)
    -> Eigen::VectorXcd {
  if (f.rows() != vu.rows() || f.cols() != vu.cols()) {
    throw std::invalid_argument(
        "f and vu could not be broadcast together with shape (" +
        std::to_string(f.rows()) + ", " + std::to_string(f.cols()) + ") (" +
        std::to_string(vu.rows()) + ", " + std::to_string(vu.cols()) + ")");
  }

  if (h.rows() != f.cols() || h.rows() != vu.cols()) {
    throw std::invalid_argument(
        "f, vu could not be broadcast with h with shape (" +
        std::to_string(f.rows()) + ", " + std::to_string(f.cols()) + ") (" +
        std::to_string(h.cols()) + ")");
  }
  auto w_size = f.rows();
  auto result = Eigen::VectorXcd(w_size);

  if (h.hasNaN()) {
    result.fill(std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN()));
    return result;
  }

  auto H = Eigen::MatrixXd(w_size << 1, h.rows());

  H.topRows(w_size) = f.array() * vu.array().cos();
  H.bottomRows(w_size) = f.array() * vu.array().sin();

  auto solution = ((H * H.transpose()).inverse() * H) * h;
  result.real() = solution.topRows(w_size);
  result.imag() = solution.bottomRows(w_size);

  return result;
}

auto Table::tide_from_tide_series(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const Eigen::Ref<const Eigen::VectorXcd>& wave,
    const angle::Formulae& formulae) const -> Eigen::VectorXd {
  detail::check_eigen_shape("epoch", epoch, "leap_seconds", leap_seconds);
  if (static_cast<size_t>(wave.rows()) != size()) {
    throw std::invalid_argument(
        "wave must contain as many elements as the number of waves in the "
        "table");
  }
  auto result = Eigen::VectorXd(epoch.rows());

  /// The object responsible for the calculation of astronomical angles.
  auto angles = angle::Astronomic(formulae);

  // The wave properties of the object must be immutable for the provided
  // instance.
  auto wt = Table(*this);

  for (auto ix = 0; ix < epoch.rows(); ++ix) {
    double tide = 0;
    angles.update(epoch(ix), leap_seconds(ix));
    wt.compute_nodal_corrections(angles);

    for (size_t jx = 0; jx < wt.size(); ++jx) {
      const auto& item = wt[static_cast<Constituent>(jx)];
      double phi = item->vu();

      tide += item->f() * (wave(jx).real() * std::cos(phi) +
                           wave(jx).imag() * std::sin(phi));
    }
    result(ix) = tide;
  }
  return result;
}

auto Table::tide_from_mapping(const double epoch, const uint16_t leap_seconds,
                              const Eigen::Ref<const Eigen::MatrixXcd>& wave,
                              const angle::Formulae& formulae,
                              const size_t num_threads) const
    -> Eigen::MatrixXd {
  if (static_cast<size_t>(wave.rows()) != size()) {
    throw std::invalid_argument(
        "wave must contain as many elements as the number of waves in the "
        "table");
  }
  auto result =
      Eigen::MatrixXd(Eigen::MatrixXd::Zero(wave.cols(), wave.rows()));
  auto worker = [&](const int64_t start, const int64_t end) {
    // The wave properties of the object must be immutable for the provided
    // instance.
    auto wt = Table(*this);
    wt.compute_nodal_corrections(
        angle::Astronomic(formulae, epoch, leap_seconds));

    for (auto ix = start; ix < end; ++ix) {
      for (size_t jx = 0; jx < wt.size(); ++jx) {
        const auto& item = wt[static_cast<Constituent>(jx)];
        double phi = item->vu();

        result(ix, jx) += item->f() * (wave(jx, ix).real() * std::cos(phi) +
                                       wave(jx, ix).imag() * std::sin(phi));
      }
    }
  };
  detail::parallel_for(worker, wave.cols(), num_threads);
  return result;
}

auto Table::compute_nodal_modulations(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const angle::Formulae& formulae) const
    -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> {
  detail::check_eigen_shape("epoch", epoch, "leap_seconds", leap_seconds);
  auto f = Eigen::MatrixXd(size(), epoch.size());
  auto vu = Eigen::MatrixXd(size(), epoch.size());

  /// The object responsible for the calculation of astronomical angles.
  auto angles = angle::Astronomic(formulae);

  // The wave properties of the object must be immutable for the provided
  // instance.
  auto wt = Table(*this);

  for (auto ix = 0; ix < epoch.size(); ++ix) {
    angles.update(epoch(ix), leap_seconds(ix));
    wt.compute_nodal_corrections(angles);

    for (std::size_t jx = 0; jx < wt.size(); ++jx) {
      const auto& wave = wt[static_cast<Constituent>(jx)];
      f(jx, ix) = wave->f();
      vu(jx, ix) = wave->vu();
    }
  }
  return std::make_tuple(f, vu);
}

auto Table::select_waves_for_analysis(const double duration, const double f)
    -> std::vector<std::string> {
  auto result = std::vector<std::string>();
  for (auto&& wave : Table()) {
    if (wave->period() < f * (duration / 3600.0)) {
      result.emplace_back(wave->name());
    }
  }
  return result;
}

}  // namespace wave
}  // namespace fes
