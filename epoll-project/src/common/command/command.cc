#include "command.h"

#include <cctype>
#include <string_view>

namespace {}

namespace xtc::command {
std::optional<Command> parse_line(std::string_view line) {
  if (line.empty() || line.front() != '/') {
    return std::nullopt;
  }
}
}  // namespace xtc::command