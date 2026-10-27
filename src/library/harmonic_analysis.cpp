#include "fes/harmonic_analysis.hpp"

#include <Eigen/Dense>
#include <limits>

#include "fes/detail/broadcast.hpp"

namespace fes {

auto harmonic_analysis(const Eigen::Ref<const Eigen::VectorXd>& h,
                       const DynamicRef<const Eigen::MatrixXd>& f,
                       const DynamicRef<const Eigen::MatrixXd>& vu)
    -> Eigen::VectorXcd {
  detail::check_eigen_shape("f", f, "vu", vu);
  if (h.rows() != f.cols() || h.rows() != vu.cols()) {
    throw std::invalid_argument(
        "f, vu could not be broadcast with h with shape " +
        detail::eigen_shape(f) + ", " + detail::eigen_shape(h));
  }
  auto w_size = f.rows();
  auto result = Eigen::VectorXcd(w_size);

  if (h.hasNaN()) {
    result.fill(std::complex<double>(std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN()));
    return result;
  }

  // The mean sea level S0 is estimated with the constituents (last row of
  // ones): otherwise a non-zero mean of h biases their amplitudes. It is not
  // returned.
  auto H = Eigen::MatrixXd((w_size << 1) + 1, h.rows());

  H.topRows(w_size) = f.array() * vu.array().cos();
  H.middleRows(w_size, w_size) = f.array() * vu.array().sin();
  H.bottomRows(1).setOnes();

  Eigen::LDLT<Eigen::MatrixXd> ldlt(H * H.transpose());
  Eigen::VectorXd solution = ldlt.solve(H * h);
  result.real() = solution.head(w_size);
  result.imag() = solution.segment(w_size, w_size);

  return result;
}

}  // namespace fes
