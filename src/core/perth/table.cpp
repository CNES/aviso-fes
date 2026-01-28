#include "fes/perth/table.hpp"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>

#include "fes/perth/constituent.hpp"
#include "fes/perth/tidal_frequency.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep
#include "fes/xdo.hpp"

namespace py = pybind11;

namespace fes {
namespace perth {

static auto init_table(py::module &m) -> void {
  py::class_<perth::Wave>(
      m, "Wave", "Data structure that holds the tide of a constituent.")
      .def_property_readonly(
          "doodson_number",
          [](const perth::Wave &self) -> Eigen::Vector<int8_t, 7> {
            return self.doodson_number;
          },
          "Doodson number of the constituent.")
      .def_property_readonly(
          "tide",
          [](const perth::Wave &self) -> std::complex<double> {
            return self.tide;
          },
          "Tide of the constituent.")
      .def_property_readonly(
          "tidal_argument",
          [](const perth::Wave &self) -> double { return self.tidal_argument; },
          "Doodson argument of the constituent.")
      .def_property_readonly(
          "type",
          [](const perth::Wave &self) -> TidalType { return self.type; },
          "Type of tidal wave.")
      .def_property_readonly(
          "is_inferred",
          [](const perth::Wave &self) -> bool { return self.is_inferred; },
          "Whether the tide was inferred from the constituents.")
      .def_property_readonly(
          "freq",
          [](const perth::Wave &self) -> double {
            return tidal_frequency(self.doodson_number.head(6).cast<double>());
          },
          "Get the frequency of the tidal constituent.")
      .def(
          "xdo_numerical",
          [](const perth::Wave &self) -> std::string {
            return xdo_numerical(self.doodson_number);
          },
          "Get the XDO numerical representation of the wave.")
      .def(
          "xdo_alphabetical",
          [](const perth::Wave &self) -> std::string {
            return xdo_alphabetical(self.doodson_number);
          },
          "Get the XDO alphabetical representation of the wave.")
      .def(
          "name",
          [](const perth::Wave &self) -> std::string {
            auto wt = WaveTable{};
            for (const auto id : wt.keys()) {
              const auto &wave = wt[id];
              if (wave.doodson_number == self.doodson_number &&
                  wave.tide == self.tide) {
                return constituents::name(id);
              }
            }
            return std::string{"Unknown"};
          },
          "Get the name of the wave.");

  class WaveIterator {
   public:
    using pointer = const perth::Wave *;
    using reference = const perth::Wave &;

    WaveIterator(const perth::WaveTable *table,
                 std::vector<Constituent>::const_iterator it)
        : table_(table), current_(it) {}

    auto operator*() const -> reference { return (*table_)[*current_]; }

    auto operator->() const -> pointer { return &(*table_)[*current_]; }

    auto operator++() -> WaveIterator & {
      ++current_;
      return *this;
    }

    auto operator++(int) -> WaveIterator {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator==(const WaveIterator &other) const -> bool {
      return current_ == other.current_;
    }

    auto operator!=(const WaveIterator &other) const -> bool {
      return !(*this == other);
    }

   private:
    const perth::WaveTable *table_;
    std::vector<Constituent>::const_iterator current_;
  };

  py::class_<perth::WaveTable>(m, "WaveTable", "Table of tidal constituents.")
      .def(
          py::init([](const boost::optional<std::vector<std::string>>
                          &constituents) {
            if (constituents) {
              std::vector<Constituent> ids;
              ids.reserve(constituents->size());
              for (const auto &name : *constituents) {
                ids.push_back(constituents::parse(name));
              }
              return perth::WaveTable(ids);
            } else {
              return perth::WaveTable();
            }
          }),
          py::arg("constituents") = boost::none,
          "Create a WaveTable from a list of constituent names. If the list is "
          "empty, all constituents are included.")
      .def(
          "__len__",
          [](const perth::WaveTable &self) -> size_t { return self.size(); },
          "Returns the number of constituents in the table.")
      .def(
          "__getitem__",
          [](const perth::WaveTable &self,
             std::string constituent) -> const Wave & {
            auto id = constituents::parse(constituent);
            return self[id];
          },
          py::arg("constituent"), "Get a constituent by its enum value.")
      .def(
          "__contains__",
          [](const perth::WaveTable &self,
             const std::string &constituent) -> bool {
            try {
              return self.contains(constituents::parse(constituent));
            } catch (...) {
              return false;
            }
          },
          py::arg("constituent"),
          "Check if a constituent is in the table by its name.")
      .def(
          "__iter__",
          [](const perth::WaveTable &self) {
            return py::make_iterator(WaveIterator(&self, self.begin()),
                                     WaveIterator(&self, self.end()));
          },
          py::keep_alive<0, 1>(), "Iterate over the waves in the table.")
      .def(
          "keys",
          [](const perth::WaveTable &self) {
            std::vector<std::string> keys;
            keys.reserve(self.size());
            for (const auto &wave : self.keys()) {
              keys.push_back(constituents::name(wave));
            }
            return keys;
          },
          "Get the list of constituent names in the table.")
      .def(
          "values",
          [](const perth::WaveTable &self) {
            std::vector<perth::Wave> values;
            values.reserve(self.size());
            for (const auto &wave : self.items()) {
              values.push_back(wave);
            }
            return values;
          },
          "Get the list of waves in the table.");
}

}  // namespace perth
}  // namespace fes

auto init_perth_table(py::module &m) -> void { fes::perth::init_table(m); }
