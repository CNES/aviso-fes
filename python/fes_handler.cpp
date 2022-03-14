// This file is part of FES library.
//
// FES is free software: you can redistribute it and/or modify
// it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FES is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU LESSER GENERAL PUBLIC LICENSE for more details.
//
// You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
// along with FES.  If not, see <http://www.gnu.org/licenses/>.
#include "fes_handler.hpp"

#include <datetime.h>

#include <iostream>

/// Returns number of days since civil 1970-01-01.  Negative values indicate
/// days prior to 1970-01-01.
///
/// http://howardhinnant.github.io/date_algorithms.html
static inline int64_t
days_from_civil(int y, unsigned m, unsigned d)
{
  y -= static_cast<int>(m <= 2);
  const auto era = (y >= 0 ? y : y - 399) / 400;
  const auto yoe = static_cast<unsigned>(y - era * 400);           // [0, 399]
  const auto doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
  const auto doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
  return era * 146097LL + static_cast<int64_t>(doe) - 719468LL;
}

std::chrono::system_clock::time_point
timestamp(pybind11::handle datetime)
{
  if (!datetime) {
    throw std::invalid_argument(
      "a datetime.datetime is required (got type null)");
  }

  if (!PyDateTimeAPI) {
    PyDateTime_IMPORT;
  }

  if (PyDateTime_Check(datetime.ptr())) {
    if (reinterpret_cast<_PyDateTime_BaseTZInfo*>(datetime.ptr())->hastzinfo) {
      throw std::invalid_argument(
        "only the naive datetime object can be converted to timestamp");
    }

    auto sec = days_from_civil(PyDateTime_GET_YEAR(datetime.ptr()),
                               PyDateTime_GET_MONTH(datetime.ptr()),
                               PyDateTime_GET_DAY(datetime.ptr())) *
               86400;

    sec += PyDateTime_DATE_GET_HOUR(datetime.ptr()) * 3600 +
           PyDateTime_DATE_GET_MINUTE(datetime.ptr()) * 60 +
           PyDateTime_DATE_GET_SECOND(datetime.ptr());

    return std::chrono::system_clock::from_time_t(sec) +
           std::chrono::microseconds(
             PyDateTime_DATE_GET_MICROSECOND(datetime.ptr()));
  }

  throw std::invalid_argument(
    "a datetime.datetime is required (got type " +
    std::string(pybind11::str(datetime.get_type().attr("__name__"))) + ")");
}

void
Handler::check(const int status) const
{
  if (status != FES_SUCCESS) {
    throw std::runtime_error(fes_error(fes_.get()));
  }
}

std::vector<std::chrono::system_clock::time_point>
Handler::cast_datetime(pybind11::array& array) const
{
  auto result = std::vector<std::chrono::system_clock::time_point>();
  auto size = array.size();
  result.reserve(size);
  auto type_num =
    pybind11::detail::array_descriptor_proxy(array.dtype().ptr())->type_num;
  if (type_num == pybind11::detail::npy_api::NPY_OBJECT_) {
    for (auto& item : array) {
      result.emplace_back(timestamp(item));
    }
  } else if (type_num == 21 /* NPY_DATETIME */) {
    auto dtype = std::string(pybind11::str(array.dtype()));
    if (dtype != "datetime64[us]") {
      throw std::invalid_argument(
        "date has wrong datetime unit, expected datetime64[us], "
        "got " +
        dtype);
    }
    pybind11::array_t<int64_t> data = array;
    auto _data = data.unchecked<1>();
    for (pybind11::ssize_t ix = 0; ix < size; ++ix) {
      auto epoch = _data[ix];
      result.emplace_back(
        std::chrono::system_clock::from_time_t(epoch / 1'000'000) +
        std::chrono::microseconds(epoch % 1'000'000));
    }
  }
  return result;
}

Handler::Handler(const std::string& tide,
                 const std::string& mode,
                 const std::string& path)
{
  fes_enum_access access_mode;
  fes_enum_tide_type tide_type;

  if (tide == "ocean") {
    tide_type = FES_TIDE;
  } else if (tide == "radial") {
    tide_type = FES_RADIAL;
  } else {
    throw std::invalid_argument("tide must be 'ocean' or 'radial' not '" +
                                tide + "'");
  }

  if (mode == "io") {
    access_mode = FES_IO;
  } else if (mode == "memory") {
    access_mode = FES_MEM;
  } else {
    throw std::invalid_argument("mode must be 'io' or 'memory' not '" + tide +
                                "'");
  }

  FES fes;

  auto status = fes_new(&fes, tide_type, access_mode, path.c_str());
  if (fes == nullptr) {
    throw std::runtime_error("not enough memory to allocate the FES handler");
  }
  fes_ = std::shared_ptr<void>(fes, [](FES fes) { fes_delete(fes); });
  check(status);
}

std::tuple<double, double, int>
Handler::calculate(const double lon,
                   const double lat,
                   const std::chrono::system_clock::time_point& date) const
{
  double h, h_long_period;
  auto epoch = std::chrono::duration_cast<std::chrono::microseconds>(
                 date.time_since_epoch())
                 .count();
  auto status = fes_core(fes_.get(),
                         lat,
                         lon,
                         ((epoch * 1e-6) / 86400.0) + 7305,
                         &h,
                         &h_long_period);
  if (status == 1) {
    if (fes_errno(fes_.get()) == FES_NO_DATA) {
      return std::make_tuple(std::numeric_limits<double>::quiet_NaN(),
                             std::numeric_limits<double>::quiet_NaN(),
                             0);
    }
    throw std::runtime_error(fes_error(fes_.get()));
  }
  return std::make_tuple(h, h_long_period, fes_min_number(fes_.get()));
}

pybind11::tuple
Handler::calculate(pybind11::array_t<double>& lon,
                   pybind11::array_t<double>& lat,
                   pybind11::array& date)
{
  // arrays must one-dimensionnal
  if (lon.ndim() != 1) {
    throw std::invalid_argument("lon must be a one-dimensional array");
  }
  if (lat.ndim() != 1) {
    throw std::invalid_argument("lat must be a one-dimensional array");
  }
  if (date.ndim() != 1) {
    throw std::invalid_argument("date must be a one-dimensional array");
  }

  auto size = lon.size();

  // arrays must have identical dimensions
  if (size != lat.size()) {
    throw std::invalid_argument(
      "lon, lat couldn't be broadcast together with shape (" +
      std::to_string(size) + ",) (" + std::to_string(lat.size()) + ",)");
  }
  if (size != date.size()) {
    throw std::invalid_argument(
      "lon, date couldn't be broadcast together with shape (" +
      std::to_string(size) + ",) (" + std::to_string(date.size()) + ",)");
  }

  // Cast python date to C++ objects
  auto _date = cast_datetime(date);
  if (_date.size() == 0) {
    throw std::invalid_argument(
      "Incompatible arguments:\n"
      " 1. calculate(self, numpy.ndarray[m, 1], numpy.ndarray[m, 1], "
      "numpy.ndarray[datetime.datetime[m, 1]])\n"
      " 2. calculate(self, numpy.ndarray[m, 1], numpy.ndarray[m, 1], "
      "numpy.ndarray[datetime64[us][m, 1]])");
  }

  // Allocates results
  pybind11::array_t<double> h(pybind11::array::ShapeContainer{ size });
  pybind11::array_t<double> h_long_period(
    pybind11::array::ShapeContainer{ size });
  pybind11::array_t<double> samples(pybind11::array::ShapeContainer{ size });

  auto _lon = lon.unchecked<1>();
  auto _lat = lat.unchecked<1>();
  auto _h = h.mutable_unchecked<1>();
  auto _h_long_period = h_long_period.mutable_unchecked<1>();
  auto _samples = samples.mutable_unchecked<1>();
  {
    pybind11::gil_scoped_release gil;
    std::unique_lock<std::mutex> lock(mutex_);

    for (pybind11::ssize_t ix = 0; ix < size; ++ix) {
      std::tie(_h(ix), _h_long_period(ix), _samples(ix)) =
        calculate(_lon[ix], _lat[ix], _date[ix]);
    }
  }
  return pybind11::make_tuple(h, h_long_period, samples);
}
