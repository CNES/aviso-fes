#include "fes/interface/wave_table.hpp"

#include <memory>
#include <stdexcept>

#include "fes/darwin/wave_table.hpp"
#include "fes/perth/wave_table.hpp"

namespace fes {

auto wave_table_factory(const EngineType engine_type)
    -> std::unique_ptr<WaveTableInterface> {
  switch (engine_type) {
    case EngineType::kDarwin:
      return std::make_unique<darwin::WaveTable>();
    case EngineType::kDoodson:
      return std::make_unique<perth::WaveTable>();
    default:
      throw std::invalid_argument("Invalid engine type");
  }
}

auto wave_table_factory(const EngineType engine_type,
                        const std::vector<std::string>& constituents)
    -> std::unique_ptr<WaveTableInterface> {
  switch (engine_type) {
    case EngineType::kDarwin:
      return std::make_unique<darwin::WaveTable>(constituents);
    case EngineType::kDoodson:
      return std::make_unique<perth::WaveTable>(constituents);
    default:
      throw std::invalid_argument("Invalid engine type");
  }
}

}  // namespace fes
