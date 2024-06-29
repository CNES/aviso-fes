// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/constituent.hpp"

#include <gtest/gtest.h>

#include "fes/wave/table.hpp"

inline auto check_parse(const fes::wave::Table& table) -> void {
  for (auto&& item : table) {
    ASSERT_EQ(fes::constituents::parse(item->name()), item->ident());
  }
}

TEST(Constituents, Parse) {
  check_parse(fes::wave::Table());
  EXPECT_THROW(fes::constituents::parse("__x__"), std::invalid_argument);
  EXPECT_EQ(fes::constituents::parse("msqm"), fes::kMsqm);
}

TEST(Constituents, ListParseAndGetName) {
  auto list = fes::constituents::known();
  for (auto&& item : list) {
    auto ident = fes::constituents::parse(item);
    EXPECT_EQ(fes::constituents::name(ident), item);
  }
}
