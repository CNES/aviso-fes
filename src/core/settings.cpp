// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/settings.hpp"

#include <pybind11/pybind11.h>

#include "fes/types.hpp"

namespace py = pybind11;

namespace fes {

inline void init_settings(py::module& m) {
  py::enum_<fes::TidalType>(m, "TidalType", "Possible type of tidal wave")
      .value("LONG_PERIOD", fes::kLongPeriod, "Long period tidal waves")
      .value("SHORT_PERIOD", fes::kShortPeriod, "Short period tidal waves")
      .export_values();

  py::enum_<InterpolationType>(
      m, "InterpolationType",
      "Enumeration of inference (admittance) interpolation types.")
      .value("ZERO_ADMITTANCE", InterpolationType::kZeroAdmittance,
             "No interpolation of admittances.")
      .value("LINEAR_ADMITTANCE", InterpolationType::kLinearAdmittance,
             "Piecewise linear interpolation of admittances.")
      .value("FOURIER_ADMITTANCE", InterpolationType::kFourierAdmittance,
             "Munk-Cartwright Fourier series interpolation.")
      .export_values();

  py::class_<Settings>(m, "Settings", "Settings for tidal computations.")
      .def("with_astronomic_formulae", &Settings::with_astronomic_formulae,
           py::arg("formulae"),
           R"__doc__(
Set the astronomic formulae.

Args:
    formulae: Astronomic formulae used to determine the positions of celestial
      bodies.

Returns:
    Reference to the Settings object.
)__doc__")
      .def("with_time_tolerance", &Settings::with_time_tolerance,
           py::arg("time_tolerance"),
           R"__doc__(
Set the time tolerance.

Args:
    time_tolerance: The time in seconds during which astronomical angles are
        considered constant. The default value is 0 seconds, indicating that
        astronomical angles do not remain constant with time.

Returns:
    Reference to the Settings object.

Note:
    When set to zero, astronomical angles are recalculated for each date
    change. Otherwise, angles remain valid as long as the time difference
    from the last evaluation is within the tolerance.

    Tidal evaluations may vary slightly between runs using different thread
    counts, as the computation period is divided differently based on the
    number of threads, affecting when angles are updated.
)__doc__")
      .def("with_num_threads", &Settings::with_num_threads,
           py::arg("num_threads"),
           R"__doc__(
Set the number of threads to use for computation.

Args:
    num_threads: Number of threads. If 0, the number of threads is
      automatically determined.

Returns:
    Reference to the Settings object.
)__doc__")
      .def_property_readonly("formulae", &Settings::astronomic_formulae,
                             "Get the astronomic formulae.")
      .def_property_readonly("time_tolerance", &Settings::time_tolerance,
                             "Get the time tolerance.")
      .def_property_readonly(
          "num_threads", &Settings::num_threads,
          "Get the number of threads to use for computation.");

  py::class_<FesRuntimeSettings, Settings>(m, "FesRuntimeSettings",
                                           "Settings for the FES engine.")
      .def(py::init<>(), "Default constructor.");

  py::class_<PerthRuntimeSettings, Settings>(m, "PerthRuntimeSettings",
                                             "Settings for the PERTH5 engine.")
      .def(py::init<>(), "Default constructor.")
      .def("with_group_modulations",
           &PerthRuntimeSettings::with_group_modulations,
           py::arg("group_modulations"),
           R"__doc__(
Set whether to use group modulation nodal corrections.

Args:
    group_modulations: True to use group modulation nodal corrections.

Returns:
    Reference to the PerthRuntimeSettings object.
)__doc__")
      .def("with_inference_type", &PerthRuntimeSettings::with_inference_type,
           py::arg("inference_type"),
           R"__doc__(
Set the inference interpolation type.

Args:
    inference_type: Inference interpolation type.

Returns:
    Reference to the PerthRuntimeSettings object.
)__doc__")
      .def_property_readonly(
          "group_modulations", &PerthRuntimeSettings::group_modulations,
          "Get whether to use group modulation nodal corrections.")
      .def_property_readonly("inference_type",
                             &PerthRuntimeSettings::inference_type,
                             "Get the inference interpolation type.");
}

}  // namespace fes

void init_settings(py::module& m) { fes::init_settings(m); }
