// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/constituent.hpp"

#include <gtest/gtest.h>

#include "fes/darwin/table.hpp"

namespace fes {
namespace darwin {

inline auto check_parse(const WaveTable& table) -> void {
  for (auto&& item : table) {
    ASSERT_EQ(constituents::parse(item->name()), item->ident());
  }
}

TEST(Constituents, Parse) {
  check_parse(WaveTable());
  EXPECT_THROW(constituents::parse("__x__"), std::invalid_argument);
  EXPECT_EQ(constituents::parse("msqm"), kMSqm);
}

TEST(Constituents, ListParseAndGetName) {
  auto list = constituents::known();
  for (auto&& item : list) {
    auto ident = constituents::parse(item);
    EXPECT_EQ(constituents::name(ident), item);
  }
}

}  // namespace darwin
}  // namespace fes
