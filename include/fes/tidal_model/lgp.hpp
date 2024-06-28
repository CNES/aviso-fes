// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tidal_model/lgp.hpp
/// @brief LGP tidal models
#pragma once
#include <Eigen/Core>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "fes/abstract_tidal_model.hpp"
#include "fes/detail/isviewstream.hpp"
#include "fes/detail/serialize.hpp"
#include "fes/eigen.hpp"
#include "fes/mesh/index.hpp"
#include "fes/string_view.hpp"
#include "fes/wave.hpp"

namespace fes {
namespace tidal_model {

/// @brief A class representing an accelerator for LGP tidal models.
///
/// This class is used to accelerate the interpolation of LGP tidal models by
/// caching the selected triangle for a given point.
class LGPAccelerator : public Accelerator {
 public:
  /// Default constructor.
  /// @param[in] formulae The formulae used to calculate the astronomic angle.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @param[in] n_constituents The number of tidal constituents handled by the
  /// tidal model.
  LGPAccelerator(const angle::Formulae& formulae, const double time_tolerance,
                 const size_t n_constituents)
      : Accelerator(formulae, time_tolerance, n_constituents) {}

  /// Default destructor.
  virtual ~LGPAccelerator() = default;

  /// Set the selected triangle for the accelerator.
  ///
  /// @param[in] triangle The selected triangle.
  auto set(fes::mesh::SelectedTriangle&& triangle) -> void {
    selected_ = triangle;
  }

  /// Reset the point of the selected triangle.
  auto reset(const geometry::Point& point) -> void { selected_.point = point; }

  /// Get the selected triangle for the accelerator.
  ///
  /// @return The selected triangle.
  auto get() const noexcept -> const fes::mesh::SelectedTriangle& {
    return selected_;
  }

  /// Check if a point is in the cache.
  ///
  /// @param[in] point The point to check.
  /// @return True if the point is in the cache, false otherwise.
  inline auto in_cache(const geometry::Point& point) const -> bool {
    // If the selected triangle is valid and the point is inside the triangle,
    // the point is considered to be in the cache. A point that is projected
    // onto the edge of the nearest triangle will not be considered to be in the
    // cache.
    return selected_.index != -1 && selected_.inside &&
           selected_.triangle.covered_by(point);
  }

 private:
  /// The selected triangle for the accelerator.
  mesh::SelectedTriangle selected_{};
};

/// @brief %LGP tidal models.
///
/// Interpolate the modeled tidal constituents from the finite elements using
/// an %LGP discretization.
///
/// @tparam T The type of the wave model loaded.
/// @tparam N The degree of the %LGP discretization.
template <typename T, int N>
class LGP : public fes::AbstractTidalModel<T> {
 public:
  /// %LGP codes.
  using codes_t = Eigen::Matrix<int, Eigen::Dynamic, N * 3>;

  /// Build a new %LGP tidal model.
  ///
  /// @param[in] index The mesh index.
  /// @param[in] codes %LGP codes.
  /// @param[in] tide_type The tide type handled by the model.
  /// @param[in] max_distance The maximum distance allowed to extrapolate the
  /// wave model. By default, extrapolation is disabled, all points outside the
  /// mesh will be considered undefined.
  LGP(std::shared_ptr<mesh::Index> index, codes_t codes, TideType tide_type,
      double max_distance = 0);

  /// Default destructor
  virtual ~LGP() override = default;

  /// Add a tidal constituent to the model.
  ///
  /// @param[in] ident The wave model identifier.
  /// @param[in] wave The wave model.
  inline auto add_constituent(const Constituent ident,
                              Vector<std::complex<T>> wave) -> void override {
    // wave is a vector of values for each LGP codes. The number of values must
    // match the number of LGP codes handled by this instance.
    if (expected_data_size_ != wave.size()) {
      throw std::invalid_argument("wave size does not match expected size: " +
                                  std::to_string(wave.size()) +
                                  " != " + std::to_string(expected_data_size_));
    }
    this->data_.emplace(ident, std::move(wave));
  }

  /// @brief Create a new instance of the LGPAccelerator class to speed up the
  /// calculation.
  ///
  /// To avoid searching for the query point if it is already contained in the
  /// last selected triangle, this method creates a new instance of the
  /// LGPAccelerator class, which stores the last selected triangle for the
  /// mesh.
  ///
  /// @param[in] formulae The formulae used to calculate the astronomic angle.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @return A pointer  to the newly created LGPAccelerator instance.
  auto accelerator(const angle::Formulae& formulae,
                   const double time_tolerance) const -> Accelerator* override {
    return new LGPAccelerator(formulae, time_tolerance, this->data_.size());
  }

  /// Interpolate the wave models loaded at the given point.
  ///
  /// @param[in] point The point to interpolate at.
  /// @param[inout] quality A flag indicating if the point was extrapolated.
  /// @param[inout] acc An accelerator to speed up the calculation.
  /// @return A list of interpolated wave models.
  auto interpolate(const geometry::Point& point, Quality& quality,
                   Accelerator* acc) const -> const ConstituentValues& override;

  /// Get the mesh index.
  ///
  /// @return The mesh index.
  inline auto index() const -> std::shared_ptr<mesh::Index> const& {
    return index_;
  }

  /// Get a string representation of the state of the tidal model.
  ///
  /// @return A string representation of the state of the tidal model.
  auto getstate() const -> std::string;

 protected:
  /// @brief Default constructor
  LGP() = default;

  /// @brief Calculate the coefficients of the Lagrange polynomials
  /// @param[in] x The x coordinate of the point to interpolate at.
  /// @param[in] y The y coordinate of the point to interpolate at.
  /// @return The coefficients of the Lagrange polynomials.
  virtual auto calculate_beta(const double x, const double y) const
      -> Eigen::Matrix<double, N * 3, 1> = 0;

  /// @brief Set the state of the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @note As this class is abstract, this method must be overloaded by the
  /// derived classes to define the state of the tidal model.
  auto setstate_instance(const string_view& data);

 private:
  /// Expected data size for each data set
  int expected_data_size_{};

  /// Index used to find the nearest triangle
  std::shared_ptr<mesh::Index> index_{};

  /// The maximum distance allowed to extrapolate the wave model.
  double max_distance_{};

  /// %LGP codes for each triangles in the index
  codes_t codes_{};
};

/// @brief %LGP1 tidal model.
///
/// Interpolate the modeled tidal constituents from the finite elements using
/// the %LGP1 discretization.
///
/// @tparam T The type of the wave model loaded.
template <typename T>
class LGP1 : public LGP<T, 1> {
 public:
  /// Default constructor
  using LGP<T, 1>::LGP;

  /// @brief Deserialize the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @return The tidal model.
  static auto setstate(const string_view& data) -> LGP1<T> {
    auto model = LGP1<T>();
    model.setstate_instance(data);
    return model;
  }

 private:
  /// @brief Compute the beta coefficients for the %LGP1 discretization.
  ///
  /// @param[in] x The x coordinate of the point.
  /// @param[in] y The y coordinate of the point.
  /// @return The beta coefficients.
  inline auto calculate_beta(const double x, const double y) const
      -> Eigen::Matrix<double, 3, 1> override {
    return (Eigen::Matrix<double, 3, 1>() << 1 - x - y, x, y).finished();
  }

  /// @brief Set the state of the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @note As the `setstate_instance` method is protected, this method must be
  /// overloaded by the derived classes to define the state of the tidal model.
  auto setstate_instance(const string_view& data) -> void {
    try {
      LGP<T, 1>::setstate_instance(data);
    } catch (const std::exception& e) {
      throw std::runtime_error("invalid LGP1 tidal model state");
    }
  }
};

/// @brief %LGP2 tidal model.
///
/// Interpolate the modeled tidal constituents from the finite elements using
/// the %LGP2 discretization.
///
/// @tparam T The type of the wave model loaded.
template <typename T>
class LGP2 : public LGP<T, 2> {
 public:
  /// Default constructor
  using LGP<T, 2>::LGP;

  /// @brief Deserialize the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @return The tidal model.
  static auto setstate(const string_view& data) -> LGP2<T> {
    auto model = LGP2<T>();
    model.setstate_instance(data);
    return model;
  }

 private:
  /// @brief Compute the beta coefficients for the %LGP2 discretization.
  ///
  /// @param[in] x The x coordinate of the point.
  /// @param[in] y The y coordinate of the point.
  /// @return The beta coefficients.
  inline auto calculate_beta(const double x, const double y) const
      -> Eigen::Matrix<double, 6, 1> override {
    return (Eigen::Matrix<double, 6, 1>()
                << 2 * (x + y - 0.5) * (x + y - 1),  //  2x²+2y²+4xy-3x-3y+1
            -4 * x * (x + y - 1),                    // -4x²    -4xy+4x
            2 * x * (x - 0.5),                       //  2x²        - x
            4 * x * y,                               //         +4xy
            2 * y * (y - 0.5),                       //     +2y²       - y
            -4 * y * (x + y - 1))                    //     -4y²-4xy   +4y
        .finished();
  }

  /// @brief Set the state of the tidal model.
  ///
  /// @param[in] data The serialized tidal model.
  /// @note As the `setstate_instance` method is protected, this method must be
  /// overloaded by the derived classes to define the state of the tidal model.
  auto setstate_instance(const string_view& data) -> void {
    try {
      LGP<T, 2>::setstate_instance(data);
    } catch (const std::exception& e) {
      throw std::runtime_error("invalid LGP2 tidal model state");
    }
  }
};

// /////////////////////////////////////////////////////////////////////////////
template <typename T, int N>
LGP<T, N>::LGP(std::shared_ptr<mesh::Index> index, LGP::codes_t codes,
               TideType tide_type, const double max_distance)
    : AbstractTidalModel<T>(tide_type),
      index_(std::move(index)),
      max_distance_(max_distance),
      codes_(std::move(codes)) {
  // The number of triangles in the index must match the number of LGP codes
  // provided.
  if (index_->n_triangles() != static_cast<size_t>(codes_.rows())) {
    throw std::invalid_argument(
        "index and codes must have the same number of triangles: " +
        std::to_string(index_->n_triangles()) +
        " != " + std::to_string(codes_.rows()));
  }

  // Determine the first and last LGP codes for each triangle
  auto min_index = std::numeric_limits<int>::max();
  auto max_index = std::numeric_limits<int>::min();
  std::for_each(codes_.data(), codes_.data() + codes_.size(),
                [&min_index, &max_index](const auto& code) {
                  min_index = std::min(min_index, code);
                  max_index = std::max(max_index, code);
                });
  if (min_index < 0) {
    throw std::invalid_argument("codes_t must be positive");
  }

  // Store the expected data size to interpolate
  expected_data_size_ = max_index + 1;
}

// /////////////////////////////////////////////////////////////////////////////
template <typename T, int N>
auto LGP<T, N>::interpolate(const geometry::Point& point, Quality& quality,
                            Accelerator* acc) const
    -> const ConstituentValues& {
  auto* lgp_acc = reinterpret_cast<LGPAccelerator*>(acc);

  // Reset the accelerator if the point is not in the cache, otherwise update
  // the point in use.
  lgp_acc->in_cache(point) ? lgp_acc->reset(point)
                           : lgp_acc->set(index_->search(point, max_distance_));

  // Remove all the data from the previous interpolation
  lgp_acc->clear();

  // Get the cached triangle
  const auto& selected_triangle = lgp_acc->get();
  if (selected_triangle.index == -1) {
    // No triangle found, return NaN
    for (const auto& item : this->data_) {
      lgp_acc->emplace_back(
          item.first,
          std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN()));
    }
    quality = Quality::kUndefined;
    return lgp_acc->values();
  }

  // Get the LGP codes for the triangle
  const auto& codes = codes_.row(selected_triangle.index);

  // In the case of extrapolation, if the projected point is one of the vertices
  // of the triangle, then we return values associated with this vertex. The
  // LGP code of vertex 0 is at index 0, the code of vertex 1 is at index 2 and
  // the code of vertex 3 is at index 4.
  auto vertex_id =
      selected_triangle.triangle.is_vertex(selected_triangle.point);
  if (vertex_id != -1) {
    for (const auto& item : this->data_) {
      const auto value = item.second(codes(vertex_id << 1));
      lgp_acc->emplace_back(item.first, static_cast<std::complex<T>>(value));
    }
    quality = Quality::kExtrapolated1;
    return lgp_acc->values();
  }

  // Calculate ξ and η for the given point
  const auto xy = selected_triangle.triangle.reference_right_angled(
      selected_triangle.point);

  // Calculate the beta coefficients for the given point
  const auto beta = calculate_beta(std::get<0>(xy), std::get<1>(xy));

  // Interpolate the wave model for each data set
  for (const auto& item : this->data_) {
    const auto& wave = item.second;
    auto dot = std::complex<double>(0, 0);

    // Read the values for each LGP code
    for (auto ix = 0; ix < N * 3; ++ix) {
      dot += beta(ix) * static_cast<std::complex<double>>(wave(codes(ix)));
    }
    lgp_acc->emplace_back(item.first, dot);
  }
  quality = Quality::kInterpolated;
  return lgp_acc->values();
}

template <typename T, int N>
auto LGP<T, N>::getstate() const -> std::string {
  auto ss = std::stringstream();
  ss.exceptions(std::stringstream::failbit);
  detail::serialize::write_data(ss, this->tide_type_);
  detail::serialize::write_string(ss, index_->getstate());
  detail::serialize::write_data(ss, max_distance_);
  detail::serialize::write_matrix<int, Eigen::Dynamic, N * 3>(ss, codes_);
  detail::serialize::write_constituent_map(ss, this->data_);
  return ss.str();
}

template <typename T, int N>
auto LGP<T, N>::setstate_instance(const string_view& data) {
  detail::isviewstream ss(data);
  ss.exceptions(std::stringstream::failbit);
  this->tide_type_ = detail::serialize::read_data<TideType>(ss);
  this->index_ = std::make_shared<mesh::Index>(
      mesh::Index::setstate(detail::serialize::read_string(ss)));
  this->max_distance_ = detail::serialize::read_data<double>(ss);
  this->codes_ = detail::serialize::read_matrix<int, Eigen::Dynamic, N * 3>(ss);
  this->data_ =
      detail::serialize::read_constituent_map<Constituent, std::complex<T>>(ss);
}

}  // namespace tidal_model
}  // namespace fes
