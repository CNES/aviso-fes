#include "fes/inference.hpp"

#include <memory>
#include <stdexcept>

namespace fes {

auto inference_factory(WaveTableInterface& wave_table,
                       const InferenceType inference_type)
    -> std::unique_ptr<InferenceInterface> {
  switch (inference_type) {
    case InferenceType::kSpline:
      return std::make_unique<SplineInference>();
    case InferenceType::kZero:
      return std::make_unique<PerthInference>(
          wave_table, InterpolationType::kZeroAdmittance);
    case InferenceType::kLinear:
      return std::make_unique<PerthInference>(
          wave_table, InterpolationType::kLinearAdmittance);
    case InferenceType::kFourier:
      return std::make_unique<PerthInference>(
          wave_table, InterpolationType::kFourierAdmittance);
    default:
      throw std::invalid_argument("Unknown inference type");
  }
}

}  // namespace fes
