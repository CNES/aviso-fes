#pragma once

#include <Eigen/Core>
#include <array>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include "fes/abstract_tidal_model.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/nodal_corrections.hpp"
#include "fes/tidal_constituents.hpp"

namespace fes {
namespace perth {

/// @brief Class representing a set of tidal constituents and their associated
/// tide components.
/// @tparam T The type of the tide component.
template <typename T>
class WaveSet : public TidalConstituents<Constituent> {
 public:
  using value_type = T;
  using Key = std::array<Constituent, kNumConstituentItems>;
  using Item = std::array<T, kNumConstituentItems>;

  /// @brief Default constructor
  constexpr WaveSet() noexcept = default;

  /// @brief Constructor with keys and items
  /// @param[in] keys The keys of the constituent set
  /// @param[in] items The items of the constituent set
  constexpr WaveSet(Key &&keys, Item &&items) noexcept
      : keys_(std::move(keys)),
        items_(std::move(items)),
        keys_vector_(keys_.begin(), keys_.end()),
        nodal_corrections_(keys_.size()) {
    std::fill(nodal_corrections_.begin(), nodal_corrections_.end(),
              NodalCorrections{std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN()});
  }

  /// @brief Accessor operator
  constexpr auto operator[](Constituent constituent) const noexcept
      -> const T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  /// @brief Accessor operator
  /// @param[in] constituent The constituent to access
  /// @return Reference to the tide component associated with the constituent
  constexpr auto operator[](Constituent constituent) noexcept -> T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  /// @brief Get the size of the constituent set
  constexpr auto size() const noexcept -> std::size_t {
    return kNumConstituentItems;
  }

  /// @brief Get the keys of the constituent set
  constexpr auto keys() const noexcept -> const Key & { return keys_; }

  /// @brief Get the keys of the constituent set
  constexpr auto items() const noexcept -> const Item & { return items_; }

  /// @brief Get the items of the constituent set
  constexpr auto items() noexcept -> Item & { return items_; }

  /// @brief Get the keys as a vector
  constexpr auto keys_vector() const -> const std::vector<Constituent> & {
    return keys_vector_;
  }

  /// @brief Get iterators to the keys
  constexpr auto begin() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.begin();
  }

  /// @brief Get iterators to the keys
  constexpr auto end() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.end();
  }

  /// @brief Access the constituent and its associated item by index
  /// @param[in] index The index of the constituent
  /// @return A pair of the constituent and its associated item
  constexpr auto at(const size_t index) const
      -> const std::pair<Constituent, T> {
    if (index >= static_cast<size_t>(kNumConstituentItems)) {
      throw std::out_of_range("Index out of range");
    }
    return {keys_[index], items_[index]};
  }

  /// @brief Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[in] value The tide value
  void set_tide(Constituent ident, const std::complex<double> &value) override {
    (*this)[ident].tide = value;
  }

  auto update_nodal_corrections(const double epoch,
                                const bool group_modulations,
                                Accelerator<Constituent> &acc) -> void;

  auto nodal_correction(const Constituent constituent) const
      -> const NodalCorrections & {
    auto index = static_cast<std::size_t>(constituent);
    return nodal_corrections_[index];
  }

 private:
  Key keys_{};
  Item items_{};
  std::vector<Constituent> keys_vector_;
  std::vector<NodalCorrections> nodal_corrections_;
};

/// @brief Alias for a constituent table.
using WaveTable = WaveSet<Wave>;

/// @brief Assemble a constituent table from a list of constituents.
/// @param[in] constituents The list of constituents to include in the table.
/// If empty, all constituents are included.
/// @return The assembled constituent table.
auto build_table(const std::vector<Constituent> &constituents = {})
    -> WaveTable;

}  // namespace perth
}  // namespace fes
