// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/thread.hpp
/// @brief Parallelization
#pragma once
#include <exception>
#include <thread>
#include <vector>

namespace fes {
namespace detail {

/// Automates the cutting of vectors to be processed in thread.
///
/// @tparam Lambda Lambda function
/// @param[in] callable Lambda function called in each thread launched
/// @param[in] size Size of all vectors to be processed
/// @param[in] num_threads The number of threads to use for the computation. If
/// 0 all CPUs are used. If 1 is given, no parallel computing code is used at
/// all, which is useful for debugging.
template <typename Lambda>
void parallel_for(const Lambda& callable, const size_t size,
                  size_t num_threads) {
  // If num_threads is 0, use all CPUs
  if (num_threads == 0) {
    num_threads = std::thread::hardware_concurrency();
  }

  // Adjust num_threads to not exceed the size
  num_threads = std::min(num_threads, static_cast<size_t>(size));

  // If num_threads is 1, no parallel computing code is used
  if (num_threads == 1) {
    callable(0, size);
    return;
  }

  // List of threads responsible for parallelizing the calculation
  std::vector<std::thread> threads;
  std::exception_ptr exception = nullptr;

  // Access index to the vectors required for calculation
  size_t start = 0;
  size_t shift = size / num_threads;

  threads.reserve(num_threads);

  // Launch threads
  for (size_t ix = 0; ix < num_threads; ++ix) {
    size_t end = (ix == num_threads - 1) ? size : start + shift;
    threads.emplace_back([&callable, start, end, &exception]() {
      try {
        callable(start, end);
      } catch (...) {
        exception = std::current_exception();
      }
    });
    start += shift;
  }

  // Join threads
  for (auto&& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  // Rethrow the last exception caught
  if (exception) {
    std::rethrow_exception(exception);
  }
}

}  // namespace detail
}  // namespace fes
