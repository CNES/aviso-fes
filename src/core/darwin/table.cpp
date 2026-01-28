// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/table.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/darwin/constituent.hpp"
#include "fes/python/datemanip.hpp"
#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep

namespace py = pybind11;

namespace fes {
namespace darwin {

static void init_table(py::module& m) {
  py::class_<WaveTable>(m, "WaveTable",
                        "Properties of the tide waves handled by FES")
      .def(py::init([](const boost::optional<std::vector<std::string>>&
                           constituents) {
             if (constituents) {
               return WaveTable(*constituents);
             } else {
               return WaveTable();
             }
           }),
           py::arg("constituents") = boost::none,
           R"__doc__(Properties of the tide waves handled by FES.

  Args:
    constituents: List of tidal constituents to handle. If None, all
      constituents known to FES will be used.
  )__doc__")
      .def("compute_nodal_corrections", &WaveTable::compute_nodal_corrections,
           py::arg("angles"),
           R"__doc__(Compute nodal corrections.

Args:
  angles: Astronomical angle indicating the date for which to calculate the tide.
)__doc__")
      .def("admittance", &WaveTable::admittance,
           "Compute waves by admittance from the 7 major constituents: O1, Q1, "
           "K1, 2N2, N2, M2, K2.")
      .def_static(
          "harmonic_analysis",
          [](const Eigen::Ref<const Eigen::VectorXd>& h,
             const DynamicRef<const Eigen::MatrixXd>& f,
             const DynamicRef<const Eigen::MatrixXd>& vu) -> Eigen::VectorXcd {
            py::gil_scoped_release release;
            return WaveTable::harmonic_analysis(h, f, vu);
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
          [](const WaveTable& self, py::array& dates,
             const Eigen::Ref<const Eigen::VectorXcd>& wave,
             const angle::Formulae& formulae) -> Eigen::VectorXd {
            auto epoch = python::npdatetime64_to_epoch(dates);
            {
              py::gil_scoped_release gil;
              return self.tide_from_tide_series(epoch, wave);
            }
          },
          py::arg("dates"), py::arg("wave"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          R"__doc__(
Calculates the tide of a given time series.

Args:
  dates: UTC dates. The array must be one-dimensional and of type
    :py:class:`numpy.datetime64`.
  wave: Tidal wave properties computed by
    :py:meth:`pyfes.WaveTable.harmonic_analysis
    <pyfes.core.WaveTable.harmonic_analysis>`.
  formulae: Astronomic formulae used to calculate the astronomic angles.
    Defaults to :py:attr:`pyfes.Formulae.kSchuremanOrder3
    <pyfes.core.Formulae.kSchuremanOrder3>`.

Return:
  The tide calculated for the time series provided.
)__doc__")
      .def(
          "tide_from_mapping",
          [](const WaveTable& self, const py::handle& date,
             const DynamicRef<const Eigen::MatrixXcd>& wave,
             const angle::Formulae& formulae,
             const size_t num_threads) -> Eigen::MatrixXd {
            auto epoch = python::datemanip::as_float64(date);
            {
              py::gil_scoped_release gil;
              return self.tide_from_mapping(epoch, wave, formulae, num_threads);
            }
          },
          py::arg("date"), py::arg("mapping"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          py::arg("num_threads") = 0,
          R"__doc__(
Calculates the tide for a given date from a grid describing the wave properties
over an area of interest.

Args:
  date: UTC date.
  mapping: Mapping of the wave properties over an area of interest.
  formulae: Astronomic formulae used to calculate the astronomic angles.
    Defaults to :py:attr:`pyfes.Formulae.kSchuremanOrder3
    <pyfes.core.Formulae.kSchuremanOrder3>`.
  num_threads: Number of threads to use. If 0, the number of threads is
    automatically determined.
Returns:
  The tide calculated for the mapping provided at the given date.
)__doc__")
      .def(
          "compute_nodal_modulations",
          [](const WaveTable& self, py::array& dates,
             const angle::Formulae& formulae)
              -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> {
            auto epoch = python::npdatetime64_to_epoch(dates);
            {
              py::gil_scoped_release gil;
              return self.compute_nodal_modulations(epoch, formulae);
            }
          },
          py::arg("dates"),
          py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder3,
          R"__doc__(
Compute nodal modulations for amplitude and phase.

Args:
    dates: Desired UTC time. The array must be one-dimensional
        and of type :py:class:`numpy.datetime64`.
    formulae: Astronomic formulae used to calculate the astronomic angles.
        Defaults to :py:attr:`pyfes.Formulae.kSchuremanOrder3
        <pyfes.core.Formulae.kSchuremanOrder3>`.

Returns:
    tuple: the nodal correction for amplitude, v (greenwich argument) + u
    (nodal correction for phase)
)__doc__")
      .def_static("select_waves_for_analysis",
                  &WaveTable::select_waves_for_analysis, py::arg("duration"),
                  py::arg("f") = 2.0,
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
      .def("keys", &WaveTable::constituents, R"__doc__(
Return the list of tidal constituents handled by this instance.

Returns:
  List of tidal constituents.
)__doc__")
      .def(
          "values",
          [](const WaveTable& self) -> std::vector<std::shared_ptr<Wave>> {
            auto result = std::vector<std::shared_ptr<Wave>>{};
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
      .def("__len__", [](const WaveTable& self) { return self.size(); })
      .def(
          "__getitem__",
          [](const WaveTable& self,
             const std::string& name) -> std::shared_ptr<Wave> {
            auto ident = constituents::parse(name);
            return self[ident];
          },
          py::arg("index"))
      .def(
          "__contains__",
          [](const WaveTable& self, const std::string& name) -> bool {
            try {
              return self.contains(constituents::parse(name));
            } catch (...) {
              return false;
            }
          },
          py::arg("index"))
      .def(
          "__iter__",
          [](const WaveTable& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());
}

}  // namespace darwin
}  // namespace fes

auto init_darwin_table(py::module& m) -> void { fes::darwin::init_table(m); }
