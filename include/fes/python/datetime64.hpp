// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/python/datetime64.hpp
/// @brief Numpy datetime64 handling
#pragma once
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <Eigen/Core>
#include <string>

namespace fes {
namespace python {

static inline auto npdatetime64_scale(const pybind11::dtype& dtype) -> double {
  auto type_num =
      pybind11::detail::array_descriptor_proxy(dtype.ptr())->type_num;
  if (type_num != 21 /* NPY_DATETIME */) {
    throw std::invalid_argument(
        "array must be a numpy array of datetime64 items");
  }
  auto units = std::string(pybind11::str(static_cast<pybind11::handle>(dtype)));
  if (units == "datetime64[as]") {
    return 1e-18;
  }
  if (units == "datetime64[fs]") {
    return 1e-15;
  }
  if (units == "datetime64[ps]") {
    return 1e-12;
  }
  if (units == "datetime64[ns]") {
    return 1e-9;
  }
  if (units == "datetime64[us]") {
    return 1e-6;
  }
  if (units == "datetime64[ms]") {
    return 1e-3;
  }
  if (units == "datetime64[s]") {
    return 1;
  }
  if (units == "datetime64[m]") {
    return 60;
  }
  if (units == "datetime64[h]") {
    return 3600;
  }
  if (units == "datetime64[D]") {
    return 86400;
  }
  throw std::invalid_argument(
      "array has wrong datetime unit, expected datetime64[as], "
      "datetime64[fs], datetime64[ns], datetime64[us], datetime64[ms] or "
      "datetime64[s] got " +
      units);
}

static inline auto npdatetime64_to_epoch(const pybind11::array& array)
    -> Eigen::VectorXd {
  auto scale = npdatetime64_scale(array.dtype());
  auto ptr = array.unchecked<int64_t, 1>();
  auto result = Eigen::VectorXd(ptr.size());
  {
    pybind11::gil_scoped_release release;
    for (pybind11::ssize_t ix = 0; ix < ptr.size(); ++ix) {
      result(ix) = ptr(ix) * scale;
    }
  }
  return result;
}

}  // namespace python
}  // namespace fes
