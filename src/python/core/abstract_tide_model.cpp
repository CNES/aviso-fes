// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/abstract_tidal_model.hpp"
#include "fes/constituent.hpp"
#include "fes/detail/thread.hpp"
#include "fes/wave/table.hpp"

namespace py = pybind11;

// Trampoline class for AbstractTidalModel
template <typename T, typename ConstituentId>
class PyAbstractTidalModel : public fes::AbstractTidalModel<T, ConstituentId> {
 private:
  using cname = fes::AbstractTidalModel<T, ConstituentId>;

 public:
  using fes::AbstractTidalModel<T, ConstituentId>::AbstractTidalModel;

  auto accelerator(const fes::angle::Formulae& formulae,
                   const double time_tolerance) const
      -> fes::Accelerator<ConstituentId>* override {
    PYBIND11_OVERLOAD_PURE(fes::Accelerator<ConstituentId>*, cname, accelerator,
                           formulae, time_tolerance);
  }

  auto interpolate(const fes::geometry::Point& point, fes::Quality& quality,
                   fes::Accelerator<ConstituentId>* acc) const
      -> const fes::ConstituentValues<ConstituentId>& override {
    PYBIND11_OVERLOAD_PURE(const fes::ConstituentValues<ConstituentId>&, cname,
                           interpolate, point, quality, acc);
  }

  void add_constituent(const fes::Constituent ident,
                       fes::Vector<std::complex<T>> wave) override {
    PYBIND11_OVERLOAD_PURE(void, cname, add_constituent, ident, wave);
  }
};

/// Interpolate the wave models loaded at the given positions.
///
/// @param[in] self The tidal model.
/// @param[in] lon The longitude of the point to interpolate at.
/// @param[in] lat The latitude of the point to interpolate at.
/// @param[in] num_threads The number of threads to use.
/// @return A tuple containing the interpolated wave models stored in a
/// dictionary and a flag indicating if the point was extrapolated,
/// interpolated or if the model is undefined.
template <typename T, typename ConstituentId>
static auto interpolate(const fes::AbstractTidalModel<T, ConstituentId>& self,
                        const Eigen::Ref<const Eigen::VectorXd>& lon,
                        const Eigen::Ref<const Eigen::VectorXd>& lat,
                        const size_t num_threads = 0)
    -> std::tuple<std::map<fes::Constituent, Eigen::VectorXcd>,
                  Eigen::Matrix<int8_t, -1, 1>> {
  if (lon.size() != lat.size()) {
    throw std::invalid_argument("lon and lat must have the same size");
  }
  // Allocate result vectors
  auto values = std::map<fes::Constituent, Eigen::VectorXcd>();
  auto qualities = Eigen::Matrix<int8_t, -1, 1>(lon.size());

  for (auto&& ident : self.identifiers()) {
    values[ident] = Eigen::VectorXcd(lon.size());
  }

  // Interpolate in parallel
  auto thread = [&](const int64_t start, const int64_t end) -> void {
    auto acc = std::unique_ptr<fes::Accelerator<ConstituentId>>(
        self.accelerator(fes::angle::Formulae::kSchuremanOrder1, 0.0));
    auto* acc_ptr = acc.get();
    for (auto ix = start; ix < end; ++ix) {
      const auto point = fes::geometry::Point(lon[ix], lat[ix]);
      auto quality = fes::kUndefined;
      for (auto&& item : self.interpolate(point, quality, acc_ptr)) {
        values[std::get<0>(item)][ix] =
            quality == fes::kUndefined
                ? std::numeric_limits<double>::quiet_NaN()
                : std::get<1>(item);
      }
      qualities[ix] = static_cast<int8_t>(quality);
    }
  };

  fes::detail::parallel_for(thread, lon.size(), num_threads);
  return std::make_tuple(values, qualities);
}

template <typename T, typename ConstituentId>
void init_abstract_tide_model(py::module& m, const std::string& postfix) {
  py::class_<fes::AbstractTidalModel<T, ConstituentId>,
             PyAbstractTidalModel<T, ConstituentId>,
             std::shared_ptr<fes::AbstractTidalModel<T, ConstituentId>>>(
      m, ("AbstractTidalModel" + postfix).c_str(),
      "Abstract class for a tidal model.")
      .def(
          "add_constituent",
          [](fes::AbstractTidalModel<T, ConstituentId>& self,
             const std::string& name,
             fes::Vector<std::complex<T>> wave) -> void {
            self.add_constituent(name, std::move(wave));
          },
          py::arg("name"), py::arg("wave"),
          R"__doc__(
Add a tidal constituent to the model.

Args:
  name: The name of tidal constituent to add. Search is not case sensitive. So
    ``Msqm``, ``MSQM`` and ``msqm`` are equivalent.
  wave: The wave model.
)__doc__")
      .def("accelerator",
           &fes::AbstractTidalModel<T, ConstituentId>::accelerator,
           py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder1,
           py::arg("time_tolerance") = 0.0,
           py::call_guard<py::gil_scoped_release>(),
           R"__doc__(
Get the accelerator used to speed up the interpolation of the tidal model.

Returns:
  The accelerator.
)__doc__")
      .def("interpolate", &interpolate<T, ConstituentId>, py::arg("lon"),
           py::arg("lat"), py::arg("num_threads") = 0,
           py::call_guard<py::gil_scoped_release>(),
           R"__doc__(
Interpolate the wave models loaded at the given coordinates.

Args:
  lon: The longitude of the point to interpolate at.
  lat: The latitude of the point to interpolate at.
  num_threads: The number of threads to use. If 0, the number of threads is
    determined by the number of cores.

Returns:
  A tuple containing the interpolated wave models stored in a dictionary and a
  flag indicating if the point was extrapolated, interpolated or if the model
  is undefined.
      )__doc__")
      .def(
          "interpolate",
          [](const fes::AbstractTidalModel<T, ConstituentId>& self,
             const double lon, const double lat,
             fes::wave::Table& wt) -> fes::Quality {
            auto acc = std::unique_ptr<fes::Accelerator<ConstituentId>>(
                self.accelerator(fes::angle::Formulae::kSchuremanOrder1, 0.0));
            return self.interpolate({lon, lat}, wt, acc.get());
          },
          py::arg("lon"), py::arg("lat"), py::arg("wave_table"),
          py::call_guard<py::gil_scoped_release>(),
          R"__doc__(
Interpolate the wave models loaded at the given position.

Args:
  lon: The longitude of the point to interpolate at.
  lat: The latitude of the point to interpolate at.
  wave_table: The wave table to store the interpolated wave models.

Returns:
  A flag indicating if the point was extrapolated, interpolated or if the model
  is undefined.
)__doc__")
      .def_property(
          "dynamic",
          [](const fes::AbstractTidalModel<T, ConstituentId>& self)
              -> const std::vector<fes::Constituent>& {
            return self.dynamic();
          },
          [](fes::AbstractTidalModel<T, ConstituentId>& self,
             const std::vector<fes::Constituent>& dynamic) -> void {
            self.dynamic(dynamic);
          },
          R"__doc__(
Dynamic tidal constituents that are not interpolated by the model. The
constituents declared here will be considered as part of the model components
and will not be calculated by admittance and excluded from the long-period
equilibrium wave calculation routine (`lpe_minus_n_waves`).
)__doc__")
      .def("clear", &fes::AbstractTidalModel<T, ConstituentId>::clear,
           "Clear the loaded wave models from memory.")
      .def_property_readonly(
          "tide_type", &fes::AbstractTidalModel<T, ConstituentId>::tide_type,
          "Return the type of tide.")
      .def("identifiers",
           &fes::AbstractTidalModel<T, ConstituentId>::identifiers,
           "Return the identifiers of the loaded wave models.")
      .def("__len__", &fes::AbstractTidalModel<T, ConstituentId>::size,
           "Return the number of loaded wave models.")
      .def(
          "__bool__",
          [](const fes::AbstractTidalModel<T, ConstituentId>& self) -> bool {
            return !self.empty();
          },
          "Return true if no wave models are loaded.");
}

void init_abstract_tide_model(py::module& m) {
  py::enum_<fes::TideType>(m, "TideType")
      .value("kTide", fes::TideType::kTide)
      .value("kRadial", fes::TideType::kRadial)
      .export_values();

  py::class_<fes::Accelerator<fes::Constituent>>(
      m, "Accelerator",
      "Accelerator used to speed up the interpolation of tidal models.");

  init_abstract_tide_model<double, fes::Constituent>(m, "Complex128");
  init_abstract_tide_model<float, fes::Constituent>(m, "Complex64");
}
