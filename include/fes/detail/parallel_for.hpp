// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/parallel_for.hpp
/// @brief Parallel for utility function.
#pragma once
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>

namespace fes {
namespace detail {

/// @brief Determine an appropriate chunk size for parallel processing.
/// @param size The total size of the data to be processed.
/// @param num_threads The number of threads to be used for processing.
/// @return The determined chunk size.
constexpr auto determine_chunk_size(const size_t size,
                                    const size_t num_threads) -> size_t {
  // Aim for a few chunks per thread so that fast threads can pick up extra
  // work while slow threads finish their share. Four is a reasonable
  // compromise between load balancing and per-chunk setup overhead.
  constexpr size_t kChunksPerThread = 4;
  const auto num_chunks = num_threads * kChunksPerThread;
  const auto chunk_size = (size + num_chunks - 1) / num_chunks;
  return std::max(chunk_size, static_cast<size_t>(1));
}

/// @brief Automates the cutting of vectors to be processed in thread.
///
/// Each spawned thread atomically claims chunks of the iteration range from a
/// shared counter and calls ``callable(start, end)`` on each claimed chunk
/// until the range is exhausted. This poor-man's work stealing keeps fast
/// threads busy while slow threads (e.g. extrapolated points, NaN paths)
/// finish their share, with no third-party dependency.
///
/// @note PyFES callers commonly allocate per-thread state (Accelerator,
/// WaveTable, Inference, LongPeriodEquilibrium) on entry of ``callable``.
/// With the default chunking, that state is allocated roughly
/// ``kChunksPerThread`` times per thread instead of once. Pass
/// ``chunk_size = (size + num_threads - 1) / num_threads`` to fall back to a
/// strictly static partition (one call per thread) when this overhead
/// matters more than load balancing.
///
/// @tparam Lambda Lambda function with signature ``void(size_t, size_t)``.
/// @param[in] callable Lambda function called in each thread launched.
/// @param[in] size Total size of the iteration range.
/// @param[in] num_threads The number of threads to use. If 0 all CPUs are
/// used. If 1, no parallel computing is used (useful for debugging).
/// @param[in] chunk_size Number of elements processed by ``callable`` per
/// invocation. ``0`` selects ``determine_chunk_size(size, num_threads)``.
template <typename Lambda>
void parallel_for(const Lambda& callable, const size_t size,
                  size_t num_threads, size_t chunk_size = 0) {
  // If num_threads is 0, use all CPUs. ``hardware_concurrency`` may itself
  // return 0 on exotic systems; fall back to a single worker.
  if (num_threads == 0) {
    num_threads = std::thread::hardware_concurrency();
  }
  if (num_threads == 0) {
    num_threads = 1;
  }

  if (size == 0) {
    return;
  }

  // Single-threaded path: useful for debugging and for tiny inputs.
  if (num_threads == 1) {
    callable(0, size);
    return;
  }

  // Cap thread count to the iteration size.
  num_threads = std::min(num_threads, size);

  if (chunk_size == 0) {
    chunk_size = determine_chunk_size(size, num_threads);
  }

  std::atomic<size_t> shared_index{0};
  std::exception_ptr exception = nullptr;
  std::mutex exception_mutex;

  // The body executed by every thread (and by the calling thread). Captures
  // ``exception`` and ``exception_mutex`` so the first exception thrown by
  // any worker is preserved without a data race; ``shared_index`` is the
  // atomic chunk dispenser.
  auto worker = [&]() {
    try {
      while (true) {
        const auto start =
            shared_index.fetch_add(chunk_size, std::memory_order_relaxed);
        if (start >= size) {
          break;
        }
        const auto end = std::min(start + chunk_size, size);
        callable(start, end);
      }
    } catch (...) {
      std::lock_guard<std::mutex> lock(exception_mutex);
      if (!exception) {
        exception = std::current_exception();
      }
    }
  };

  // Spawn ``num_threads - 1`` helper threads; the calling thread joins the
  // pool too, saving one ``std::thread`` spawn and one join per call.
  std::vector<std::thread> threads;
  threads.reserve(num_threads - 1);
  for (size_t ix = 1; ix < num_threads; ++ix) {
    threads.emplace_back(worker);
  }
  worker();

  for (auto&& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  // Rethrow the first exception captured, if any.
  if (exception) {
    std::rethrow_exception(exception);
  }
}

}  // namespace detail
}  // namespace fes
