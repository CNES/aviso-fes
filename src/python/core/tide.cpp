// Copyright (c) 2023 CNES
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
    - The height of the the diurnal and semi-diurnal constituents of the
      tidal spectrum (cm)
    - The height of the long period wave constituents of the tidal
      spectrum (cm)
    - The quality of the tide calculation. Could be kUndefined if the
      point is not defined by the model, kInterpolated if the model is
      interpolated, kExtrapolated if the model is extrapolated.

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
}
