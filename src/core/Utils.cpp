#include <GameOfLife/core/Utils.hpp>
#include <iomanip>
#include <ios>
#include <string>

namespace J0o0ll::GOL::Utils {
std::string IntToString(int number) { return std::to_string(number); }

std::string FloatToString(float number, int precision) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(precision) << number;
  return oss.str();
}
} // namespace J0o0ll::GOL::Utils