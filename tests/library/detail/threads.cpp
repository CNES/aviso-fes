// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/detail/thread.hpp"

TEST(Thread, ParallelFor) {
  auto data = std::vector<size_t>(100);
  auto callable = [&data](const size_t start, const size_t end) {
    for (auto i = start; i < end; ++i) {
      data[i] = i;
    }
  };
  fes::detail::parallel_for(callable, 100, 15);
  for (auto i = 0; i < 100; ++i) {
    EXPECT_EQ(data[i], i);
  }
}

TEST(Thread, ParallelForCatchException) {
  auto data = std::vector<size_t>(100);
  auto callable = [&data](const size_t start, const size_t end) {
    for (auto i = start; i < end; ++i) {
      data[i] = i;
    }
    throw std::runtime_error("An error occurred");
  };
  EXPECT_THROW(fes::detail::parallel_for(callable, 100, 15),
               std::runtime_error);
  EXPECT_THROW(fes::detail::parallel_for(callable, 100, 1), std::runtime_error);
}
