// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave/table.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/datemanip.hpp"
#include "fes/python/datetime64.hpp"

namespace py = pybind11;

void init_wave_table(py::module& m) {
  py::class_<fes::wave::Table>(m, "WaveTable",
                               "Properties of tide waves computed")
      .def(py::init<std::vector<std::string>>(),
           py::arg("waves") = std::vector<std::string>{})
      .def("find", &fes::wave::Table::find, py::arg("ident"), R"__doc__(
Find a wave in the table by its name.

Args:
    name: Name of the wave. The search is not case sensitive, so "M2" and "m2"
        are equivalent.

Returns:
    The wave if found, None otherwise.
)__doc__")
      .def("compute_nodal_corrections",
           &fes::wave::Table::compute_nodal_corrections, py::arg("angles"),
           R"__doc__(
Compute nodal corrections.
Args:
  angles: Astronomic angle, indicating the date on which the tide is to be
    calculated.
)__doc__")
      .def("admittance", &fes::wave::Table::admittance, R"__doc__(
Compute waves by admittance from these 7 major ones : O1, Q1, K1, 2n2, N2, M2,
K2.
)__doc__")
      .def_static(
          "harmonic_analysis",
          [](const Eigen::Ref<const Eigen::VectorXd>& h,
             const Eigen::Ref<const Eigen::MatrixXd>& f,
             const Eigen::Ref<const Eigen::MatrixXd>& vu) -> Eigen::VectorXcd {
            py::gil_scoped_release release;
            return fes::wave::Table::harmonic_analysis(h, f, vu);
          },
          py::arg("h"), py::arg("f"), py::arg("vu"),
          R"__doc__(
Harmonic Analysis.

Args:
  h: Sea level.
  f: Nodal correction coefficient applied to the amplitude of the
    constituents analyzed.
  vu: Astronomical argument at time :math:`t` + the nodal correction
    coefficient applied to the phase of the constituents analyzed.

Returns:
    The complex number representing the different reconstructed waves.
)__doc__")
      .def(
          "tide_from_tide_series",
          [](const fes::wave::Table& self, py::array& dates,
             const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
             const Eigen::Ref<const Eigen::VectorXcd>& wave,
             const fes::angle::Formulae& formulae) -> Eigen::VectorXd {
            auto epoch = fes::python::npdatetime64_to_epoch(dates);
            {
              py::gil_scoped_release gil;
              return self.tide_from_tide_series(epoch, leap_seconds, wave);
            }
          },
          py::arg("dates"), py::arg("leap_seconds"), py::arg("wave"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          R"__doc__(
Calculates the tide of a given time series.

Args:
  dates: UTC dates. The array must be one-dimensional and of type
    :py:class:`numpy.datetime64`.
  leap_seconds: Leap seconds at the date of the tide calculation.
  wave: Tidal wave properties computed by
    :py:meth:`fes.core.WaveTable.harmonic_analysis`.
  formulae: Astronomic formulae used to calculate the astronomic angles.
    Defaults to :py:attr:`fes.angle.Formulae.kSchuremanOrder3`.

Return:
  The tide calculated for the time series provided.
)__doc__")
      .def(
          "tide_from_mapping",
          [](const fes::wave::Table& self, const py::handle& date,
             const uint16_t leap_seconds,
             const Eigen::Ref<const Eigen::MatrixXcd>& wave,
             const fes::angle::Formulae& formulae,
             const size_t num_threads) -> Eigen::MatrixXd {
            auto epoch = fes::python::datemanip::as_float64(date);
            {
              py::gil_scoped_release gil;
              return self.tide_from_mapping(epoch, leap_seconds, wave, formulae,
                                            num_threads);
            }
          },
          py::arg("date"), py::arg("leap_seconds"), py::arg("mapping"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          py::arg("num_threads") = 0,
          R"__doc__(
Calculates the tide for a given date from a grid describing the wave properties
over an area of interest.

Args:
  date: UTC date.
  leap_seconds: Leap seconds at the date of the tide calculation.
  mapping: Mapping of the wave properties over an area of interest.
  formulae: Astronomic formulae used to calculate the astronomic angles.
    Defaults to :py:attr:`fes.angle.Formulae.kSchuremanOrder3`.
  num_threads: Number of threads to use. If 0, the number of threads is
    automatically determined.
Returns:
  The tide calculated for the mapping provided at the given date.
)__doc__")
      .def(
          "compute_nodal_modulations",
          [](const fes::wave::Table& self, py::array& dates,
             const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
             const fes::angle::Formulae& formulae)
              -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> {
            auto epoch = fes::python::npdatetime64_to_epoch(dates);
            {
              py::gil_scoped_release gil;
              return self.compute_nodal_modulations(epoch, leap_seconds,
                                                    formulae);
            }
          },
          py::arg("dates"), py::arg("leap_seconds"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          R"__doc__(
Compute nodal modulations for amplitude and phase.
Args:
  dates: Desired UTC time. The array must be one-dimensional
    and of type :py:class:`numpy.datetime64`.
  leap_seconds: Leap seconds at the date of the tide calculation.
  formulae: Astronomic formulae used to calculate the astronomic angles.
    Defaults to :py:attr:`fes.angle.Formulae.kSchuremanOrder3`.
Returns:
  tuple: the nodal correction for amplitude, v (greenwich argument) + u
  (nodal correction for phase)
)__doc__")
      .def_static("select_waves_for_analysis",
                  &fes::wave::Table::select_waves_for_analysis,
                  py::arg("duration"), py::arg("f") = 2.0,
                  R"__doc__(
Return the list of tidal waves such that their period is more than twice the
duration of the time series analyzed.

Args:
  duration: Duration of the time series analyzed in seconds
  f: Number of times the period of the wave is greater than the duration of the
    time series analyzed
Returns:
  List of selected tidal waves.
)__doc__")
      .def("keys", &fes::wave::Table::constituents, R"__doc__(
Return the list of tidal constituents handled by this instance.

Returns:
  List of tidal constituents.
)__doc__")
      .def(
          "values",
          [](const fes::wave::Table& self)
              -> std::vector<std::shared_ptr<fes::Wave>> {
            auto result = std::vector<std::shared_ptr<fes::Wave>>{};
            for (const auto& wave : self) {
              result.emplace_back(wave);
            }
            return result;
          },
          R"__doc__(
Return the list of tidal waves handled by this instance.

Returns:
  List of tidal waves.
)__doc__")
      .def("__len__", [](const fes::wave::Table& self) { return self.size(); })
      .def(
          "__getitem__",
          [](const fes::wave::Table& self, fes::Constituent ident)
              -> std::shared_ptr<fes::Wave> { return self[ident]; },
          py::arg("index"))
      .def(
          "__iter__",
          [](const fes::wave::Table& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());
}
