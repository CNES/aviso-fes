// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#pragma once

// The purpose of this file is to group the documentation of the different C++
// namespaces. Therefore, it is not intended to be included, even if it will not
// be a problem, in your project.

/// @brief FES root namespace
namespace fes {

/// @brief Astronomical angles.
namespace angle {}

/// @brief Computes the fundamental frequencies in the earth-moon-sun motion.
namespace speed {}

/// @brief Details of implementation.
namespace detail {

/// @brief Angles.
namespace angle {

/// @brief Astronomical angles.
namespace astronomic {

/// @brief Frequency of the astronomical angles.
namespace frequency {}

/// Speed of the astronomical angles.
namespace speed {}

}  // namespace astronomic
}  // namespace angle

/// @brief Geometric algorithms.
namespace geometry {}

/// @brief Mathematical Operations.
namespace math {}

}  // namespace detail

/// @brief Defines concepts, primitives and algorithms for solving geometry
/// problems.
namespace geometry {}

/// @brief Primitives and algorithms to find the nearest point on a mesh.
namespace mesh {}

/// @brief Python bindings.
namespace python {}

/// @brief Types of tidal models used to interpolate the tidal components.
namespace tidal_model {}

/// @brief Manage and define tidal waves
namespace wave {}

}  // namespace fes
