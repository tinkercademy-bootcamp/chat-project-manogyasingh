#ifndef UTILS_H
#define UTILS_H

#include "spdlog/spdlog.h"
#include <stdexcept>
#include <string.h>
#include <string_view>

namespace xtc {
bool check_error(bool test, std::string error_message,
                 bool raise_exception = true);
} // namespace xtc

#endif // UTILS_H