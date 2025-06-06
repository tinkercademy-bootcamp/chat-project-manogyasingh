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
  line = ltrim(line);
  if (line.empty() || line.front() != '/') {
    return std::nullopt;
  }

  const auto thiscommand = token(line);

  if (thiscommand == "/help") {
    return Command{CommandType::Help, {}, {}};
  }

  if (thiscommand == "/set_username") {
    const auto name = token(line);
    if (name.empty()) return std::nullopt;
    return Command{CommandType::SetUsername, std::string{name}, {}};
  }

  if (thiscommand == "/send") {
    const auto target = token(line);

    if (target.size() < 2 || target.front() != '@') {
      return std::nullopt;
    }
    line = ltrim(line);
    if (line.empty()) {
      return std::nullopt;
    }
    return Command{CommandType::Send, std::string{target.substr(1)}, std::string{line}};
  }

  return std::nullopt;
}
}  // namespace xtc::command