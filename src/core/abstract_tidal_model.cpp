// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/abstract_tidal_model.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>

#include "fes/darwin/constituent.hpp"
#include "fes/darwin/table.hpp"
#include "fes/detail/thread.hpp"
#include "fes/perth/table.hpp"

namespace py = pybind11;

namespace fes {

inline auto init_tide_type(py::module& m) -> void {
  py::enum_<fes::TideType>(m, "TideType")
      .value("TIDE", fes::TideType::kTide)
      .value("RADIAL", fes::TideType::kRadial)
      .export_values();
}

inline auto init_accelerator(py::module& m) -> void {
  py::class_<Accelerator>(
      m, "Accelerator",
      "Accelerator used to speed up the interpolation of tidal models.");
}

// Trampoline class for AbstractTidalModel
template <typename T>
class PyAbstractTidalModel : public AbstractTidalModel<T> {
 private:
  using cname = AbstractTidalModel<T>;

 public:
  using AbstractTidalModel<T>::AbstractTidalModel;

  auto accelerator(const angle::Formulae& formulae,
                   const double time_tolerance) const -> Accelerator* override {
    PYBIND11_OVERLOAD_PURE(Accelerator*, cname, accelerator, formulae,
                           time_tolerance);
  }

  auto interpolate(const geometry::Point& point, Quality& quality,
                   Accelerator* acc) const
      -> const ConstituentValues& override {
    PYBIND11_OVERLOAD_PURE(const ConstituentValues&, cname, interpolate, point,
                           quality, acc);
  }

  void add_constituent(const uint8_t ident,
                       Vector<std::complex<T>> wave) override {
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
template <typename T>
static auto interpolate(const AbstractTidalModel<T>& self,
                        const Eigen::Ref<const Eigen::VectorXd>& lon,
                        const Eigen::Ref<const Eigen::VectorXd>& lat,
                        const size_t num_threads = 0)
    -> std::tuple<std::map<std::string, Eigen::VectorXcd>,
                  Eigen::Matrix<int8_t, -1, 1>> {
  if (lon.size() != lat.size()) {
    throw std::invalid_argument("lon and lat must have the same size");
  }
  // Allocate result vectors
  auto values = std::map<uint8_t, Eigen::VectorXcd>();
  auto qualities = Eigen::Matrix<int8_t, -1, 1>(lon.size());

  for (auto&& ident : self.identifiers()) {
    values[ident] = Eigen::VectorXcd(lon.size());
  }

  // Interpolate in parallel
  auto thread = [&](const int64_t start, const int64_t end) -> void {
    auto acc = std::unique_ptr<Accelerator>(
        self.accelerator(angle::Formulae::kSchuremanOrder1, 0.0));
    auto* acc_ptr = acc.get();
    for (auto ix = start; ix < end; ++ix) {
      auto quality = kUndefined;
      for (auto&& item :
           self.interpolate({lon[ix], lat[ix]}, quality, acc_ptr)) {
        values[std::get<0>(item)][ix] =
            quality == kUndefined ? std::numeric_limits<double>::quiet_NaN()
                                  : std::get<1>(item);
      }
      qualities[ix] = static_cast<int8_t>(quality);
    }
  };

  detail::parallel_for(thread, lon.size(), num_threads);
  auto result = std::map<std::string, Eigen::VectorXcd>();
  for (auto&& item : values) {
    result[self.enum_mapper().to_string(item.first)] = std::move(item.second);
  }
  return std::make_tuple(result, qualities);
}

template <typename T>
void init_abstract_tidal_model(py::module& m, const std::string& postfix) {
  py::class_<AbstractTidalModel<T>, PyAbstractTidalModel<T>,
             std::shared_ptr<AbstractTidalModel<T>>>(
      m, ("AbstractTidalModel" + postfix).c_str(),
      "Abstract class for a tidal model.")
      .def(
          "add_constituent",
          [](AbstractTidalModel<T>& self, const std::string& name,
             Vector<std::complex<T>> wave) -> void {
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
      .def("accelerator", &AbstractTidalModel<T>::accelerator,
           py::arg("formulae") = angle::Formulae::kSchuremanOrder1,
           py::arg("time_tolerance") = 0.0,
           py::call_guard<py::gil_scoped_release>(),
           R"__doc__(
Get the accelerator used to speed up the interpolation of the tidal model.

Returns:
  The accelerator.
)__doc__")
      .def("interpolate", &interpolate<T>, py::arg("lon"), py::arg("lat"),
           py::arg("num_threads") = 0, py::call_guard<py::gil_scoped_release>(),
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
      .def_property(
          "dynamic",
          [](const AbstractTidalModel<T>& self) -> std::vector<std::string> {
            return self.dynamic();
          },
          [](AbstractTidalModel<T>& self,
             const std::vector<std::string>& dynamic) -> void {
            self.dynamic(dynamic);
          },
          R"__doc__(
Dynamic tidal constituents that are not interpolated by the model. The
constituents declared here will be considered as part of the model components
and will not be calculated by admittance and excluded from the long-period
equilibrium wave calculation routine (`lpe_minus_n_waves`).
)__doc__")
      .def("clear", &AbstractTidalModel<T>::clear,
           "Clear the loaded wave models from memory.")
      .def(
          "identifiers",
          [](const AbstractTidalModel<T>& self) -> std::vector<std::string> {
            auto ids = std::vector<std::string>();
            ids.reserve(self.size());
            for (auto&& ident : self.identifiers()) {
              ids.push_back(self.enum_mapper().to_string(ident));
            }
            return ids;
          },
          "Return the identifiers of the loaded wave models.")
      .def("__len__", &AbstractTidalModel<T>::size,
           "Return the number of loaded wave models.")
      .def(
          "__bool__",
          [](const AbstractTidalModel<T>& self) -> bool {
            return !self.empty();
          },
          "Return true if no wave models are loaded.");
}

}  // namespace fes

void init_abstract_tidal_model(py::module& m) {
  fes::init_tide_type(m);
  fes::init_accelerator(m);
  fes::init_abstract_tidal_model<double>(m, "Complex128");
  fes::init_abstract_tidal_model<float>(m, "Complex64");
}
