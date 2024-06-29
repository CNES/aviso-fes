// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/python/optional.hpp
/// @brief Boost optional cast to/from Python
#pragma once
#include <pybind11/stl.h>

#include <boost/optional.hpp>

namespace pybind11 {
namespace detail {

/// @brief  Type caster for boost::optional
/// @tparam T Type of the optional value
template <typename T>
struct type_caster<boost::optional<T>>
    : public optional_caster<boost::optional<T>> {};

/// @brief  Type caster for boost::none_t
template <>
struct type_caster<boost::none_t> : void_caster<boost::none_t> {};

}  // namespace detail
}  // namespace pybind11
