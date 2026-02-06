#include "fes/perth/nodal_corrections.hpp"

#include <cmath>

#include "fes/detail/math.hpp"
#include "fes/perth/doodson.hpp"

namespace fes {
namespace perth {

using detail::math::degrees;
using detail::math::pow;
using detail::math::radians;

auto evaluate_nodal_corrections(double omega, double p,
                                const std::vector<ConstituentId> &constituents)
    -> std::vector<NodalCorrections> {
  const auto sinn = std::sin(radians(omega));
  const auto cosn = std::cos(radians(omega));
  const auto sin2n = std::sin(2.0 * radians(omega));
  const auto cos2n = std::cos(2.0 * radians(omega));
  // const auto sin3n = std::sin(3.0 * radians(omega));
  // const auto sinp = std::sin(radians(p));
  // const auto cosp = std::cos(radians(p));
  const auto sin2p = std::sin(2.0 * radians(p));
  const auto cos2p = std::cos(2.0 * radians(p));

  // Compute standard nodal corrections f and u
  std::vector<NodalCorrections> corrections(constituents.size());

  for (size_t ix = 0; ix < constituents.size(); ++ix) {
    const auto &constituent = constituents[ix];
    auto &correction = corrections[ix];
    double term1;
    double term2;

    switch (constituent) {
      case kMm:
      case kMSm:
        term1 = -0.0534 * sin2p - 0.0219 * std::sin((radians(2 * p - omega)));
        term2 = 1.0 - 0.1308 * cosn - 0.0534 * cos2p -
                0.0219 * std::cos((radians(2 * p - omega)));
        break;
      case kMf:
      case kMSqm:
        //   case kMSp:
      case kMqm:
        term1 = -0.04324 * sin2p - 0.41465 * sinn - 0.03873 * sin2n;
        term2 = 1.0 + 0.04324 * cos2p + 0.41465 * cosn + 0.03873 * cos2n;
        break;
      case kMSf:  // This is linear tide, not compound.
        term1 = 0.137 * sinn;
        term2 = 1.0;
        break;
      case kMtm:
        term1 = -0.018 * sin2p - 0.4145 * sinn - 0.040 * sin2n;
        term2 = 1.0 + 0.018 * cos2p + 0.4145 * cosn + 0.040 * cos2n;
        break;
      case kMStm:
        term1 = -0.380 * sin2p - 0.413 * sinn - 0.037 * sin2n;
        term2 = 1.0 + 0.380 * cos2p + 0.413 * cosn + 0.037 * cos2n;
        break;
      case kO1:
        term1 = 0.1886 * sinn - 0.0058 * sin2n - 0.0065 * sin2p;
        term2 = 1.0 + 0.1886 * cosn - 0.0058 * cos2n - 0.0065 * cos2p;
        break;
      case k2Q1:
      case kQ1:
      case kRho1:
      case kSigma1:
        term1 = 0.1886 * sinn;
        term2 = 1.0 + 0.1886 * cosn;
        break;
      case kTau1:
        term1 = 0.219 * sinn;
        term2 = 1.0 - 0.219 * cosn;
        break;
      case kBeta1:
        term1 = 0.226 * sinn;
        term2 = 1.0 + 0.226 * cosn;
        break;
      case kM1:  // This assumes M1 argument includes p.
        term1 = -0.2294 * sinn - 0.3594 * sin2p -
                0.0664 * std::sin((radians(2 * p - omega)));
        term2 = 1.0 + 0.1722 * cosn + 0.3594 * cos2p +
                0.0664 * std::cos((radians(2 * p - omega)));
        break;
      case kChi1:
        term1 = -0.250 * sinn;
        term2 = 1.0 + 0.193 * cosn;
        break;
      case kP1:
        term1 = -0.0112 * sinn;
        term2 = 1.0 - 0.0112 * cosn;
        break;
      case kK1:
        term1 = -0.1554 * sinn + 0.0031 * sin2n;
        term2 = 1.0 + 0.1158 * cosn - 0.0028 * cos2n;
        break;
      case kJ1:
      case kTheta1:
        term1 = -0.227 * sinn;
        term2 = 1.0 + 0.169 * cosn;
        break;
      case kOO1:
      case kUps1:
        term1 = -0.640 * sinn - 0.134 * sin2n - 0.150 * sin2p;
        term2 = 1.0 + 0.640 * cosn + 0.134 * cos2n + 0.150 * cos2p;
        break;
      case kM2:
      case k2N2:
      case kMu2:
      case kN2:
      case kNu2:
      case kLambda2:
      case kMS4:
      case kEps2:
      case kSN4:
      case k2SM6:
        term1 = -0.03731 * sinn + 0.00052 * sin2n;
        term2 = 1.0 - 0.03731 * cosn + 0.00052 * cos2n;
        break;
      case kN2P:
        term1 = 0.1705 * sinn - 0.0035 * sin2n - 0.0176 * sin2p;
        term2 = 1.0 + 0.1705 * cosn - 0.0035 * cos2n - 0.0176 * cos2p;
        break;
      case kL2:
        term1 = -0.250 * sin2p - 0.110 * std::sin((radians(2 * p - omega))) -
                0.037 * sinn;
        term2 = 1.0 - 0.250 * cos2p -
                0.110 * std::cos((radians(2 * p - omega))) - 0.037 * cosn;
        break;
      case kL2P:
        term1 = -0.2495 * sinn;
        term2 = 1.0 + 0.1315 * cosn;
        break;
      case kK2:
      case kSK4:
        term1 = -0.3108 * sinn - 0.0324 * sin2n;
        term2 = 1.0 + 0.2853 * cosn + 0.0324 * cos2n;
        break;
      case kGamma2:
        term1 = 0.147 * std::sin((radians(2 * (omega - p))));
        term2 = 1.0 + 0.147 * std::cos((radians(2 * (omega - p))));
        break;
      case kDelta2:
        term1 = 0.505 * sin2p + 0.505 * sinn - 0.165 * sin2n;
        term2 = 1.0 - 0.505 * cos2p - 0.505 * cosn + 0.165 * cos2n;
        break;
      case kEta2:
        term1 = -0.436 * sinn;
        term2 = 1.0 + 0.436 * cosn;
        break;
      case kS2:
        term1 = 0.00225 * sinn;
        term2 = 1.0 + 0.00225 * cosn;
        break;
      case kM3:
        // Linear 3rd-degree terms
        term1 = -0.05644 * sinn;
        term2 = 1.0 - 0.05644 * cosn;
        break;
      case kM13:
        // Linear 3rd-degree terms
        term1 = -0.01815 * sinn;
        term2 = 1.0 - 0.27837 * cosn;
        break;
      default:
        // For all other constituents, use default values.
        term1 = 0.;
        term2 = 1.;
        break;
    }

    // Calculate f and u
    correction.f = std::sqrt(term1 * term1 + term2 * term2);
    correction.u = degrees(std::atan2(term1, term2));

    // Following tides are all compound & use recursion
    if (term1 == 0.0) {
      NodalCorrections nodal_offset;
      std::vector<NodalCorrections> nodal_corrections;

      switch (constituent) {
        case kSO1:
          nodal_offset = evaluate_nodal_correction(omega, p, kO1);
          correction.f = nodal_offset.f;
          correction.u = -nodal_offset.u;
          break;
        case kM4:
        case kMN4:
        case kN4:
        case k2MS2:
        case kMSN6:
        case k2MS6:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = pow<2>(nodal_offset.f);
          correction.u = 2.0 * nodal_offset.u;
          break;
        case kMSN2:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = pow<2>(nodal_offset.f);
          correction.u = 0;
          break;
        case k2MN2:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = pow<3>(nodal_offset.f);
          correction.u = nodal_offset.u;
          break;
        case k2SM2:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = nodal_offset.f;
          correction.u = -nodal_offset.u;
          break;
        case kM6:
        case k2MN6:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = pow<3>(nodal_offset.f);
          correction.u = 3.0 * nodal_offset.u;
          break;
        case kM8:
          nodal_offset = evaluate_nodal_correction(omega, p, kM2);
          correction.f = pow<4>(nodal_offset.f);
          correction.u = 4.0 * nodal_offset.u;
          break;
        case kMK4:
        case kMKS2:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kK2});
          correction.f = nodal_corrections[0].f * nodal_corrections[1].f;
          correction.u = nodal_corrections[0].u + nodal_corrections[1].u;
          break;
        case kMSK6:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kK2});
          correction.f = nodal_corrections[0].f * nodal_corrections[1].f;
          correction.u = nodal_corrections[0].u - nodal_corrections[1].u;
          break;
        case k2MK6:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kK2});
          correction.f =
              pow<2>(nodal_corrections[0].f) * nodal_corrections[1].f;
          correction.u = 2.0 * nodal_corrections[0].u + nodal_corrections[1].u;
          break;
        case kMO3:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kO1});
          correction.f = nodal_corrections[0].f * nodal_corrections[1].f;
          correction.u = nodal_corrections[0].u + nodal_corrections[1].u;
          break;
        case kMK3:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kK1});
          correction.f = nodal_corrections[0].f * nodal_corrections[1].f;
          correction.u = nodal_corrections[0].u + nodal_corrections[1].u;
          break;
        case k2MK3:
          nodal_corrections = evaluate_nodal_corrections(omega, p, {kM2, kK1});
          correction.f =
              pow<2>(nodal_corrections[0].f) * nodal_corrections[1].f;
          correction.u = 2.0 * nodal_corrections[0].u - nodal_corrections[1].u;
          break;
        default:
          break;  // No further corrections needed
      }
    }
  }
  return corrections;
}

auto evaluate_nodal_corrections(double perihelion, double omega, double perigee,
                                double hsolar,
                                const std::vector<ConstituentId> &constituents)
    -> std::vector<NodalCorrections> {
  auto h = radians(hsolar);
  auto p = radians(perigee);
  auto o = -radians(omega);
  auto pp = radians(perihelion);

  // Compute corrections f and u
  std::vector<NodalCorrections> corrections(constituents.size());
  for (size_t ix = 0; ix < constituents.size(); ++ix) {
    const auto &constituent = constituents[ix];
    auto &correction = corrections[ix];

    double term1;
    double term2;

    switch (constituent) {
      case kMm:
        term1 = -0.0137 * std::sin(-2.0 * h + 2.0 * p - o) +
                0.1912 * std::sin(-2.0 * h + 2.0 * p) -
                0.0125 * std::sin(-2.0 * h + 2.0 * p + o) -
                0.0657 * std::sin(-o) - 0.0653 * std::sin(o) -
                0.0534 * std::sin(2.0 * p) - 0.0219 * std::sin(2.0 * p + o) -
                0.0139 * std::sin(2.0 * h);
        term2 = 1.0 + 0.0137 * std::cos(2.0 * h - 2.0 * p - o) +
                0.1912 * std::cos(-2.0 * h + 2.0 * p) -
                0.0125 * std::cos(-2.0 * h + 2.0 * p + o) -
                0.1309 * std::cos(o) - 0.0534 * std::cos(2.0 * p) -
                0.0219 * std::cos(2.0 * p + o) - 0.0139 * std::cos(2.0 * h);
        break;
      case kMf:
        term1 = 0.0875 * std::sin(-2.0 * h) + 0.0432 * std::sin(-2.0 * p) +
                0.4145 * std::sin(o) + 0.0387 * std::sin(2.0 * o);
        term2 = 1.0 + 0.0875 * std::cos(2.0 * h) + 0.0432 * std::cos(2.0 * p) +
                0.4145 * std::cos(o) + 0.0387 * std::cos(2.0 * o);
        break;
      case kMtm:
        term1 = 0.0721 * std::sin(-2.0 * h) +
                0.1897 * std::sin(-2.0 * h + 2.0 * p) +
                0.0784 * std::sin(-2.0 * h + 2.0 * p + o) +
                0.4146 * std::sin(o);
        term2 = 1.0 + 0.0721 * std::cos(2.0 * h) +
                0.1897 * std::cos(-2.0 * h + 2.0 * p) +
                0.0784 * std::cos(-2.0 * h + 2.0 * p + o) +
                0.4146 * std::cos(o);
        break;
      case kMqm:
        term1 = 1.207 * std::sin(-2.0 * h + 2.0 * p) +
                0.497 * std::sin(-2.0 * h + 2.0 * p + o) + 0.414 * std::sin(o);
        term2 = 1.0 + 1.207 * std::cos(-2.0 * h + 2.0 * p) +
                0.497 * std::cos(-2.0 * h + 2.0 * p + o) + 0.414 * std::cos(o);
        break;
      case k2Q1:
        term1 = 0.1886 * std::sin(-o) +
                0.2274 * std::sin(2.0 * h - 2.0 * p - o) +
                1.2086 * std::sin(2.0 * h - 2.0 * p);
        term2 = 1.0 + 0.1886 * std::cos(o) +
                0.2274 * std::cos(2.0 * h - 2.0 * p - o) +
                1.2086 * std::cos(2.0 * h - 2.0 * p);
        break;
      case kSigma1:
        term1 = 0.1561 * std::sin(-2.0 * h + 2.0 * p - o) -
                0.1882 * std::sin(o) + 0.7979 * std::sin(-2.0 * h + 2.0 * p) +
                0.0815 * std::sin(h - pp);
        term2 = 1.0 + 0.1561 * std::cos(-2.0 * h + 2.0 * p - o) +
                0.1882 * std::cos(o) + 0.8569 * std::cos(-2.0 * h + 2.0 * p) +
                0.0538 * std::cos(h - pp);
        break;
      case kQ1:
        term1 = 0.1886 * std::sin(-o) +
                0.0359 * std::sin(2.0 * h - 2.0 * p - o) +
                0.1901 * std::sin(2.0 * h - 2.0 * p);
        term2 = 1.0 + 0.1886 * std::cos(o) +
                0.0359 * std::cos(2.0 * h - 2.0 * p - o) +
                0.1901 * std::cos(2.0 * h - 2.0 * p);
        break;
      case kO1:
        term1 = -0.0058 * std::sin(-2.0 * o) + 0.1886 * std::sin(-o) -
                0.0065 * std::sin(2.0 * p) - 0.0131 * std::sin(2.0 * h);
        term2 = 1.0 - 0.0058 * std::cos(2.0 * o) + 0.1886 * std::cos(o) -
                0.0065 * std::cos(2.0 * p) - 0.0131 * std::cos(2.0 * h);
        break;
      case kM1:  // Note: central line is 155.655
        term1 = 0.0941 * std::sin(-2.0 * h) + 0.0664 * std::sin(-2.0 * p - o) +
                0.3594 * std::sin(-2.0 * p) + 0.2008 * std::sin(o) +
                0.1910 * std::sin(2.0 * h - 2.0 * p) +
                0.0422 * std::sin(2.0 * h - 2.0 * p + o);
        term2 = 1.0 + 0.0941 * std::cos(2.0 * h) +
                0.0664 * std::cos(2.0 * p + o) + 0.3594 * std::cos(2.0 * p) +
                0.2008 * std::cos(o) + 0.1910 * std::cos(2.0 * h - 2.0 * p) +
                0.0422 * std::cos(2.0 * h - 2.0 * p + o);
        break;
      case kK1:
        term1 = -0.0184 * std::sin(-3.0 * h + pp) +
                0.0036 * std::sin(-2.0 * h - o) + 0.3166 * std::sin(2.0 * h) -
                0.0026 * std::sin(h + pp) + 0.0075 * std::sin(-h + pp) +
                0.1558 * std::sin(o) - 0.0030 * std::sin(2.0 * o) +
                0.0049 * std::sin(h - pp) + 0.0128 * std::sin(2.0 * h);
        term2 = 1.0 - 0.0184 * std::cos(-3.0 * h + pp) +
                0.0036 * std::cos(2.0 * h + o) - 0.3166 * std::cos(2.0 * h) +
                0.0026 * std::cos(h + pp) + 0.0075 * std::cos(h - pp) +
                0.1164 * std::cos(o) - 0.0030 * std::cos(2.0 * o) +
                0.0049 * std::cos(h - pp) + 0.0128 * std::cos(2.0 * h);
        break;
      case kJ1:
        term1 = 0.1922 * std::sin(-2.0 * h + 2.0 * p) +
                0.0378 * std::sin(-2.0 * h + 2.0 * p + o) +
                0.2268 * std::sin(o) - 0.0155 * std::sin(2.0 * p);
        term2 = 1.0 + 0.1922 * std::cos(-2.0 * h + 2.0 * p) +
                0.0378 * std::cos(-2.0 * h + 2.0 * p + o) +
                0.1701 * std::cos(o) - 0.0155 * std::cos(2.0 * p);
        break;
      case kOO1:
        term1 = 0.3029 * std::sin(-2.0 * h) + 0.0593 * std::sin(-2.0 * h + o) +
                0.1497 * std::sin(-2.0 * p) + 0.6404 * std::sin(o) +
                0.1337 * std::sin(2.0 * o);
        term2 = 1.0 + 0.3029 * std::cos(-2.0 * h) +
                0.0593 * std::cos(-2.0 * h + o) + 0.1497 * std::cos(-2.0 * p) +
                0.6404 * std::cos(o) + 0.1337 * std::cos(2.0 * o);
        break;
      case kEps2:
        term1 = 0.385 * std::sin(-2.0 * h + 2.0 * p);
        term2 = 1.0 + 0.385 * std::cos(-2.0 * h + 2.0 * p);
        break;
      case k2N2:  // Better is mu2 unless 2MS2 active
        term1 = 0.0374 * std::sin(o) + 1.2064 * std::sin(2.0 * h - 2.0 * p) -
                0.0139 * std::sin(-h + pp) -
                0.0170 * std::sin(h - 2.0 * p + pp) - 0.0104 * std::sin(h - p) +
                0.0156 * std::sin(h - pp) -
                0.0448 * std::sin(2.0 * h - 2.0 * p - o) +
                0.0808 * std::sin(3.0 * h - 2.0 * p - 4.939) +
                0.0369 * std::sin(4.0 * h - 4.0 * p);
        term2 =
            1.0 - 0.0374 * std::cos(o) + 1.2064 * std::cos(2.0 * h - 2.0 * p) -
            0.0139 * std::cos(-h + pp) - 0.0170 * std::cos(h - 2.0 * p + pp) -
            0.0104 * std::cos(h - p) + 0.0156 * std::cos(h - pp) -
            0.0448 * std::cos(2.0 * h - 2.0 * p - o) +
            0.0808 * std::cos(3.0 * h - 2.0 * p - 4.939) +
            0.0369 * std::cos(4.0 * h - 4.0 * p);
        break;
      case kMu2:
        term1 = -0.0115 * std::sin(-3.0 * h + 2.0 * p + pp) -
                0.0310 * std::sin(-2.0 * h + 2.0 * p - o) +
                0.8289 * std::sin(-2.0 * h + 2.0 * p) -
                0.0140 * std::sin(-h + pp) - 0.0086 * std::sin(-h + p) +
                0.0130 * std::sin(-h + 2.0 * p - pp) + 0.0371 * std::sin(o) +
                0.0670 * std::sin(h - pp) +
                0.0306 * std::sin(2.0 * h - 2.0 * p);
        term2 = 1.0 - 0.0115 * std::cos(-3.0 * h + 2.0 * p + pp) -
                0.0310 * std::cos(-2.0 * h + 2.0 * p - o) +
                0.8289 * std::cos(-2.0 * h + 2.0 * p) -
                0.0140 * std::cos(-h + pp) - 0.0086 * std::cos(-h + p) +
                0.0130 * std::cos(-h + 2.0 * p - pp) - 0.0371 * std::cos(o) +
                0.0670 * std::cos(h - pp) +
                0.0306 * std::cos(2.0 * h - 2.0 * p);
        break;
      case kN2:
        term1 = -0.0084 * std::sin(-h + pp) - 0.0373 * std::sin(-o) +
                0.0093 * std::sin(h - pp) +
                0.1899 * std::sin(2.0 * h - 2.0 * p) -
                0.0071 * std::sin(2.0 * h - 2.0 * p - o);
        term2 = 1.0 - 0.0084 * std::cos(-h + pp) - 0.0373 * std::cos(o) +
                0.0093 * std::cos(h - pp) +
                0.1899 * std::cos(2.0 * h - 2.0 * p) -
                0.0071 * std::cos(2.0 * h - 2.0 * p - o);
        break;
      case kM2:
        term1 = -0.0030 * std::sin(-2.0 * h + 2.0 * p) - 0.0373 * std::sin(-o) +
                0.0065 * std::sin(h - pp) + 0.0011 * std::sin(2.0 * h);
        term2 = 1.0 - 0.0030 * std::cos(-2.0 * h + 2.0 * p) -
                0.0373 * std::cos(o) - 0.0004 * std::cos(h - pp) +
                0.0011 * std::cos(2.0 * h);
        break;
      case kL2:
        term1 = 0.2609 * std::sin(-2.0 * h + 2.0 * p) - 0.0370 * std::sin(-o) -
                0.2503 * std::sin(2.0 * p) - 0.1103 * std::sin(2.0 * p + o) -
                0.0491 * std::sin(2.0 * h) - 0.0230 * std::sin(2.0 * h + o);
        term2 = 1.0 + 0.2609 * std::cos(-2.0 * h + 2.0 * p) -
                0.0370 * std::cos(o) - 0.2503 * std::cos(2.0 * p) -
                0.1103 * std::cos(2.0 * p + o) - 0.0491 * std::cos(2.0 * h) -
                0.0230 * std::cos(2.0 * h + o);
        break;
      case kS2:
        term1 = 0.0585 * std::sin(-h + pp) - 0.0084 * std::sin(h - pp) +
                0.2720 * std::sin(2.0 * h) + 0.0811 * std::sin(2.0 * h + o) +
                0.0088 * std::sin(2.0 * h + 2.0 * o);
        term2 = 1.0 + 0.0585 * std::cos(-h + pp) - 0.0084 * std::cos(h - pp) +
                0.2720 * std::cos(2.0 * h) + 0.0811 * std::cos(2.0 * h + o) +
                0.0088 * std::cos(2.0 * h + 2.0 * o);
        break;
      default:
        term1 = 0.0;
        term2 = 0.0;
        break;
    }

    if (term2 == 0.0) {
      correction = evaluate_nodal_correction(omega, perigee, constituent);
    } else {
      correction.f = std::sqrt(term1 * term1 + term2 * term2);
      correction.u = degrees(std::atan2(term1, term2));
    }
  }
  return corrections;
}

NodalCorrectionProcessor::NodalCorrectionProcessor(
    const NodalCorrectionsArgs &args) {
  auto doodson_args = calculate_celestial_vector(args.angles());
  omega_ = -doodson_args(4);
  perigee_ = doodson_args(3);
  hsolar_ = doodson_args(2);
  psolar_ = doodson_args(5);
  group_modulations_ = args.group_modulations();
}

auto NodalCorrectionProcessor::operator()(ConstituentId ident) const
    -> NodalCorrections {
  if (group_modulations_) {
    return evaluate_nodal_correction(psolar_, omega_, perigee_, hsolar_, ident);
  }
  return evaluate_nodal_correction(omega_, perigee_, ident);
}

auto NodalCorrectionProcessor::operator()(const std::vector<ConstituentId> &ids)
    const -> std::vector<NodalCorrections> {
  if (group_modulations_) {
    return evaluate_nodal_corrections(psolar_, omega_, perigee_, hsolar_, ids);
  }
  return evaluate_nodal_corrections(omega_, perigee_, ids);
}

}  // namespace perth
}  // namespace fes
