// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <functional>

#include "fes/angle/astronomic.hpp"
#include "fes/axis.hpp"
#include "fes/darwin/constituent.hpp"
#include "fes/darwin/tide.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/settings.hpp"
#include "fes/tidal_model/cartesian.hpp"

namespace fes {

class EvaluateTideTest : public ::testing::Test {
 protected:
  // Helper function to add a constituent to the tidal model
  template <typename T>
  static auto add_constituent(tidal_model::Cartesian<double>& model,
                              const T constituent, const Complex& value) {
    Eigen::Matrix<Complex, 4, 1> values =
        Eigen::Matrix<Complex, 4, 1>::Constant(value);
    model.add_constituent(static_cast<uint8_t>(constituent), values);
  }

  void SetUp() override {
    // Create a test tide table with known values
    auto x_axis = Axis(-180, 179, 360);  // Longitude from -180 to 179
    auto y_axis = Axis(-90, 90, 181);    // Latitude from -90 to 90

    darwin_ = std::make_unique<tidal_model::Cartesian<double>>(
        x_axis, y_axis, darwin::constituents::map(), kTide);
    perth_ = std::make_unique<tidal_model::Cartesian<double>>(
        x_axis, y_axis, perth::constituents::map(), kTide);
    add_constituent(*darwin_, darwin::Constituent::kQ1, kQ1);
    add_constituent(*darwin_, darwin::Constituent::kO1, kO1);
    add_constituent(*darwin_, darwin::Constituent::kP1, kP1);
    add_constituent(*darwin_, darwin::Constituent::kS1, kS1);
    add_constituent(*darwin_, darwin::Constituent::kK1, kK1);
    add_constituent(*darwin_, darwin::Constituent::kN2, kN2);
    add_constituent(*darwin_, darwin::Constituent::kM2, kM2);
    add_constituent(*darwin_, darwin::Constituent::kS2, kS2);
    add_constituent(*darwin_, darwin::Constituent::kK2, kK2);
    add_constituent(*darwin_, darwin::Constituent::kM4, kM4);
    add_constituent(*darwin_, darwin::Constituent::kMS4, kMS4);
    add_constituent(*darwin_, darwin::Constituent::k2N2, k2N2);
    add_constituent(*darwin_, darwin::Constituent::kMu2, kMu2);
    add_constituent(*darwin_, darwin::Constituent::kJ1, kJ1);
    add_constituent(*darwin_, darwin::Constituent::kSigma1, kSigma1);
    add_constituent(*darwin_, darwin::Constituent::kOO1, kOO1);

    add_constituent(*perth_, perth::Constituent::kQ1, kQ1);
    add_constituent(*perth_, perth::Constituent::kO1, kO1);
    add_constituent(*perth_, perth::Constituent::kP1, kP1);
    add_constituent(*perth_, perth::Constituent::kS1, kS1);
    add_constituent(*perth_, perth::Constituent::kK1, kK1);
    add_constituent(*perth_, perth::Constituent::kN2, kN2);
    add_constituent(*perth_, perth::Constituent::kM2, kM2);
    add_constituent(*perth_, perth::Constituent::kS2, kS2);
    add_constituent(*perth_, perth::Constituent::kK2, kK2);
    add_constituent(*perth_, perth::Constituent::kM4, kM4);
    add_constituent(*perth_, perth::Constituent::kMS4, kMS4);
    add_constituent(*perth_, perth::Constituent::k2N2, k2N2);
    add_constituent(*perth_, perth::Constituent::kMu2, kMu2);
    add_constituent(*perth_, perth::Constituent::kJ1, kJ1);
    add_constituent(*perth_, perth::Constituent::kSigma1, kSigma1);
    add_constituent(*perth_, perth::Constituent::kOO1, kOO1);
  }
  std::unique_ptr<tidal_model::Cartesian<double>> darwin_{nullptr};
  std::unique_ptr<tidal_model::Cartesian<double>> perth_{nullptr};
  static constexpr double lon_{-7.6880002021789551};
  static constexpr double lat_{59.194999694824219};
  // 1983-01-01T00:00:00Z
  static constexpr double epoch_{410227200.0};

  static constexpr Complex kQ1{0.020445803855, -0.023776893126};
  static constexpr Complex kO1{0.077220699676, 0.012257148169};
  static constexpr Complex kP1{-0.020080937187, 0.016418071499};
  static constexpr Complex kS1{0.004785750983, 0.004907670934};
  static constexpr Complex kK1{-0.079487218251, 0.052526752141};
  static constexpr Complex kN2{-0.186384804405, 0.030053704357};
  static constexpr Complex kM2{-0.905210879869, -0.206028739816};
  static constexpr Complex kS2{-0.245810906157, -0.256641196181};
  static constexpr Complex kK2{-0.073832471438, -0.070972165775};
  static constexpr Complex kM4{0.002554349793, 0.001419489975};
  static constexpr Complex kMS4{-0.000595591398, -0.001404640701};
  static constexpr Complex k2N2{-0.022375092702, 0.013246037651};
  static constexpr Complex kMu2{-0.026544319710, 0.024079048285};
  static constexpr Complex kJ1{-0.003306714539, 0.001542657468};
  static constexpr Complex kSigma1{0.000293298389, -0.007000013174};
  static constexpr Complex kOO1{-0.002184466393, -0.001772761540};
};

class EvaluateTideFromConstituentsTest : public ::testing::Test {
 protected:
  static auto add_constituent(
      std::map<darwin::Constituent, Complex>& map,
      const std::function<darwin::Constituent(const std::string&)>& func,
      const std::string& name, const std::pair<double, double>& value) {
    map[func(name)] =
        std::polar(value.first, detail::math::radians(value.second));
  }

  static auto add_constituent(
      std::map<perth::Constituent, Complex>& map,
      const std::function<perth::Constituent(const std::string&)>& func,
      const std::string& name, const std::pair<double, double>& value) {
    map[func(name)] =
        std::polar(value.first, detail::math::radians(value.second));
  }

  auto SetUp() -> void override {
    std::map<std::string, std::pair<double, double>> constituents{
        {"M2", {205.113, 109.006}},   {"K1", {6.434, 75.067}},
        {"N2", {41.695, 90.633}},     {"O1", {6.587, 327.857}},
        {"P1", {2.252, 63.658}},      {"Q1", {2.040, 281.362}},
        {"K2", {21.361, 145.892}},    {"S2", {74.876, 148.283}},
        {"S1", {0.797, 11.441}},      {"SA", {4.905, 322.761}},
        {"T2", {4.171, 138.535}},     {"MF", {1.031, 175.663}},
        {"MM", {0.425, 199.741}},     {"2N2", {5.699, 72.786}},
        {"M4", {5.437, 105.940}},     {"J1", {0.241, 123.005}},
        {"SSA", {2.047, 98.898}},     {"MSF", {0.356, 24.980}},
        {"MSQM", {0.115, 254.934}},   {"EPS2", {1.968, 89.471}},
        {"L2", {6.392, 102.910}},     {"M3", {1.977, 15.860}},
        {"R2", {0.534, 158.066}},     {"MU2", {8.566, 105.087}},
        {"MTM", {0.110, 142.031}},    {"NU2", {7.780, 86.614}},
        {"LAMBDA2", {2.625, 75.845}}, {"MN4", {1.937, 60.491}},
        {"MS4", {3.258, 181.835}},    {"MKS2", {0.758, 173.969}},
        {"N4", {0.291, 9.263}},       {"M6", {3.153, 354.764}},
        {"M8", {0.231, 231.883}},     {"S4", {0.217, 289.151}},
        {"2Q1", {0.376, 234.893}},    {"OO1", {0.136, 213.353}},
        {"M1", {0.535, 83.038}},
    };

    for (const auto& item : constituents) {
      add_constituent(darwin_, darwin::constituents::parse, item.first,
                      item.second);
      add_constituent(perth_, perth::constituents::parse, item.first,
                      item.second);
    }
  }

  std::map<darwin::Constituent, Complex> darwin_;
  std::map<perth::Constituent, Complex> perth_;
};

TEST_F(EvaluateTideTest, Perth5) {
  auto lons = Eigen::Vector<double, 1>::Constant(lon_);
  auto lats = Eigen::Vector<double, 1>::Constant(lat_);
  auto epochs = Eigen::Vector<double, 1>::Constant(epoch_);

  auto result = evaluate_tide(perth_.get(), epochs, lons, lats,
                              PerthRuntimeSettings{}.with_num_threads((1)));
  EXPECT_NEAR(std::get<0>(result)(0), -0.92959402, 1e-6);
  EXPECT_NEAR(std::get<1>(result)(0), 0.00475658, 1e-6);
  EXPECT_EQ(std::get<2>(result)(0), 4);
}

TEST_F(EvaluateTideTest, FES) {
  auto lons = Eigen::Vector<double, 1>::Constant(lon_);
  auto lats = Eigen::Vector<double, 1>::Constant(lat_);
  auto epochs = Eigen::Vector<double, 1>::Constant(epoch_);

  auto result = evaluate_tide(
      perth_.get(), epochs, lons, lats,
      FesRuntimeSettings{}.with_num_threads((1)).with_astronomic_formulae(
          angle::Formulae::kIERS));
  EXPECT_NEAR(std::get<0>(result)(0), 1.174250344816586, 1e-6);
  EXPECT_NEAR(std::get<1>(result)(0), 0.91756905183442172, 1e-6);
  EXPECT_EQ(std::get<2>(result)(0), 4);
}

TEST_F(EvaluateTideFromConstituentsTest, Darwin) {
  // BREST location = (lon:-4.495, lat:48.383)

  auto epochs = Eigen::Vector<double, 1>::Constant(410227200.0);

  auto result = darwin::evaluate_tide_from_constituents(
      darwin_, epochs, 48.383, FesRuntimeSettings{}.with_num_threads((1)));
  EXPECT_NEAR(std::get<0>(result)(0), -272.41405405513166, 1e-6);
  EXPECT_NEAR(std::get<1>(result)(0), 3.8406047433116997, 1e-6);
}

TEST_F(EvaluateTideFromConstituentsTest, Perth5) {
  // BREST location = (lon:-4.495, lat:48.383)

  auto epochs = Eigen::Vector<double, 1>::Constant(410227200.0);

  auto result = fes::perth::evaluate_tide_from_constituents(
      perth_, epochs, 48.383, PerthRuntimeSettings{}.with_num_threads((1)));
  EXPECT_NEAR(std::get<0>(result)(0), -271.65613183365048, 1e-6);
  EXPECT_NEAR(std::get<1>(result)(0), 3.9218121242411668, 1e-6);
}

}  // namespace fes
