// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin.hpp
/// @brief Darwin parameters for tidal constituents.
#pragma once

#include <cstdint>

namespace fes {

/// @brief  Darwin parameters for tidal constituents.
struct Darwin {
  int8_t t;       //!< Mean solar angle relative to Greenwich
  int8_t s;       //!< Moon's mean longitude
  int8_t h;       //!< Sun's mean longitude
  int8_t p;       //!< Longitude of the moon's perigee
  int8_t n;       //!< Longitude of moon's ascending node
  int8_t p1;      //!< Longitude of sun's perigee
  int8_t shift;   //!< Shift value
  int8_t eps;     //!< Coefficient for the longitude in moon's orbit of lunar
                  //!< intersection
  int8_t nu;      //!< Coefficient for the right ascension of lunar intersection
  int8_t nuprim;  //!< Coefficient for the term in argument of lunisolar
                  //!< constituent K1
  int8_t nusec;   //!< Coefficient for the term in argument of lunisolar
                  //!< constituent K2

  /// Default constructor
  constexpr Darwin(int8_t t, int8_t s, int8_t h, int8_t p, int8_t n, int8_t p1,
                   int8_t shift, int8_t eps, int8_t nu, int8_t nuprim,
                   int8_t nusec) noexcept
      : t(t),
        s(s),
        h(h),
        p(p),
        n(n),
        p1(p1),
        shift(shift),
        eps(eps),
        nu(nu),
        nuprim(nuprim),
        nusec(nusec) {}

  /// @brief Utility class to build Darwin parameters using named
  /// parameters.
  class Builder {
   public:
    /// Default constructor
    /// Initializes all parameters to zero.
    /// This is useful for creating a Darwin object with default values.
    /// The user can then set only the parameters they are interested in.
    /// @note This constructor is constexpr to allow compile-time construction.
    constexpr Builder() = default;

    /// @brief Sets the mean solar angle relative to Greenwich.
    /// @param v The value to set for the mean solar angle.
    /// @return  Reference to the Builder object for method chaining.
    constexpr auto T(int8_t v) noexcept -> Builder& {
      t_ = v;
      return *this;
    }

    /// @brief Sets the moon's mean longitude.
    /// @param v The value to set for the moon's mean longitude.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto s(int8_t v) noexcept -> Builder& {
      s_ = v;
      return *this;
    }

    /// @brief Sets the sun's mean longitude.
    /// @param v The value to set for the sun's mean longitude.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto h(int8_t v) noexcept -> Builder& {
      h_ = v;
      return *this;
    }

    /// @brief Sets the longitude of the moon's perigee.
    /// @param v The value to set for the longitude of the moon's perigee.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto p(int8_t v) noexcept -> Builder& {
      p_ = v;
      return *this;
    }

    /// @brief Sets the longitude of the moon's ascending node.
    /// @param v The value to set for the longitude of the moon's ascending
    /// node.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto n(int8_t v) noexcept -> Builder& {
      n_ = v;
      return *this;
    }

    /// @brief Sets the longitude of the sun's perigee.
    /// @param v The value to set for the longitude of the sun's perigee.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto p1(int8_t v) noexcept -> Builder& {
      p1_ = v;
      return *this;
    }

    /// @brief Sets the shift value.
    /// @param v The value to set for the shift.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto shift(int8_t v) noexcept -> Builder& {
      shift_ = v;
      return *this;
    }

    /// @brief Sets the coefficient for the longitude in moon's orbit of lunar
    /// intersection.
    /// @param v The value to set for the coefficient.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto xi(int8_t v) noexcept -> Builder& {
      eps_ = v;
      return *this;
    }

    /// @brief Sets the coefficient for the right ascension of lunar
    /// intersection.
    /// @param v The value to set for the coefficient.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto nu(int8_t v) noexcept -> Builder& {
      nu_ = v;
      return *this;
    }

    /// @brief Sets the coefficient for the term in argument of lunisolar
    /// constituent K1.
    /// @param v The value to set for the coefficient.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto nuprim(int8_t v) noexcept -> Builder& {
      nuprim_ = v;
      return *this;
    }

    /// @brief Sets the coefficient for the term in argument of lunisolar
    /// constituent K2.
    /// @param v The value to set for the coefficient.
    /// @return Reference to the Builder object for method chaining.
    constexpr auto nusec(int8_t v) noexcept -> Builder& {
      nusec_ = v;
      return *this;
    }

    /// @brief Builds the Darwin parameters.
    /// @return A Darwin object initialized with the parameters set in the
    /// Builder.
    /// @note This method is constexpr to allow compile-time construction.
    constexpr auto build() const noexcept -> Darwin {
      return Darwin(t_, s_, h_, p_, n_, p1_, shift_, eps_, nu_, nuprim_,
                    nusec_);
    }

   private:
    /// Member variables to hold the parameters.
    int8_t t_{0};
    int8_t s_{0};
    int8_t h_{0};
    int8_t p_{0};
    int8_t n_{0};
    int8_t p1_{0};
    int8_t shift_{0};
    int8_t eps_{0};
    int8_t nu_{0};
    int8_t nuprim_{0};
    int8_t nusec_{0};
  };
};

}  // namespace fes
