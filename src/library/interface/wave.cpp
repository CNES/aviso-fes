#include "fes/interface/wave.hpp"

#include <map>
#include <string>

namespace fes {

// Greek-letter to LaTeX mapping
static const std::map<std::string, std::string> kGreekLetterToLatex = {
    {"Alpha", "{\\alpha}"}, {"Beta", "{\\beta}"}, {"Gamma", "{\\gamma}"},
    {"Delta", "{\\delta}"}, {"Psi", "{\\psi}"},   {"Phi", "{\\phi}"},
    {"Theta", "{\\theta}"}, {"Chi", "{\\chi}"},   {"Pi", "{\\pi}"},
    {"Mu", "{\\mu}"},       {"Nu", "{\\nu}"},     {"Lambda", "{\\lambda}"},
    {"Eps", "{\\epsilon}"}, {"Eta", "{\\eta}"},   {"Sigma", "{\\sigma}"},
    {"Ups", "{\\upsilon}"}, {"Rho", "{\\rho}"},   {"Tau", "{\\tau}"},
};

auto WaveInterface::latex_name() const -> std::string {
  auto name = std::string(this->name());
  for (const auto& item : kGreekLetterToLatex) {
    auto pos = name.find(item.first);
    if (pos != std::string::npos) {
      name.replace(pos, item.first.length(), item.second);
      name.insert(0, "$");
      name.append("$");
    }
  }
  return name;
}

}  // namespace fes
