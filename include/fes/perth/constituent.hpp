#pragma once

#include <Eigen/Core>
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fes/eigen.hpp"
#include "fes/tidal_constituents.hpp"

namespace fes {
namespace perth {

/// @brief Possible type of tidal wave.
enum ConstituentType : uint8_t {
  kLongPeriod = 0,  //!< Long period tidal waves
  kShortPeriod      //!< Short period tidal waves
};

/// @brief Enum class representing tidal constituents.
enum Constituent : uint8_t {
  kNode,
  kSa1,
  kSa,
  kSsa,
  kSta,
  kMSm,
  kMm,
  kMSf,
  kMf,
  kMStm,
  kMtm,
  kMSqm,
  kMqm,
  k2Q1,
  kSigma1,
  kQ1,
  kRho1,
  kO1,
  kTau1,
  kBeta1,
  kM13,
  kM1,
  kChi1,
  kPi1,
  kP1,
  kS1,
  kK1,
  kPsi1,
  kPhi1,
  kTheta1,
  kJ1,
  kSO1,
  kOO1,
  kUps1,
  kEps2,
  k2N2,
  k2MS2,
  kMu2,
  kN2,
  kN2P,
  kNu2,
  kGamma2,
  kAlpa2,
  kM2,
  kBeta2,
  kDelta2,
  kMKS2,
  kLambda2,
  k2MN2,
  kL2,
  kL2P,
  kT2,
  kS2,
  kR2,
  kK2,
  kMSN2,
  kEta2,
  k2SM2,
  k2MK3,
  kMO3,
  kM3,
  kMK3,
  kN4,
  kMN4,
  kM4,
  kSN4,
  kMS4,
  kMK4,
  kS4,
  kR4,
  kSK4,
  k2MN6,
  kM6,
  kMSN6,
  k2MS6,
  k2MK6,
  k2SM6,
  kMSK6,
  kS6,
  kM8,
  kNumConstituents,
};

/// @brief Data structure that holds the tide of a constituent.
struct Wave {
  Vector7b doodson_number;    //!< Doodson number of the constituent
  std::complex<double> tide;  //!< Tide of the constituent
  double tidal_argument;      //!< Doodson argument
  ConstituentType type;       //!< Type of tidal wave
  bool is_inferred = false;   //!< Whether the tide was inferred from the
                              //!< constituents
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

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
        keys_vector_(keys_.begin(), keys_.end()) {}

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

 private:
  Key keys_{};
  Item items_{};
  std::vector<Constituent> keys_vector_;
};

/// @brief Alias for a constituent table.
using WaveTable = WaveSet<Wave>;

/// @brief Assemble a constituent table from a list of constituents.
/// @param[in] constituents The list of constituents to include in the table.
/// If empty, all constituents are included.
/// @return The assembled constituent table.
auto build_table(const std::vector<Constituent> &constituents = {})
    -> WaveTable;

namespace constituents {

/// @brief Convert a constituent enum to its string name.
/// @param[in] constituent The constituent to convert.
/// @return The string name of the constituent.
auto name(Constituent constituent) -> std::string;

/// @brief Parse a constituent name string to its enum value.
/// @param[in] name The name of the constituent.
/// @return The corresponding Constituent enum value.
auto parse(const std::string &name) -> Constituent;

}  // namespace constituents

}  // namespace perth
}  // namespace fes
