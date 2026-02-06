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

  auto H = Eigen::MatrixXd(w_size << 1, h.rows());

  H.topRows(w_size) = f.array() * vu.array().cos();
  H.bottomRows(w_size) = f.array() * vu.array().sin();

  Eigen::LDLT<Eigen::MatrixXd> ldlt(H * H.transpose());
  Eigen::VectorXd solution = ldlt.solve(H * h);
  result.real() = solution.topRows(w_size);
  result.imag() = solution.bottomRows(w_size);

  return result;
}

}  // namespace fes
