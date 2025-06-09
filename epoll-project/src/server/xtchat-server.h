#ifndef XTC_SERVER_H
#define XTC_SERVER_H

#include <fcntl.h>
#include <netinet/in.h>

#include <string>
#include <unordered_map>

#include "../common/command/command.h"
#include "../common/error_handling/error_handling.h"
#include "../common/net/socket_helper.h"
#include "channel/xtchat-channel.h"

namespace xtc::server {

class Server {
 public:
  Server(int port);
  ~Server();
  void run();

 private:
  int port_;
  int epoll_fd_;
  int server_socket_fd_;
  sockaddr_in server_address_;

  // emulate a bidirectional map
  std::unordered_map<int, std::string> username_from_socket_;
  std::unordered_map<std::string, int> socket_from_username_;
  std::unordered_map<int, std::string> client_read_buffers_;
  std::unordered_map<std::string, xtc::server::Channel> channels_;

  void opt_bind_listen();
  void add_to_epoll(int sock, uint32_t events);
  void remove_from_epoll(int sock);
  void set_non_blocking(int sock);

  // overloaded send function to send by
  // either username or socket
  void send_to_user(std::string username, std::string payload);
  void send_to_user(int sock, std::string payload);
  void send_to_channel(const std::string& channel_name,
                       const std::string& source_user,
                       const std::string& payload);
  void purge_user(std::string username);
  void purge_user(int sock);

  static const int kMaxEvents = 64;
  static constexpr int kBufferSize = 1024;

  void handle_new_connection();
  void handle_client_data(int sock);

  void handle_command(const xtc::command::Command& cmd, int sock);

  std::string help_text =
      "\nUsage:\n"
      "/help                                       - Print this message\n"
      "/set_username @<name>                       - Set your username\n"
      "/send @<user> <message>                     - Send a private message to "
      "a user\n"
      "/create_channel #<channel>                  - Create a new channel\n"
      "/join_channel #<channel>                    - Join an existing channel\n"
      "/leave_channel #<channel>                   - Leave a channel you are "
      "in\n"
      "/send_channel #<channel> <message>           - Send a message to a "
      "channel\n"
      "/list_joined_channels                        - List channels you have "
      "joined\n"
      "/list_all_channels                          - List all available "
      "channels on the server\n"
      "/transfer_channel_ownership #<channel> @<user> - Transfer ownership of "
      "a channel\n"
      "/delete_channel #<channel>                  - Delete a channel you "
      "own\n"
      "/kickout @<user> #<channel>                 - Remove user from channel, but only if you're the channel owner.";
};
}  // namespace xtc::server

#endif  // XTCHAT_SERVER_H