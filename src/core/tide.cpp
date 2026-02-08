// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/interface/tidal_model.hpp"
#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep
#include "fes/settings.hpp"
#include "fes/tide.hpp"

namespace py = pybind11;

namespace fes {

template <typename T>
auto py_evaluate_tide(const TidalModelInterface<T>* const tidal_model,
                      py::array& dates,
                      const Eigen::Ref<const Eigen::VectorXd>& longitudes,
                      const Eigen::Ref<const Eigen::VectorXd>& latitudes,
                      const boost::optional<Settings>& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, fes::Vector<fes::Quality>> {
  if (dates.size() != longitudes.size() || dates.size() != latitudes.size()) {
    throw std::invalid_argument(
        "date, longitude and latitude must have the same size");
  }
  auto epoch = python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    return evaluate_tide(tidal_model, epoch, longitudes, latitudes, settings);
  }
}

auto py_evaluate_tide_from_constituents(
    const std::map<std::string, std::pair<double, double>>& constituents,
    py::array& dates, const double latitude,
    const boost::optional<Settings>& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd> {
  auto epoch = python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;

    auto constituents_parsed = std::map<ConstituentId, Complex>{};
    for (const auto& item : constituents) {
      constituents_parsed.emplace(
          constituents::parse(item.first),
          std::polar(item.second.first,
                     detail::math::radians(item.second.second)));
    }

    return evaluate_tide_from_constituents(constituents_parsed, epoch, latitude,
                                           settings);
  }
}

template <typename T>
void init_evaluate_tide(py::module& m) {
  m.def(
      "evaluate_tide",
      [](const TidalModelInterface<T>* const tidal_model, py::array& date,
         const Eigen::Ref<const Eigen::VectorXd>& longitude,
         const Eigen::Ref<const Eigen::VectorXd>& latitude,
         const boost::optional<Settings>& settings)
          -> std::tuple<Eigen::VectorXd, Eigen::VectorXd,
                        fes::Vector<fes::Quality>> {
        return py_evaluate_tide<T>(tidal_model, date, longitude, latitude,
                                   settings);
      },
      py::arg("tidal_model"), py::arg("date"), py::arg("longitude"),
      py::arg("latitude"), py::arg("settings") = boost::none,
      R"__doc(
Ocean tide calculation.

This function computes ocean tides by interpolating tidal constituents from
a tidal model at the specified positions and times.

Args:
  tidal_model: Tidal model used to interpolate the modelized waves.
  date: Date of the tide calculation (numpy.datetime64 array).
  longitude: Longitude in degrees for the positions at which the tide is
    calculated.
  latitude: Latitude in degrees for the positions at which the tide is
    calculated.
  settings: Settings for the tide computation.

Returns:
  A tuple containing:
    * The height of the diurnal and semi-diurnal constituents of the
      tidal spectrum (same units as model constituents, typically cm).
    * The height of the long period wave constituents of the tidal
      spectrum (same units as model constituents, typically cm).
    * The quality flag indicating the reliability of the tide
      calculation at the given position:

      * **0**: the tide is undefined (no model data available).
      * **Positive values**: the tide is interpolated using N data points.
      * **Negative values**: the tide is extrapolated using -N data points.

Note:
  Computed height of the diurnal and semi-diurnal constituents is set
  to NaN if no data is available at the given position.

Warning:
  If :attr:`Settings.compute_long_period_equilibrium` returns true, the tidal
  model must use centimeters. The long period equilibrium is computed in
  centimeters and is added to the model tide; mixing units would make the
  result inconsistent.
)__doc");
}

void init_evaluate_tide_from_constituents(py::module& m) {
  m.def("evaluate_tide_from_constituents", &py_evaluate_tide_from_constituents,
        py::arg("constituents"), py::arg("date"), py::arg("latitude"),
        py::arg("settings") = boost::none,
        R"__doc(Ocean tide calculation from known constituents.

Unlike evaluate_tide() which interpolates constituents from a tidal model,
this function computes the tidal prediction directly from a list of tidal
constituents whose properties (amplitude and phase) are already known.
This is typically used for tide gauge analysis and prediction, where the
constituents have been previously determined from harmonic analysis of
observed sea level data.

Args:
  constituents: A dictionary mapping constituent names (e.g., "M2", "S2")
    to tuples of (amplitude, phase) where phase is in degrees.
  date: Date of the tide calculation (numpy.datetime64 array).
  latitude: Latitude in degrees for the position at which the tide is
    calculated.
  settings: Settings for the tide computation.

Returns:
  A tuple containing:
    * The height of the diurnal and semi-diurnal constituents of the
      tidal spectrum (same units as the input constituents).
    * The height of the long period wave constituents of the tidal
      spectrum (same units as the input constituents).

Warning:
  If :attr:`Settings.compute_long_period_equilibrium` returns true, the input
  constituents must use centimeters. The long period equilibrium is computed in
  centimeters and is added to the constituent tide; mixing units would make the
  result inconsistent.
)__doc");
}

}  // namespace fes

void init_tide(py::module& m) {
  fes::init_evaluate_tide<double>(m);
  fes::init_evaluate_tide<float>(m);
  fes::init_evaluate_tide_from_constituents(m);
}
