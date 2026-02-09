// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pyerrors.h>

#include <cstdint>
#include <stdexcept>

#include "fes/constituent.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep

namespace py = pybind11;

namespace fes {

inline void init_constituent(py::module& m) {
  // Register the ConstituentValidationError exception
  py::register_exception<ConstituentValidationError>(
      m, "ConstituentValidationError", PyExc_ValueError);

  m.def(
      "parse_constituent",
      [](const std::string& name) -> int64_t {
        return static_cast<int64_t>(constituents::parse(name));
      },
      "Parse a constituent name into its corresponding identifier.",
      py::arg("name"));
  m.def(
      "known_constituents",
      []() -> std::vector<std::string> {
        const auto size = static_cast<uint8_t>(ConstituentId::kNumConstituents);
        std::vector<std::string> names;
        names.reserve(size);
        for (int8_t ix = 0; ix < size; ++ix) {
          names.emplace_back(
              constituents::name(static_cast<ConstituentId>(ix)));
        }
        return names;
      },
      "Return a list of all known constituent names.");
}

}  // namespace fes

auto init_constituents(py::module& m) -> void { fes::init_constituent(m); }
