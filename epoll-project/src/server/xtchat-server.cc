#include "xtchat-server.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>

#include "../common/command/command.h"

namespace xtc::server {
using xtc::command::Command;
using xtc::command::parse_line;

Server::Server(int port)
    : server_socket_fd_(xtc::net::create_socket()),
      port_(port),
      server_address_(xtc::net::create_address(port)) {
  set_non_blocking(server_socket_fd_);
  opt_bind_listen();
  epoll_fd_ = epoll_create1(0);
  check_error(epoll_fd_ < 0, "Couldn't make epoll socket");
  add_to_epoll(server_socket_fd_, EPOLLIN | EPOLLET);
}

Server::~Server() {
  close(epoll_fd_);
  close(server_socket_fd_);
}

// the one function to set socket options, address options,
// bind, and start listening on
// (the server socket file descriptor)
void Server::opt_bind_listen() {
  int err_code;

  // set the socket option to allow reusing address and port
  int opt = 1;
  err_code = setsockopt(server_socket_fd_, SOL_SOCKET,
                        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  xtc::check_error(err_code < 0, "setsockopt() error\n");

  // allow any address to connect
  server_address_.sin_addr.s_addr = INADDR_ANY;

  // bind socket to the address
  err_code = bind(server_socket_fd_, (sockaddr*)&server_address_,
                  sizeof(server_address_));
  xtc::check_error(err_code < 0, "bind failed\n");

  // listen on the server fd_
  err_code = listen(server_socket_fd_, 5);
  xtc::check_error(err_code < 0, "listen failed\n");
  std::cout << "Server listening on port " << port_ << "\n";
}

void Server::add_to_epoll(int sock, uint32_t events) {
  epoll_event ev;
  ev.events = events;
  ev.data.fd = sock;

  int err_code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock, &ev);
  xtc::check_error(err_code < 0, "Could not add event to epoll");
}

void Server::remove_from_epoll(int sock) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock, nullptr);
}

void Server::set_non_blocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  int err_code = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
  xtc::check_error(err_code < 0, "Failed to set server socket to non blocking");
}

void Server::run() {
  SPDLOG_INFO("Server started..!");
  epoll_event events[kMaxEvents];

  while (true) {
    int nfds = epoll_wait(epoll_fd_, events, kMaxEvents, -1);
    xtc::check_error(nfds < 0, "epoll_wait error");

    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == server_socket_fd_) {
        handle_new_connection();
      } else {
        handle_client_data(events[i].data.fd);
      }
    }
  }
}

void Server::handle_new_connection() {
  while (true) {
    sockaddr_in client_address;
    socklen_t client_addr_len = sizeof(client_address);
    int sock =
        accept(server_socket_fd_, (sockaddr*)&client_address, &client_addr_len);
    if (sock < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) break;
      check_error(true, "accept error");
    }
    set_non_blocking(sock);
    add_to_epoll(sock, EPOLLIN | EPOLLET);
    std::string username = "user-" + std::to_string(sock);

    socket_from_username_[username] = sock;
    username_from_socket_[sock] = username;

    SPDLOG_INFO(
        "New connection from client fd: {}, Assigned temporary username: @{}",
        sock, username);

    send_to_user(sock, help_text);
  }
}
void Server::handle_client_data(int sock) {
  char buffer[kBufferSize];

  while (true) {
    ssize_t bytes_read = recv(sock, buffer, kBufferSize, 0);
    if (bytes_read > 0) {
      client_read_buffers_[sock].append(buffer, bytes_read);
    } else if (bytes_read == 0) {

      purge_user(sock);
      return;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {

        break;
      }

      purge_user(sock);
      return;
    }
  }

  std::string& data = client_read_buffers_[sock];
  size_t pos;
  while ((pos = data.find('\n')) != std::string::npos) {
    std::string line = data.substr(0,pos);
   
    data.erase(0, pos + 1);

    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }

    if (line.empty()) {
      continue;
    }

    spdlog::info("Processing command line for fd {}: [{}]", sock, std::string(line));
    if (auto cmd = parse_line(line)) {
      handle_command(*cmd, sock);
    } else {
      spdlog::warn("Line from fd {} not parsed as command: [{}]", sock,
                   std::string(line));
    }
  }
}

void Server::handle_command(const Command& cmd, int sock) {
  switch (cmd.cmd) {
    case command::CommandType::Help:
      send_to_user(sock, help_text);
      break;
    case command::CommandType::Send: {
      const std::string& target = cmd.arg1;
      const std::string& message = cmd.arg2;
      if (!socket_from_username_.contains(target)) {
        send_to_user(sock, "No such user @" + target + "\n");
        return;
      }
      const std::string& from = username_from_socket_[sock];
      send_to_user(target, "@" + from + ": " + message + '\n');
      break;
    }
    case command::CommandType::SetUsername: {
      const std::string& new_name = cmd.arg1;
      if (socket_from_username_.contains(new_name)) {
      send_to_user(sock, "Username @" + new_name + " is already taken\n");
      } else {
      auto old_name = username_from_socket_[sock];
      socket_from_username_.erase(old_name);
      username_from_socket_[sock] = new_name;
      socket_from_username_[new_name] = sock;
      send_to_user(sock, "Username changed to @" + new_name + "\n");
      SPDLOG_INFO("Client fd {} changed username @{} -> @{}", sock, old_name, new_name);
      }
      break;
    }
    case command::CommandType::SendChannel: {
      const std::string& channel_name = cmd.arg1;
      const std::string& message = cmd.arg2;
      const std::string& from = username_from_socket_[sock];
      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "No such channel #" + channel_name + "\n");
        return;
      }
      auto& channel = channels_[channel_name];
      if (!channel.isMember(from)) {
        send_to_user(sock, "You are not a member of #" + channel_name + "\n");
        return;
      }
      send_to_channel(channel_name, from, message);
      break;
    }
    case command::CommandType::JoinChannel: {
      const std::string& channel_name = cmd.arg1;
      const std::string& username = username_from_socket_[sock];

      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "Channel #" + channel_name + " does not exist. Try creating it maybe.\n");
      } else {
        auto& channel = channels_[channel_name];
        if (channel.isMember(username)) {
          send_to_user(sock, "You are already a member of #" + channel_name + "\n");
        } else {
          channel.addMember(username);
          send_to_user(sock, "Joined channel #" + channel_name + "\n");
          SPDLOG_INFO("User @{} joined channel #{}", username, channel_name);
        }
      }
      break;
    }
    case command::CommandType::CreateChannel: {
      const std::string& channel_name = cmd.arg1;
      const std::string& username = username_from_socket_[sock];

      if (channels_.contains(channel_name)) {
      send_to_user(sock, "Channel #" + channel_name + " already exists.\n");
      } else {
      channels_[channel_name].addMember(username);
      channels_[channel_name].transferOwnership(username);
      send_to_user(sock, "Channel #" + channel_name + " created and joined.\n");
      SPDLOG_INFO("User @{} created channel #{}", username, channel_name);
      }
      break;
    }
    case command::CommandType::LeaveChannel: {
      const std::string& channel_name = cmd.arg1;
      const std::string& username = username_from_socket_[sock];

      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "Channel #" + channel_name + " does not exist.\n");
      } else {
        auto& channel = channels_[channel_name];
        if (!channel.isMember(username)) {
          send_to_user(sock, "You are not a member of #" + channel_name + "\n");
        } else {
          channel.removeMember(username);
          send_to_user(sock, "Left channel #" + channel_name + "\n");
          SPDLOG_INFO("User @{} left channel #{}", username, channel_name);
          // remove from all channels
          for (auto it = channels_.begin(); it != channels_.end(); ) {
            auto& channel = it->second;
            if (channel.isMember(username)) {
              channel.removeMember(username);
              // delete channel if it becomes empty
              if (channel.getMembers().empty()) {
                SPDLOG_INFO("Channel #{} deleted as last member @{} left", it->first, username);
                it = channels_.erase(it);
                continue;
              } else if (channel.getOwner() == username) {
                // make random guy owner
                const auto& members = channel.getMembers();
                auto new_owner = *members.begin();
                channel.transferOwnership(new_owner);
                SPDLOG_INFO("Ownership of channel #{} transferred from @{} to @{}", it->first, username, new_owner);
              }
            }
            ++it;
          }
        }
      }
      break;
    }
    case command::CommandType::TransferChannelOwnership: {
      const std::string& channel_name = cmd.arg1;
      const std::string& new_owner = cmd.arg2;
      const std::string& current_user = username_from_socket_[sock];

      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "Channel #" + channel_name + " does not exist.\n");
        break;
      }
      auto& channel = channels_[channel_name];
      if (channel.getOwner() != current_user) {
        send_to_user(sock, "You are not the owner of #" + channel_name + "\n");
        break;
      }
      if (!channel.isMember(new_owner)) {
        send_to_user(sock, "User @" + new_owner + " is not a member of #" + channel_name + "\n");
        break;
      }
      channel.transferOwnership(new_owner);
      send_to_user(sock, "Ownership of #" + channel_name + " transferred to @" + new_owner + "\n");
      SPDLOG_INFO("Ownership of channel #{} transferred from @{} to @{}", channel_name, current_user, new_owner);
      break;
    }
    case command::CommandType::DeleteChannel: {
      const std::string& channel_name = cmd.arg1;
      const std::string& username = username_from_socket_[sock];

      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "Channel #" + channel_name + " does not exist.\n");
      } else {
        auto& channel = channels_[channel_name];
        if (channel.getOwner() != username) {
          send_to_user(sock, "You are not the owner of #" + channel_name + "\n");
        } else {
          for (const auto& member : channel.getMembers()) {
            if (member != username) {
              send_to_user(member, "Channel #" + channel_name + " has been deleted by the owner.\n");
            }
          }
          channels_.erase(channel_name);
          send_to_user(sock, "Channel #" + channel_name + " deleted.\n");
          SPDLOG_INFO("Channel #{} deleted by owner @{}", channel_name, username);
        }
      }
      break;
    }
    case command::CommandType::ListAllChannels: {
      if (channels_.empty()) {
        send_to_user(sock, "No channels exist.\n");
      } else {
        std::string msg = "Channels:\n";
        for (const auto& [name, channel] : channels_) {
          msg += "#" + name + " (owner: @" + channel.getOwner() + ", members: " +
                 std::to_string(channel.getMembers().size()) + ")\n";
        }
        send_to_user(sock, msg);
      }
      break;
    }
    case command::CommandType::ListJoinedChannels: {
      const std::string& username = username_from_socket_[sock];
      std::vector<std::string> joined;
      for (const auto& [name, channel] : channels_) {
        if (channel.isMember(username)) {
          joined.push_back(name);
        }
      }
      if (joined.empty()) {
        send_to_user(sock, "You have not joined any channels.\n");
      } else {
        std::string msg = "Joined channels:\n";
        for (const auto& name : joined) {
          msg += "#" + name + "\n";
        }
        send_to_user(sock, msg);
      }
      break;
    }
    case command::CommandType::KickOut: {
      const std::string& channel_name = cmd.arg1;
      const std::string& target_user = cmd.arg2;
      const std::string& username = username_from_socket_[sock];

      if (!channels_.contains(channel_name)) {
        send_to_user(sock, "Channel #" + channel_name + " does not exist.\n");
        break;
      }
      auto& channel = channels_[channel_name];
      if (channel.getOwner() != username) {
        send_to_user(sock, "You are not the owner of #" + channel_name + "\n");
        break;
      }
      if (!channel.isMember(target_user)) {
        send_to_user(sock, "User @" + target_user + " is not a member of #" + channel_name + "\n");
        break;
      }
      channel.removeMember(target_user);
      send_to_user(sock, "User @" + target_user + " has been kicked out of #" + channel_name + "\n");
      if (socket_from_username_.contains(target_user)) {
        send_to_user(target_user, "You have been kicked out of #" + channel_name + " by the owner.\n");
      }
      SPDLOG_INFO("User @{} kicked out @{} from channel #{}", username, target_user, channel_name);
      break;
    }
case command::CommandType::MiniHelp: {
  send_to_user(sock, "Your last command wasn't parsed properly. Send /help for a list of valid commands and arguments.");
}
  }
}

void Server::send_to_user(int sock, std::string payload) {
  std::string username = username_from_socket_[sock];
  send_to_user(username, std::move(payload));
}

void Server::send_to_channel(const std::string& channel_name,
               const std::string& source_user,
               const std::string& payload) {
  if (!channels_.contains(channel_name)) {
  send_to_user(source_user, "No such channel #" + channel_name + "\n");
  return;
  }
  auto& channel = channels_[channel_name];
  for (const auto& member : channel.getMembers()) {
  if (member != source_user) {
    send_to_user(member, "@" + source_user + " in #" + channel_name + ": " + payload + '\n');
  }
  }
}

void Server::send_to_user(std::string username, std::string payload) {
  int sock = socket_from_username_[username];
  const char* cpayload = payload.c_str();
  size_t cpl_len = payload.length();
  send(sock, cpayload, cpl_len, 0);
  SPDLOG_INFO("Sent payload to {}", sock);
}

void Server::purge_user(const std::string username) {
  if (socket_from_username_.contains(username)) {
    int sock = socket_from_username_[username];
    socket_from_username_.erase(username);
    username_from_socket_.erase(sock);
    client_read_buffers_.erase(sock);
    remove_from_epoll(sock);
    close(sock);
    SPDLOG_INFO("Client @{} with fd: {} disconnected", username, sock);
  }
}

void Server::purge_user(int sock) {
  if (username_from_socket_.contains(sock)) {
    std::string username = username_from_socket_[sock];
    socket_from_username_.erase(username);
    username_from_socket_.erase(sock);
    client_read_buffers_.erase(sock);
    remove_from_epoll(sock);
    close(sock);
    SPDLOG_INFO("Client @{} with fd: {} disconnected", username, sock);
  }
}
}