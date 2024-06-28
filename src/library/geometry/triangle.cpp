// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/geometry/triangle.hpp"

#include "fes/detail/math.hpp"

namespace fes {
namespace geometry {

constexpr auto shift(const double value, const double center) -> double {
  auto result = detail::math::normalize_angle(value, center + 180.0);
  return detail::math::normalize_angle(result, center - 180.0);
}

inline auto shift(const Point& point, const Triangle& triangle) -> Triangle {
  const auto& v1 = triangle.v1();
  const auto& v2 = triangle.v2();
  const auto& v3 = triangle.v3();
  const auto lon1 = shift(v1.lon(), point.lon());
  const auto lon2 = shift(v2.lon(), lon1);
  const auto lon3 = shift(v3.lon(), lon2);

  return {Point(lon1, v1.lat()), Point(lon2, v2.lat()), Point(lon3, v3.lat())};
}

auto Triangle::reference_right_angled(const Point& point) const
    -> std::tuple<double, double> {
  auto const vertices = shift(point, *this).outer();
  auto iter = vertices.begin();

  // Read lon/lat of the first vertex of the triangle
  const auto t1 = iter->lon();
  const auto p1 = iter->lat();

  // Read lon/lat of the second vertex of the triangle
  const auto t2 = (++iter)->lon();
  const auto p2 = iter->lat();

  // Read lon/lat of the third vertex of the triangle
  const auto t3 = (++iter)->lon();
  const auto p3 = iter->lat();

  const auto ctx = t2 - t1;
  const auto cty = t3 - t1;
  const auto cpx = p2 - p1;
  const auto cpy = p3 - p1;

  auto jacobien = ctx * cpy - cty * cpx;

  if (detail::math::is_almost_zero(jacobien)) {
    return {0.0, 0.0};
  }
  jacobien = 1.0 / jacobien;

  const auto dx = point.lon() - t1;
  const auto dy = point.lat() - p1;

  const auto x = (cpy * dx - cty * dy) * jacobien;
  const auto y = (ctx * dy - cpx * dx) * jacobien;

  return {x, y};
}

}  // namespace geometry
}  // namespace fes
