// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/enum_mapper.hpp"

#include <gtest/gtest.h>

namespace fes {

// Test enum for testing purposes
enum class Color : uint8_t { Red = 0, Green = 1, Blue = 2 };

// Test fixture for EnumMapper tests
class EnumMapperTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mapper_.add_entry(static_cast<uint8_t>(Color::Red), "red");
    mapper_.add_entry(static_cast<uint8_t>(Color::Green), "green");
    mapper_.add_entry(static_cast<uint8_t>(Color::Blue), "blue");
    mapper_.finalize();
  }

  EnumMapper<uint8_t> mapper_;
};

// Test basic construction and size
TEST_F(EnumMapperTest, SizeReturnsCorrectCount) {
  EXPECT_EQ(mapper_.size(), 3);
}

// Test reserve
TEST(EnumMapperBasicTest, ReserveDoesNotThrow) {
  EnumMapper<int> mapper;
  EXPECT_NO_THROW(mapper.reserve(10));
}

// Test add_entry
TEST(EnumMapperBasicTest, AddEntryIncreasesSize) {
  EnumMapper<int8_t> mapper;
  EXPECT_EQ(mapper.size(), 0);

  mapper.add_entry(static_cast<int8_t>(Color::Red), "red");
  EXPECT_EQ(mapper.size(), 1);

  mapper.add_entry(static_cast<int8_t>(Color::Green), "green");
  EXPECT_EQ(mapper.size(), 2);
}

// Test from_string with exact match
TEST_F(EnumMapperTest, FromStringExactMatch) {
  EXPECT_EQ(mapper_.from_string("red"), static_cast<uint8_t>(Color::Red));
  EXPECT_EQ(mapper_.from_string("green"), static_cast<uint8_t>(Color::Green));
  EXPECT_EQ(mapper_.from_string("blue"), static_cast<uint8_t>(Color::Blue));
}

// Test from_string with case insensitive match
TEST_F(EnumMapperTest, FromStringCaseInsensitive) {
  EXPECT_EQ(mapper_.from_string("RED"), static_cast<uint8_t>(Color::Red));
  EXPECT_EQ(mapper_.from_string("Red"), static_cast<uint8_t>(Color::Red));
  EXPECT_EQ(mapper_.from_string("rEd"), static_cast<uint8_t>(Color::Red));
  EXPECT_EQ(mapper_.from_string("GREEN"), static_cast<uint8_t>(Color::Green));
  EXPECT_EQ(mapper_.from_string("BLUE"), static_cast<uint8_t>(Color::Blue));
}

// Test from_string with invalid key
TEST_F(EnumMapperTest, FromStringInvalidKeyThrows) {
  EXPECT_THROW(mapper_.from_string("yellow"), std::invalid_argument);
  EXPECT_THROW(mapper_.from_string(""), std::invalid_argument);
  EXPECT_THROW(mapper_.from_string("notacolor"), std::invalid_argument);
}

// Test to_string with valid values
TEST_F(EnumMapperTest, ToStringValidValue) {
  EXPECT_EQ(mapper_.to_string(static_cast<uint8_t>(Color::Red)), "red");
  EXPECT_EQ(mapper_.to_string(static_cast<uint8_t>(Color::Green)), "green");
  EXPECT_EQ(mapper_.to_string(static_cast<uint8_t>(Color::Blue)), "blue");
}

// Test to_string with invalid value
TEST_F(EnumMapperTest, ToStringInvalidValueThrows) {
  EXPECT_THROW(mapper_.to_string(static_cast<uint8_t>(99)),
               std::invalid_argument);
}

// Test exists with valid values
TEST_F(EnumMapperTest, ExistsValidValue) {
  EXPECT_TRUE(mapper_.exists(static_cast<uint8_t>(Color::Red)));
  EXPECT_TRUE(mapper_.exists(static_cast<uint8_t>(Color::Green)));
  EXPECT_TRUE(mapper_.exists(static_cast<uint8_t>(Color::Blue)));
}

// Test exists with invalid values
TEST_F(EnumMapperTest, ExistsInvalidValue) {
  EXPECT_FALSE(mapper_.exists(static_cast<uint8_t>(99)));
  EXPECT_FALSE(mapper_.exists(static_cast<uint8_t>(-1)));
}

// Test keys method
TEST_F(EnumMapperTest, KeysReturnsAllKeys) {
  auto keys = mapper_.keys();
  EXPECT_EQ(keys.size(), 3);
  EXPECT_NE(std::find(keys.begin(), keys.end(), "red"), keys.end());
  EXPECT_NE(std::find(keys.begin(), keys.end(), "green"), keys.end());
  EXPECT_NE(std::find(keys.begin(), keys.end(), "blue"), keys.end());
}

// Test values method
TEST_F(EnumMapperTest, ValuesReturnsAllValues) {
  auto values = mapper_.values();
  EXPECT_EQ(values.size(), 3);
  EXPECT_NE(
      std::find(values.begin(), values.end(), static_cast<uint8_t>(Color::Red)),
      values.end());
  EXPECT_NE(std::find(values.begin(), values.end(),
                      static_cast<uint8_t>(Color::Green)),
            values.end());
  EXPECT_NE(std::find(values.begin(), values.end(),
                      static_cast<uint8_t>(Color::Blue)),
            values.end());
}

// Test items method
TEST_F(EnumMapperTest, ItemsReturnsAllItems) {
  auto items = mapper_.items();
  EXPECT_EQ(items.size(), 3);

  // Check that all expected pairs exist
  bool found_red = false;
  bool found_green = false;
  bool found_blue = false;

  for (const auto& item : items) {
    if (item.first == static_cast<uint8_t>(Color::Red) &&
        item.second == "red") {
      found_red = true;
    } else if (item.first == static_cast<uint8_t>(Color::Green) &&
               item.second == "green") {
      found_green = true;
    } else if (item.first == static_cast<uint8_t>(Color::Blue) &&
               item.second == "blue") {
      found_blue = true;
    }
  }

  EXPECT_TRUE(found_red);
  EXPECT_TRUE(found_green);
  EXPECT_TRUE(found_blue);
}

// Test empty mapper
TEST(EnumMapperBasicTest, EmptyMapper) {
  EnumMapper<int8_t> mapper;
  EXPECT_EQ(mapper.size(), 0);
  EXPECT_TRUE(mapper.keys().empty());
  EXPECT_TRUE(mapper.values().empty());
  EXPECT_TRUE(mapper.items().empty());
}

// Test with integer enum
TEST(EnumMapperBasicTest, IntegerEnumSupport) {
  EnumMapper<int> mapper;
  mapper.add_entry(1, "one");
  mapper.add_entry(2, "two");
  mapper.add_entry(3, "three");
  mapper.finalize();

  EXPECT_EQ(mapper.from_string("one"), 1);
  EXPECT_EQ(mapper.from_string("TWO"), 2);
  EXPECT_EQ(mapper.to_string(3), "three");
  EXPECT_TRUE(mapper.exists(1));
  EXPECT_FALSE(mapper.exists(4));
}

// Test round-trip conversion
TEST_F(EnumMapperTest, RoundTripConversion) {
  // Enum to string to enum
  auto str = mapper_.to_string(static_cast<uint8_t>(Color::Red));
  auto value = mapper_.from_string(str);
  EXPECT_EQ(value, static_cast<uint8_t>(Color::Red));

  str = mapper_.to_string(static_cast<uint8_t>(Color::Green));
  value = mapper_.from_string(str);
  EXPECT_EQ(value, static_cast<uint8_t>(Color::Green));

  // String to enum to string
  value = mapper_.from_string("blue");
  str = mapper_.to_string(value);
  EXPECT_EQ(str, "blue");
}

}  // namespace fes
