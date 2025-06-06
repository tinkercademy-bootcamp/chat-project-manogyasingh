#ifndef COMMAND_H
#define COMMAND_H

#include <optional>
#include <string>
#include <string_view>

namespace xtc::command {

enum class Cmd { Help, SetUsername, Send };
struct Command {
  Cmd cmd;
  std::string arg1;
  std::string arg2;
};

Command parse_line (std::string_view line);

}  // namespace xtc::command
#endif