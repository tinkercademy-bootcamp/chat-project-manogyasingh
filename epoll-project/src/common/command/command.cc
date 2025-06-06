#include "command.h"

#include <cctype>
#include <string_view>


namespace {
using std::string_view;
string_view ltrim(string_view s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
    s.remove_prefix(1);
  return s;
}

string_view token(string_view& s) {
  s = ltrim(s);
  auto pos = s.find_first_of(" \t\r\n");
  auto head = s.substr(0, pos);
  s = (pos == string_view::npos) ? string_view{} : s.substr(pos);
  return head;
}
}  // namespace

namespace xtc::command {
std::optional<Command> parse_line(std::string_view line) {
  if (line.empty() || line.front() != '/') {
    return std::nullopt;
  }
}
}  // namespace xtc::command