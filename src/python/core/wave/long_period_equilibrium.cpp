// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave/long_period_equilibrium.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_wave_order2(py::module& m) {
  py::class_<fes::wave::LongPeriodEquilibrium>(m, "LongPeriodEquilibrium",
                                               R"__doc__(
Constituent to compute the long period equilibrium ocean tide.)__doc__")
      .def(py::init<>())
      .def("disable_dynamic_wave",
           &fes::wave::LongPeriodEquilibrium::disable_dynamic_wave,
           py::arg("wave_table"),
           R"__doc__(
Disable the dynamic wave used for the calculation of the long-period
equilibrium ocean tides.

Args:
  Waves to disable
)__doc__")
      .def("lpe_minus_n_waves",
           &fes::wave::LongPeriodEquilibrium::lpe_minus_n_waves,
           py::arg("angles"), py::arg("lat"),
           R"__doc__(
Computes the long-period equilibrium ocean tides.

The complete tidal spectral lines from the Cartwright-Tayler-Edden tables are
summed over to compute the long-period tide.

Order 2 and order 3 of the tidal potential for the long period waves is
now taken into account.

The decomposition was validated compared to the potential proposed by Tamura.

Waves computed dynamically are removed.

Args:
  angles: the astronomic angle, indicating the date on which the tide
    is to be calculated.
  lat: Latitude in degrees (positive north) for the position at which
    tide is computed.
Returns:
  Computed long-period tide, in centimeters.
)__doc__");
}
