// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/optional.hpp>

#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"

namespace py = pybind11;

auto evaluate_equilibrium_long_period(
    py::array& dates,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const Eigen::Ref<const fes::Vector<double>>& latitudes,
    const boost::optional<fes::Settings>& settings, const size_t num_threads)
    -> Eigen::VectorXd {
  if (dates.size() != latitudes.size() || dates.size() != leap_seconds.size()) {
    throw std::invalid_argument(
        "dates, latitudes and leap_seconds must have the same size");
  }
  auto epoch = fes::python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    return fes::evaluate_equilibrium_long_period(
        epoch, leap_seconds, latitudes, settings.value_or(fes::Settings()),
        num_threads);
  }
}

template <typename T>
auto evaluate_tide(const fes::AbstractTidalModel<T>* const tidal_model,
                   py::array& dates,
                   const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
                   const Eigen::Ref<const Eigen::VectorXd>& longitudes,
                   const Eigen::Ref<const Eigen::VectorXd>& latitudes,
                   const boost::optional<fes::Settings>& settings,
                   const size_t num_threads = 0)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, fes::Vector<fes::Quality>> {
  if (dates.size() != leap_seconds.size() ||
      dates.size() != longitudes.size() || dates.size() != latitudes.size()) {
    throw std::invalid_argument(
        "epoch, leap_seconds, longitudes and latitudes must have the same "
        "size");
  }
  auto epoch = fes::python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    return fes::evaluate_tide(tidal_model, epoch, leap_seconds, longitudes,
                              latitudes, settings.value_or(fes::Settings()),
                              num_threads);
  }
}

template <typename T>
void init_tide(py::module& m) {
  m.def("evaluate_tide", &evaluate_tide<T>, py::arg("tidal_model"),
        py::arg("date"), py::arg("leap_seconds"), py::arg("longitude"),
        py::arg("latitude"), py::arg("settings") = boost::none,
        py::arg("num_threads") = 0,
        R"__doc(
Ocean tide calculation

Args:
  tidal_model: Tidal model used to interpolate the modelized waves
  date: Date of the tide calculation
  leap_seconds: Leap seconds at the date of the tide calculation
  longitude: Longitude in degrees for the position at which the tide is
    calculated
  latitude: Latitude in degrees for the position at which the tide is
    calculated
  settings: Settings for the tide computation.
  num_threads: Number of threads to use for the computation. If 0, the
    number of threads is automatically determined.

Returns:
  A tuple that contains:
    * The height of the the diurnal and semi-diurnal constituents of the
      tidal spectrum (cm)
    * The height of the long period wave constituents of the tidal
      spectrum (cm)
    * The quality flag indicating the reliability of the tide
      calculation at the given position:

      * **0**: the tide is undefined (no model data available at the given
        position).
      * **Positive values**: the tide is interpolated at the given
        position using ``N`` data points (where ``N`` is the
        number of data points used for the interpolation).
      * **Negative values**: the tide is extrapolated at the given
        position using ``-N`` data points (where ``N`` is the
        number of data points used for the extrapolation).

.. note::

  Computed height of the diurnal and semi-diurnal constituents is set
  to nan if no data is available at the given position. the long period wave
  constituents is always computed because this value does not depend on
  input grids.
)__doc");
}

void init_tide(py::module& m) {
  init_tide<double>(m);
  init_tide<float>(m);

  m.def("evaluate_equilibrium_long_period", &evaluate_equilibrium_long_period,
        py::arg("dates"), py::arg("leap_seconds"), py::arg("latitudes"),
        py::arg("settings") = boost::none, py::arg("num_threads") = 0,
        R"__doc(Compute the long-period equilibrium ocean tides.

Args:
  dates: Dates of the tide calculation
  leap_seconds: Leap seconds at the date of the tide calculation
  latitudes: Latitude in degrees for the position at which the long-period
    tide is calculated
  settings: Settings for the tide computation.
  num_threads: Number of threads to use for the computation. If 0, the
    number of threads is automatically determined.

Returns:
  The computed long-period tide, in centimeters.
)__doc");
}
