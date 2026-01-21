#pragma once

#include <vector>

#include "fes/perth/constituent.hpp"

namespace fes {
namespace perth {

/// @brief Result structure for nodal corrections.
///
/// Nodal corrections account for the slow modulation of tidal amplitudes
/// and phases due to the 18.6-year regression of the lunar nodes.
struct NodalCorrections {
  double f;  ///< Amplitude modulation factor (typically close to 1.0)
  double u;  ///< Phase correction in degrees
};

/// @brief Computes standard nodal corrections for a list of constituents.
///
/// The nodal corrections (f and u) are computed using Schureman-style
/// formulas that depend on the mean longitude of the lunar node (omega)
/// and the mean longitude of the lunar perigee (p).
///
/// @param[in] omega Mean longitude of lunar node, in degrees.
/// @note omega = -N' (where N' is the 5th Doodson number).
/// @param[in] p Mean longitude of lunar perigee, in degrees.
/// @param[in] constituents A vector of constituents for which to compute
///   corrections.
/// @return A vector of NodalCorrections for each constituent.
auto compute_nodal_corrections(double omega, double p,
                               const std::vector<Constituent> &constituents)
    -> std::vector<NodalCorrections>;

/// @brief Compute nodal corrections for a single constituent.
///
/// @param[in] omega Mean longitude of lunar node, in degrees.
/// @param[in] p Mean longitude of lunar perigee, in degrees.
/// @param[in] constituent The constituent to compute corrections for.
/// @return Nodal corrections {f, u}.
inline auto compute_nodal_correction(double omega, double p,
                                     const Constituent constituent)
    -> NodalCorrections {
  return compute_nodal_corrections(omega, p, {constituent})[0];
}

/// @brief Computes group modulation nodal corrections.
///
/// This routine computes tidal corrections "f" and "u" using group
/// modulation formulas that account for sidelines within a tidal group,
/// rather than within a single constituent. This is a generalization of
/// the standard nodal corrections.
///
/// Only certain tidal groups can be handled this way; if called for an
/// unsupported group, it defaults to the standard nodal corrections.
///
/// @param[in] perihelion Mean longitude of the sun's perihelion, in degrees.
///   (Around J2000, this is about 282 degrees.)
/// @param[in] omega Mean longitude of the lunar node, in degrees.
/// @param[in] perigee Mean longitude of the lunar perigee, in degrees.
/// @param[in] hsolar Mean longitude of the sun, in degrees.
/// @param[in] constituents A vector of constituents for which to compute
///   corrections.
/// @return A vector of NodalCorrections for each constituent.
auto compute_nodal_corrections(double perihelion, double omega, double perigee,
                               double hsolar,
                               const std::vector<Constituent> &constituents)
    -> std::vector<NodalCorrections>;

}  // namespace perth
}  // namespace fes
