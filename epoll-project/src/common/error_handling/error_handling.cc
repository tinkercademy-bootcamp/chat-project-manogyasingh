#include "error_handling.h"

namespace xtc {

bool check_error(bool test, std::string error_message, bool raise_exception) {
  if (test) {
    SPDLOG_ERROR("{}", error_message);
    if (raise_exception) {
      throw std::runtime_error(error_message);
    }
    return true;
  }
  return false;
}
} // namespace xtc