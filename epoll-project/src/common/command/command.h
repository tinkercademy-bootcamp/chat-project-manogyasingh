#ifndef COMMAND_H
#define COMMAND_H

#include <optional>
#include <string>
#include <string_view>

namespace xtc::command {

enum class CommandType {
  Help,
  MiniHelp,
  SetUsername,
  Send,
  SendChannel,
  JoinChannel,
  CreateChannel,
  LeaveChannel,
  TransferChannelOwnership,
  DeleteChannel,
  ListJoinedChannels,
  ListAllChannels,
  KickOut,
};
struct Command {
  CommandType cmd;
  std::string arg1;
  std::string arg2;
};

std::optional<Command> parse_line(std::string_view line);

}  // namespace xtc::command
#endif