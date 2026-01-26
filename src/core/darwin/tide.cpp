// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/tide.hpp"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/optional.hpp>

#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep
#include "fes/settings.hpp"

namespace py = pybind11;

namespace fes {
namespace darwin {

static auto py_evaluate_equilibrium_long_period(
    py::array& dates, const Eigen::Ref<const Vector<double>>& latitudes,
    const boost::optional<FesRuntimeSettings>& settings) -> Eigen::VectorXd {
  auto epoch = python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    return evaluate_equilibrium_long_period(
        epoch, latitudes, settings.value_or(FesRuntimeSettings()));
  }
}

void init_tide(py::module& m) {
  m.def("evaluate_equilibrium_long_period",
        &py_evaluate_equilibrium_long_period, py::arg("dates"),
        py::arg("latitudes"), py::arg("settings") = boost::none,
        R"__doc(Compute the long-period equilibrium ocean tides.

The complete tidal spectral lines from the Cartwright-Tayler-Edden tables
are summed over to compute the long-period tide.

Args:
  dates: Dates of the tide calculation
  latitudes: Latitude in degrees for the position at which the long-period
    tide is calculated
  settings: Settings for the tide computation.

Returns:
  The computed long-period tide, in centimeters.
)__doc");
}

}  // namespace darwin
}  // namespace fes

void init_darwin_tide(py::module& m) { fes::darwin::init_tide(m); }
