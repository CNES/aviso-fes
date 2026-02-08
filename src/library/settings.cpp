#include "fes/settings.hpp"

#include <boost/range/algorithm/find.hpp>
#include <initializer_list>
#include <string>
#include <vector>

#include "fes/constituent.hpp"
#include "fes/detail/markdown_table.hpp"
#include "fes/interface/wave.hpp"
#include "fes/interface/wave_table.hpp"

namespace fes {
namespace {

// ---------------------------------------------------------------------------
// Helper functions
// ---------------------------------------------------------------------------
inline auto check_if_is_inferred(
    const ConstituentId ident, const bool is_modeled,
    const std::vector<ConstituentId>& inferred_constituents) -> bool {
  if (is_modeled) {
    return false;
  }
  return boost::range::find(inferred_constituents, ident) !=
         inferred_constituents.end();
}

inline auto engine_type_str(EngineType engine_type) -> std::string {
  switch (engine_type) {
    case EngineType::kDarwin:
      return "Darwin";
    case EngineType::kDoodson:
      return "Doodson";
    default:
      return "Unknown";
  }
}

inline auto astronomic_formulae_str(angle::Formulae formulae) -> std::string {
  switch (formulae) {
    case angle::Formulae::kSchuremanOrder1:
      return "Schureman Order 1";
    case angle::Formulae::kSchuremanOrder3:
      return "Schureman Order 3";
    case angle::Formulae::kMeeus:
      return "Meeus";
    case angle::Formulae::kIERS:
      return "IERS";
    default:
      return "Unknown";
  }
}

inline auto inference_type_str(InferenceType inference_type) -> std::string {
  switch (inference_type) {
    case InferenceType::kSpline:
      return "Spline";
    case InferenceType::kZero:
      return "Zero";
    case InferenceType::kLinear:
      return "Linear";
    case InferenceType::kFourier:
      return "Fourier";
    default:
      return "Unknown";
  }
}

inline auto yes_no(bool value) -> std::string { return value ? "Yes" : "No"; }

}  // anonymous namespace

auto generate_markdown_table(
    const Settings& settings,
    const std::vector<ConstituentId>& modeled_constituents) -> std::string {
  auto wt = wave_table_factory(settings.engine_type());
  wt->set_modeled_constituents(modeled_constituents);
  auto inference = inference_factory(*wt, settings.inference_type());
  auto inferred_constituents = inference->inferred_constituents();

  // Settings table
  detail::MarkdownTable settings_table({"Setting", "Value"});
  settings_table.add_row(
      {"Engine Type", engine_type_str(settings.engine_type())});
  settings_table.add_row(
      {"Astronomic Formulae",
       astronomic_formulae_str(settings.astronomic_formulae())});
  settings_table.add_row(
      {"Inference Type", inference_type_str(settings.inference_type())});
  settings_table.add_row(
      {"Time Tolerance (s)", std::to_string(settings.time_tolerance())});
  settings_table.add_row(
      {"Group Modulations", settings.engine_type() == EngineType::kDoodson
                                ? yes_no(settings.group_modulations())
                                : "N/A"});
  settings_table.add_row({"Compute Long Period Equilibrium",
                          yes_no(settings.compute_long_period_equilibrium())});
  settings_table.add_row(
      {"Number of Threads", std::to_string(settings.num_threads())});

  // Constituents table
  detail::MarkdownTable constituents_table(
      {"Constituent", "Speed (Deg/hr)", "XDO", "Modeled", "Inferred"});
  for (const auto& item : wt->sort_by_frequency()) {
    const auto& wave = (*wt)[item];
    auto is_modeled = wave->is_modeled();
    auto is_inferred =
        check_if_is_inferred(wave->ident(), is_modeled, inferred_constituents);

    // Only display modeled and inferred constituents
    if (!is_modeled && !is_inferred) {
      continue;
    }

    constituents_table.add_row(
        {wave->latex_name(), std::to_string(wave->frequency<kDegreePerHour>()),
         wave->xdo_alphabetical(), yes_no(is_modeled), yes_no(is_inferred)});
  }

  return settings_table.to_string() + "\n" + constituents_table.to_string() +
         "\n";
}

}  // namespace fes
