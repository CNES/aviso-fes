#include "fes/tide.hpp"

namespace fes {

auto evaluate_tide_from_constituents(
    const std::map<ConstituentId, Complex>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double latitude,
    const boost::optional<Settings>& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd> {
  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());

  // Use default settings if not provided
  auto settings_instance = settings.value_or(Settings{});

  // Worker function to evaluate the tide for a chunk of data. This will be
  // executed in parallel across multiple threads.
  auto worker = [&](const int64_t start, const int64_t end) {
    // For each thread, we create a new instance of the wave table and
    // accelerator
    auto acc = Accelerator(settings_instance.astronomic_formulae(),
                           settings_instance.time_tolerance(), 0);

    auto wt_ptr = wave_table_factory(settings_instance.engine_type());
    wt_ptr->set_modeled_constituents(constituents);
    wt_ptr->set_tides(constituents);

    auto inference_ptr =
        inference_factory(*wt_ptr, settings_instance.inference_type());

    // For optimization purposes, we capture the dereferenced pointers to avoid
    // unnecessary pointer dereferencing in the inner loop.
    auto& wt_instance = *wt_ptr;
    auto& inference_instance = *inference_ptr;

    // Initialize the long period equilibrium handler for this thread
    auto lpe = LongPeriodEquilibrium(wt_instance);

    // Flag to indicate whether to compute the long period equilibrium.
    const auto compute_long_period_equilibrium =
        settings_instance.compute_long_period_equilibrium();

    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix)) = detail::evaluate_tide_from_waves(
          epoch(ix), latitude, compute_long_period_equilibrium,
          settings_instance, wt_instance, inference_instance, lpe, acc);
    }
  };

  fes::detail::parallel_for(worker, epoch.size(),
                            settings_instance.num_threads());
  return {tide, long_period};
}

auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXd>& latitude,
    const std::vector<ConstituentId>& constituents,
    const boost::optional<Settings>& settings) -> Eigen::VectorXd {
  // Checks the input parameters
  detail::check_eigen_shape("epoch", epoch, "latitude", latitude);

  // Use default settings if not provided
  auto settings_instance = settings.value_or(Settings{});

  // Create the result vector to fill with the long period equilibrium tide
  // values
  auto result = Eigen::VectorXd(epoch.size());

  // Worker function to evaluate the long period equilibrium tide for a chunk of
  // data. This will be executed in parallel across multiple threads.
  auto worker = [&](const int64_t start, const int64_t end) {
    // Create instance in this threads that depend on the current date
    auto angles = angle::Astronomic(settings_instance.astronomic_formulae());
    auto model = LongPeriodEquilibrium();

    // If the user provided a list of constituents, we disable the dynamic wave
    // for these constituents. This allows to compute the long period
    // equilibrium tide without the contribution of the dynamic wave for these
    // constituents.
    if (!constituents.empty()) {
      auto wave_table = wave_table_factory(settings_instance.engine_type());
      wave_table->set_modeled_constituents(constituents);
      model.disable_dynamic_wave(*wave_table);
    }

    for (auto ix = start; ix < end; ++ix) {
      angles.update(epoch(ix));
      result(ix) = model.lpe_minus_n_waves(angles, latitude(ix));
    }
  };

  fes::detail::parallel_for(worker, epoch.size(),
                            settings_instance.num_threads());
  return result;
}

}  // namespace fes
