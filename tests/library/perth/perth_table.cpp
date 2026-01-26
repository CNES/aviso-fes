#include <gtest/gtest.h>

#include <algorithm>
#include <complex>
#include <cstddef>
#include <vector>

#include "fes/perth/constituent.hpp"
#include "fes/perth/table.hpp"

namespace fes {
namespace perth {

// Test default constructor
TEST(WaveSetTest, DefaultConstructor) {
  WaveSet<Wave> array;
  EXPECT_EQ(array.size(), kNumConstituentItems);
  EXPECT_EQ(array.size(),
            static_cast<std::size_t>(Constituent::kNumConstituents));
}

// Test constructor with keys and items
TEST(WaveSetTest, ConstructorWithKeysAndItems) {
  WaveSet<Wave>::Key keys;
  WaveSet<Wave>::Item items;

  // Fill with test data
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    keys[i] = static_cast<Constituent>(i);
    items[i].tide = {static_cast<double>(i), static_cast<double>(i + 1)};
  }

  WaveSet<Wave> array(std::move(keys), std::move(items));

  EXPECT_EQ(array.size(), kNumConstituentItems);

  // Verify data integrity
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = static_cast<Constituent>(i);
    EXPECT_EQ(array[constituent].tide.real(), static_cast<double>(i));
    EXPECT_EQ(array[constituent].tide.imag(), static_cast<double>(i + 1));
  }
}

// Test const and non-const operator[]
TEST(WaveSetTest, IndexOperator) {
  WaveSet<Wave> array;

  // Test non-const operator[]
  array[Constituent::kNode].tide = {1.5, 2.5};
  array[Constituent::kM2].tide = {3.7, 4.2};

  // Test const operator[]
  const auto& const_array = array;
  EXPECT_EQ(const_array[Constituent::kNode].tide.real(), 1.5);
  EXPECT_EQ(const_array[Constituent::kNode].tide.imag(), 2.5);
  EXPECT_EQ(const_array[Constituent::kM2].tide.real(), 3.7);
  EXPECT_EQ(const_array[Constituent::kM2].tide.imag(), 4.2);
}

// Test keys() method
TEST(WaveSetTest, Keys) {
  WaveSet<Wave>::Key test_keys;
  WaveSet<Wave>::Item test_items;

  // Fill with specific test data
  test_keys[0] = Constituent::kNode;
  test_keys[1] = Constituent::kSa;
  test_keys[2] = Constituent::kM2;

  WaveSet<Wave> array(std::move(test_keys), std::move(test_items));

  const auto& keys = array.keys();
  EXPECT_EQ(keys[0], Constituent::kNode);
  EXPECT_EQ(keys[1], Constituent::kSa);
  EXPECT_EQ(keys[2], Constituent::kM2);
}

// Test items() methods
TEST(WaveSetTest, Items) {
  WaveSet<Wave> array;

  // Test non-const items()
  auto& items = array.items();
  items[0].tide = {1.0, 2.0};

  // Test const items()
  const auto& const_array = array;
  const auto& const_items = const_array.items();
  EXPECT_EQ(const_items[0].tide.real(), 1.0);
  EXPECT_EQ(const_items[0].tide.imag(), 2.0);
}

// Test keys_vector method
TEST(WaveSetTest, RetrieveKeysAsVector) {
  WaveSet<Wave>::Key test_keys;
  WaveSet<Wave>::Item test_items;

  // Fill keys with specific constituents
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    test_keys[i] = static_cast<Constituent>(i);
  }

  WaveSet<Wave> array(std::move(test_keys), std::move(test_items));

  auto keys_vector = array.keys_vector();
  EXPECT_EQ(keys_vector.size(), kNumConstituentItems);

  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    EXPECT_EQ(keys_vector[i], static_cast<Constituent>(i));
  }
}

// Tests for make_tide_table function
class WaveTableTest : public ::testing::Test {
 private:
  static std::vector<Constituent> enabled_;

 protected:
  WaveTable table = WaveTable(WaveTableTest::enabled_);

  auto is_enabled(Constituent constituent) const -> bool {
    return std::find(enabled_.begin(), enabled_.end(), constituent) !=
           enabled_.end();
  }
};

std::vector<Constituent> WaveTableTest::enabled_ = {
    kQ1, kO1, kP1,  kS1,  kK1,  kN2, kM2,     kS2,
    kK2, kM4, kMS4, k2N2, kMu2, kJ1, kSigma1, kOO1};

TEST_F(WaveTableTest, TableSize) {
  EXPECT_EQ(table.size(), kNumConstituentItems);
  EXPECT_EQ(table.size(),
            static_cast<std::size_t>(Constituent::kNumConstituents));
}

TEST_F(WaveTableTest, AllConstituentsPresent) {
  auto keys_vector = table.keys_vector();

  // Check that all constituent values from 0 to kNumConstituents-1 are present
  std::vector<bool> found(kNumConstituentItems, false);
  for (auto constituent : keys_vector) {
    std::size_t index = static_cast<std::size_t>(constituent);
    ASSERT_LT(index, kNumConstituentItems);
    found[index] = true;
  }

  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    EXPECT_TRUE(found[i]) << "Constituent " << i << " not found in table";
  }
}

TEST_F(WaveTableTest, InitialValues) {
  // All tide table values should be initialized to (0, 0)
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = static_cast<Constituent>(i);
    const auto& component = table[constituent];
    EXPECT_EQ(component.tide.real(), 0.0)
        << "Real part should be 0 for constituent " << i;
    EXPECT_EQ(component.tide.imag(), 0.0)
        << "Imaginary part should be 0 for constituent " << i;
    EXPECT_EQ(component.is_inferred, !this->is_enabled(constituent))
        << "Inferred status should match for constituent " << i;
  }
}

TEST_F(WaveTableTest, DataConsistency) {
  // Verify that keys match the constituent indices
  const auto& keys = table.keys();
  for (std::size_t i = 0; i < kNumConstituentItems; ++i) {
    Constituent constituent = keys[i];
    std::size_t constituent_index = static_cast<std::size_t>(constituent);
    EXPECT_EQ(constituent_index, i)
        << "Key at position " << i << " should match its constituent value";
  }
}

TEST_F(WaveTableTest, Modifiability) {
  // Test that we can modify tide table values
  table[Constituent::kM2].tide = {1.5, 2.3};
  table[Constituent::kS2].tide = {-0.7, 1.2};

  EXPECT_EQ(table[Constituent::kM2].tide.real(), 1.5);
  EXPECT_EQ(table[Constituent::kM2].tide.imag(), 2.3);
  EXPECT_EQ(table[Constituent::kS2].tide.real(), -0.7);
  EXPECT_EQ(table[Constituent::kS2].tide.imag(), 1.2);
}

}  // namespace perth
}  // namespace fes
