// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/settings.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/optional.hpp>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"
#include "fes/python/optional.hpp"

namespace py = pybind11;

void init_settings(py::module& m) {
  py::class_<fes::Settings>(m, "Settings", "Settings for the FES computation.")
      .def(py::init([](const fes::angle::Formulae astronomic_formulae,
                       const double time_tolerance,
                       const boost::optional<std::vector<fes::Constituent>>&
                           excluded) {
             return fes::Settings(
                 astronomic_formulae, time_tolerance,
                 excluded.value_or(std::vector<fes::Constituent>()));
           }),
           py::arg("astronomic_formulae") =
               fes::angle::Formulae::kSchuremanOrder1,
           py::arg("time_tolerance") = 0.0, py::arg("excluded") = boost::none,
           R"__doc__(
Constructor.

Args:
    astronomic_formulae: Astronomic formulae used to calculate the
        astronomic angles.
    time_tolerance: The time in seconds during which astronomical
        angles are considered constant. The default value is 0 seconds,
        indicating that astronomical angles do not remain constant with time.
    excluded: The list of tidal constituents to be excluded from the model.
        Constituents included in this list will be processed through admittance
        calculations and in the long-period equilibrium wave calculation
        routine (`lpe_minus_n_waves`). The default value is an empty list.
)__doc__")
      .def_property_readonly("astronomic_formulae",
                             &fes::Settings::astronomic_formulae,
                             "Return the astronomic formulae used to calculate "
                             "the astronomic angles.")
      .def_property_readonly(
          "time_tolerance", &fes::Settings::time_tolerance,
          "Return the time in seconds for which astronomical angles are "
          "considered constant.")
      .def_property_readonly("excluded", &fes::Settings::excluded,
                             "Return the list of tidal constituents to be "
                             "excluded from the model.");
}
