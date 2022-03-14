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
#pragma once
#include "fes.h"
#include <chrono>
#include <mutex>
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <string>
#include <tuple>
#include <vector>

class Handler
{
private:
  std::mutex mutex_{};
  std::shared_ptr<void> fes_{ nullptr };

  void check(int status) const;

  std::vector<std::chrono::system_clock::time_point> cast_datetime(
    pybind11::array& array) const;

  std::tuple<double, double, int> calculate(
    const double lon,
    const double lat,
    const std::chrono::system_clock::time_point& date) const;

public:
  Handler(const std::string& tide,
          const std::string& mode,
          const std::string& path);

  virtual ~Handler() = default;

  Handler(const Handler&) = delete;
  Handler(Handler&&) = delete;
  Handler& operator=(const Handler&) const = delete;
  Handler& operator=(Handler&&) = delete;

  static void dump_template(const std::string& path)
  {
    auto rc = fes_dump_template(path.c_str());
    if (rc != FES_SUCCESS) {
      throw std::runtime_error(
        "an unknown error was detected during the generation of the "
        "template");
    }
  }

  void set_buffer_size(size_t size)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    check(fes_set_buffer_size(fes_.get(), size));
  }

  pybind11::tuple calculate(pybind11::array_t<double>& lon,
                            pybind11::array_t<double>& lat,
                            pybind11::array& date);
};
