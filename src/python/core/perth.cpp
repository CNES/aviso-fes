#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/optional.hpp>

#include "fes/abstract_tidal_model.hpp"
#include "fes/perth/tide.hpp"
#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"

namespace py = pybind11;

template <typename T, typename ConstituentId>
auto evaluate_tide(
    const fes::AbstractTidalModel<T, ConstituentId>* const tidal_model,
    py::array& dates, const Eigen::Ref<const Eigen::VectorXd>& longitudes,
    const Eigen::Ref<const Eigen::VectorXd>& latitudes,
    const boost::optional<fes::perth::Settings>& settings,
    const size_t num_threads = 0)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, fes::Vector<fes::Quality>> {
  if (dates.size() != longitudes.size() || dates.size() != latitudes.size()) {
    throw std::invalid_argument(
        "epoch, longitudes and latitudes must have the same "
        "size");
  }
  auto epoch = fes::python::npdatetime64_to_epoch(dates);
  {
    py::gil_scoped_release gil;
    return fes::perth::evaluate_tide(tidal_model, epoch, longitudes, latitudes,
                                     settings.value_or(fes::perth::Settings()),
                                     num_threads);
  }
}

template <typename T, typename ConstituentId>
void init_tide(py::module& m) {
  m.def(
      "evaluate_tide",
      [](const fes::AbstractTidalModel<T, ConstituentId>* const tidal_model,
         py::array& date, const Eigen::Ref<const Eigen::VectorXd>& longitude,
         const Eigen::Ref<const Eigen::VectorXd>& latitude,
         const boost::optional<fes::perth::Settings>& settings,
         const size_t num_threads) {
        return evaluate_tide<T, ConstituentId>(tidal_model, date, longitude,
                                               latitude, settings, num_threads);
      },
      py::arg("tidal_model"), py::arg("date"), py::arg("longitude"),
      py::arg("latitude"), py::arg("settings") = boost::none,
      py::arg("num_threads") = 0,
      R"__doc(
Ocean tide calculation

Args:
  tidal_model: Tidal model used to interpolate the modelized waves
  date: Date of the tide calculation
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
            tidal spectrum (same units as the input constituents)
        * The height of the long period wave constituents of the tidal
            spectrum (same units as the input constituents)
        * A vector of quality flags for each point.
)__doc");
}

void init_perth_tide(py::module& m) {
  init_tide<double, fes::perth::Constituent>(m);
  init_tide<float, fes::perth::Constituent>(m);
}
