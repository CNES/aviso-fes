#include "fes/perth/doodson.hpp"

#include <gtest/gtest.h>

#include "fes/angle/astronomic.hpp"
#include "fes/types.hpp"

namespace fes {
namespace perth {

TEST(DoodsonTest, CalculateCelestialVector) {
  auto astronomic_angles = angle::Astronomic(angle::Formulae::kIERS);
  // Test case with 1983-01-01T00:00:00 UTC
  astronomic_angles.update(410227200.0);
  Vector6d result = calculate_celestial_vector(astronomic_angles);

  // Expected values based on the provided test case
  Vector6d expected;
  expected << 160.59900704910373, 119.4907876655474, 280.08979471465113 - 360.0,
      111.5928443590156, 266.13901453365702 - 360.0, 282.64503464175243 - 360.0;

  // Use EXPECT_NEAR with appropriate tolerance for floating point comparison
  const double tolerance = 1e-4;

  EXPECT_NEAR(result[0], expected[0], tolerance);
  EXPECT_NEAR(result[1], expected[1], tolerance);
  EXPECT_NEAR(result[2], expected[2], tolerance);
  EXPECT_NEAR(result[3], expected[3], tolerance);
  EXPECT_NEAR(result[4], expected[4], tolerance);
  EXPECT_NEAR(result[5], expected[5], tolerance);
}

TEST(DoodsonTest, CalculateDoodsonArgument) {
  auto astronomic_angles = angle::Astronomic(angle::Formulae::kIERS);
  // Test case with 1983-01-01T00:00:00 UTC
  double time = 410227200.0;
  astronomic_angles.update(time);

  // kNode constituent data
  const Vector7b doodson_number = {0, 0, 0, 0, 1, 0, 2};

  double result = calculate_doodson_argument(astronomic_angles,
                                             doodson_number.cast<double>());
  EXPECT_NEAR(result, 86.139014533657019, 1e-5);
}

}  // namespace perth
}  // namespace fes
