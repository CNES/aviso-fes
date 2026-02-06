// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/long_period_equilibrium.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <boost/optional.hpp>

#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep
#include "fes/settings.hpp"
#include "fes/tide.hpp"
#include "pybind11/cast.h"

namespace py = pybind11;

namespace fes {

static auto py_evaluate_equilibrium_long_period(
    py::array& dates, const Eigen::Ref<const Vector<double>>& latitudes,
    const std::vector<std::string>& constituents,
    const boost::optional<Settings>& settings) -> Eigen::VectorXd {
  auto epoch = python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    auto parsed_constituents = std::vector<ConstituentId>();
    parsed_constituents.reserve(constituents.size());
    for (const auto& constituent : constituents) {
      parsed_constituents.push_back(constituents::parse(constituent));
    }
    return evaluate_equilibrium_long_period(epoch, latitudes,
                                            parsed_constituents, settings);
  }
}

static void init_lpe_tide(py::module& m) {
  py::class_<LongPeriodEquilibrium>(m, "LongPeriodEquilibrium",
                                    R"__doc__(
Constituent to compute the long period equilibrium ocean tide.)__doc__")
      .def(py::init<>())
      .def("disable_dynamic_wave", &LongPeriodEquilibrium::disable_dynamic_wave,
           py::arg("wave_table"),
           R"__doc__(
Disable the dynamic wave used for the calculation of the long-period
equilibrium ocean tides.

Args:
  Waves to disable
)__doc__")
      .def("lpe_minus_n_waves", &LongPeriodEquilibrium::lpe_minus_n_waves,
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

  m.def("evaluate_equilibrium_long_period",
        &py_evaluate_equilibrium_long_period, py::arg("dates"),
        py::arg("latitudes"), py::arg("constituents"),
        py::arg("settings") = boost::none,
        R"__doc(Compute the long-period equilibrium ocean tides.

The complete tidal spectral lines from the Cartwright-Tayler-Edden tables
are summed over to compute the long-period tide.

Args:
  dates: Dates of the tide calculation
  latitudes: Latitude in degrees for the position at which the long-period
    tide is calculated
  constituents: List of constituents to disable from the inferred calculation
    of the long-period tide.
  settings: Settings for the tide computation.

Returns:
  The computed long-period tide, in centimeters.
)__doc");
}

}  // namespace fes

void init_lpe_tide(py::module& m) { fes::init_lpe_tide(m); }
