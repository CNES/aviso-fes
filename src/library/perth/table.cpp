#include "fes/perth/table.hpp"

#include <algorithm>
#include <utility>

#include "fes/perth/constituent.hpp"
#include "fes/perth/doodson.hpp"

namespace fes {
namespace perth {

template <typename T>
auto WaveSet<T>::update_nodal_corrections(const double epoch,
                                          const bool group_modulations,
                                          Accelerator<Constituent>& acc)
    -> void {
  auto angle = acc.calculate_angle(epoch);
  auto args = calculate_celestial_vector(angle);
  const auto perigee = args(3);
  const auto omega = -args(4);
  if (group_modulations) {
    const auto hsolar = args(2);
    const auto psolar = args(5);
    nodal_corrections_ =
        compute_nodal_corrections(psolar, omega, perigee, hsolar, keys_vector_);
  } else {
    nodal_corrections_ =
        compute_nodal_corrections(omega, perigee, keys_vector_);
  }
  for (const auto& key : keys_) {
    const auto& component = (*this)[key];
    component.tidal_argument = calculate_doodson_argument(
        angle, component.doodson_number.template cast<double>());
  }
}

}  // namespace perth
}  // namespace fes
