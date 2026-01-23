#pragma once

#include <boost/optional.hpp>

#include "fes/angle/astronomic.hpp"
#include "fes/perth/inference.hpp"
#include "fes/settings.hpp"

namespace fes {
namespace perth {

/// @brief Settings for Perth tide computation.
/// This extends the general FES settings with Perth-specific options.
class Settings : public fes::Settings {
 public:
  /// @brief Construct a Settings object for Perth tide computation.
  /// @param[in] astronomic_formulae Astronomic formulae used to calculate the
  /// astronomic angles.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @param[in] group_modulations Whether to use group modulation nodal
  /// corrections.
  /// @param[in] inference_type The type of inference interpolation to use. If
  /// boost::none, no inference interpolation is used.
  explicit Settings(
      const angle::Formulae& astronomic_formulae =
          angle::Formulae::kSchuremanOrder1,
      const double time_tolerance = 0.0, const bool group_modulations = false,
      boost::optional<InterpolationType> inference_type = boost::none)
      : fes::Settings(astronomic_formulae, time_tolerance),
        group_modulations_(group_modulations),
        inference_type_(inference_type) {}

  /// @brief Returns whether to use group modulation nodal corrections.
  ///
  /// @return True if group modulation nodal corrections are to be used.
  constexpr auto group_modulations() const noexcept -> bool {
    return group_modulations_;
  }

  /// @brief Returns the type of inference interpolation to use, if any.
  ///
  /// @return The type of inference interpolation to use, or boost::none if
  ///         no inference interpolation is to be used.
  constexpr auto inference() const noexcept
      -> const boost::optional<InterpolationType>& {
    return inference_type_;
  }

 private:
  /// Whether to use group modulation nodal corrections
  bool group_modulations_{false};
  /// Type of inference interpolation to use
  boost::optional<InterpolationType> inference_type_{boost::none};
};

}  // namespace perth
}  // namespace fes
