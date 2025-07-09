#include "command.h"

#include <spdlog/spdlog.h>

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
    if (name.size() < 2 || name.front() != '@') return std::nullopt;
    return Command{CommandType::SetUsername, std::string{name.substr(1)}, {}};
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
    return Command{CommandType::Send, std::string{target.substr(1)},
                   std::string{line}};
  }
  if (thiscommand == "/send_channel") {
    const auto channel = token(line);

    if (channel.size() < 2 || channel.front() != '#') {
      return std::nullopt;
    }
    line = ltrim(line);
    if (line.empty()) {
      return std::nullopt;
    }
    return Command{CommandType::SendChannel, std::string{channel.substr(1)},
                   std::string{line}};
  }

  if (thiscommand == "/join_channel") {
    const auto channel = token(line);

    if (channel.size() < 2 || channel.front() != '#') {
      return std::nullopt;
    }
    return Command{
        CommandType::JoinChannel, std::string{channel.substr(1)}, {}};
  }
  if (thiscommand == "/leave_channel") {
    const auto channel = token(line);

    if (channel.size() < 2 || channel.front() != '#') {
      return std::nullopt;
    }
    return Command{
        CommandType::LeaveChannel, std::string{channel.substr(1)}, {}};
  }
  if (thiscommand == "/transfer_channel_ownership") {
    const auto channel = token(line);
    const auto target = token(line);

    if (channel.size() < 2 || channel.front() != '#' || target.size() < 2 ||
        target.front() != '@') {
      return std::nullopt;
    }
    return Command{CommandType::TransferChannelOwnership,
                   std::string{channel.substr(1)},
                   std::string{target.substr(1)}};
  }
  if (thiscommand == "/delete_channel") {
    const auto channel = token(line);

    if (channel.size() < 2 || channel.front() != '#') {
      return std::nullopt;
    }
    return Command{
        CommandType::DeleteChannel, std::string{channel.substr(1)}, {}};
  }
  if (thiscommand == "/list_joined_channels") {
    return Command{CommandType::ListJoinedChannels, {}, {}};
  }
  if (thiscommand == "/list_all_channels") {
    return Command{CommandType::ListAllChannels, {}, {}};
  }
  if (thiscommand == "/create_channel") {
    line = ltrim(line);
    const auto channel = token(line);
    spdlog::info("Creating channel: {}", channel);
    if (channel.size() < 2 || channel.front() != '#') {
      return std::nullopt;
    }
    return Command{
        CommandType::CreateChannel, std::string{channel.substr(1)}, {}};
  }
  if (thiscommand == "/kickout") {
    const auto target = token(line);
    const auto channel = token(line);

    if (target.size() < 2 || target.front() != '@' || channel.size() < 2 ||
        channel.front() != '#') {
      return std::nullopt;
    }
    return Command{CommandType::KickOut, std::string{channel.substr(1)},
                   std::string{target.substr(1)}};
  }
  return Command{CommandType::MiniHelp, {}, {}};
}
}  // namespace xtc::command