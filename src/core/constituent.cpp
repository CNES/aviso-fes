// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/constituent.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/enum_mapper.hpp"
#include "fes/perth/constituent.hpp"

namespace py = pybind11;

namespace fes {

static void init_enum_mapper(py::module& m) {
  py::class_<EnumMapper<uint8_t>>(
      m, "ConstituentMap",
      "Mapper between constituent enum values and their string names.")
      .def("__getitem__", &EnumMapper<uint8_t>::from_string, py::arg("name"),
           "Get enum value by string name (case-insensitive).")
      .def("__setitem__", &EnumMapper<uint8_t>::add_entry, py::arg("value"),
           py::arg("name"), "Add an entry mapping enum value to string name.")
      .def("__len__", &EnumMapper<uint8_t>::size,
           "Get the number of entries in the mapper.")
      .def(
          "__contains__",
          [](const EnumMapper<uint8_t>& self, const std::string& name) -> bool {
            try {
              self.from_string(name);
              return true;
            } catch (const std::invalid_argument&) {
              return false;
            }
          },
          py::arg("name"), "Check if a string name exists in the mapper.")
      .def("keys", &EnumMapper<uint8_t>::keys,
           "Return list of all string names (keys) in the mapper.")
      .def("values", &EnumMapper<uint8_t>::values,
           "Return list of all enum values in the mapper.")
      .def("items", &EnumMapper<uint8_t>::items,
           "Return list of (value, name) pairs in the mapper.")
      .def(
          "get_name",
          [](const EnumMapper<uint8_t>& self, const uint8_t value)
              -> std::string { return self.to_string(value); },
          py::arg("value"), "Get the string name for a given enum value.");
}

namespace darwin {

static void init_constituents(py::module& m) {
  m.def("constituents", &constituents::map,
        R"__doc__(
  Return the constituent map between IDs and names.

  Returns:
    Constituent map.
  )__doc__");
}

}  // namespace darwin

namespace perth {

static void init_constituents(py::module& m) {
  m.def("constituents", &constituents::map,
        R"__doc__(
Return the constituent map between IDs and names.

Returns:
  Constituent map.
)__doc__");
}

}  // namespace perth
}  // namespace fes

void init_constituent(py::module& m, py::module& darwin, py::module& perth) {
  fes::init_enum_mapper(m);
  fes::darwin::init_constituents(darwin);
  fes::perth::init_constituents(perth);
}
