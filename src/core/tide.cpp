// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/darwin/constituent.hpp"
#include "fes/darwin/tide.hpp"
#include "fes/detail/math.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/tide.hpp"
#include "fes/python/datetime64.hpp"
#include "fes/settings.hpp"

namespace py = pybind11;

namespace fes {

template <typename T>
auto py_evaluate_tide(const fes::AbstractTidalModel<T>* const tidal_model,
                      py::array& dates,
                      const Eigen::Ref<const Eigen::VectorXd>& longitudes,
                      const Eigen::Ref<const Eigen::VectorXd>& latitudes,
                      const fes::Settings& settings)
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
    py::array& dates, const double latitude, const fes::Settings& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd> {
  auto epoch = python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;

    if (settings.engine_type() == EngineType::kFES) {
      // Convert to Darwin constituents
      std::map<darwin::Constituent, std::complex<double>> complex_constituents;
      for (const auto& item : constituents) {
        auto constituent = darwin::constituents::parse(item.first);
        complex_constituents[constituent] = std::polar(
            item.second.first, detail::math::radians(item.second.second));
      }
      return darwin::evaluate_tide_from_constituents(complex_constituents,
                                                     epoch, latitude, settings);
    }
    if (settings.engine_type() == EngineType::kPERTH5) {
      // Convert to Perth constituents
      std::map<perth::Constituent, std::complex<double>> complex_constituents;
      for (const auto& item : constituents) {
        auto constituent = perth::constituents::parse(item.first);
        complex_constituents[constituent] = std::polar(
            item.second.first, detail::math::radians(item.second.second));
      }
      return perth::evaluate_tide_from_constituents(complex_constituents, epoch,
                                                    latitude, settings);
    }
    throw std::invalid_argument(
        "Unsupported engine type for tidal evaluation from constituents.");
  }
}

template <typename T>
void init_evaluate_tide(py::module& m) {
  m.def(
      "evaluate_tide",
      [](const fes::AbstractTidalModel<T>* const tidal_model, py::array& date,
         const Eigen::Ref<const Eigen::VectorXd>& longitude,
         const Eigen::Ref<const Eigen::VectorXd>& latitude,
         const fes::Settings& settings)
          -> std::tuple<Eigen::VectorXd, Eigen::VectorXd,
                        fes::Vector<fes::Quality>> {
        return py_evaluate_tide<T>(tidal_model, date, longitude, latitude,
                                   settings);
      },
      py::arg("tidal_model"), py::arg("date"), py::arg("longitude"),
      py::arg("latitude"), py::arg("settings"),
      R"__doc(
Ocean tide calculation.

This function computes ocean tides by interpolating tidal constituents from
a tidal model at the specified positions and times. The computation engine
(FES or PERTH5) is determined by the settings object.

Args:
  tidal_model: Tidal model used to interpolate the modelized waves.
  date: Date of the tide calculation (numpy.datetime64 array).
  longitude: Longitude in degrees for the positions at which the tide is
    calculated.
  latitude: Latitude in degrees for the positions at which the tide is
    calculated.
  settings: Settings for the tide computation. Use FesRuntimeSettings for
    the FES/Darwin engine or PerthRuntimeSettings for the PERTH5 engine.

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
)__doc");
}

void init_evaluate_tide_from_constituents(py::module& m) {
  m.def("evaluate_tide_from_constituents", &py_evaluate_tide_from_constituents,
        py::arg("constituents"), py::arg("date"), py::arg("latitude"),
        py::arg("settings"),
        R"__doc(
Ocean tide calculation from known constituents.

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
  settings: Settings for the tide computation. Use FesRuntimeSettings for
    the FES/Darwin engine or PerthRuntimeSettings for the PERTH5 engine.

Returns:
  A tuple containing:
    * The height of the diurnal and semi-diurnal constituents of the
      tidal spectrum (same units as the input constituents).
    * The height of the long period wave constituents of the tidal
      spectrum (same units as the input constituents).

Example:
  >>> import numpy as np
  >>> from pyfes import core
  >>> constituents = {
  ...     "M2": (1.5, 120.0),  # amplitude=1.5m, phase=120 degrees
  ...     "S2": (0.5, 45.0),
  ...     "K1": (0.3, 200.0),
  ... }
  >>> dates = np.array(['2024-01-01T00:00:00'], dtype='datetime64[ns]')
  >>> tide, lp = core.evaluate_tide_from_constituents(
  ...     constituents, dates, latitude=45.0,
  ...     settings=core.FesRuntimeSettings())
)__doc");
}

}  // namespace fes

void init_tide(py::module& m) {
  fes::init_evaluate_tide<double>(m);
  fes::init_evaluate_tide<float>(m);
  fes::init_evaluate_tide_from_constituents(m);
}
