#include "Client.h"
#include "../utils.h"
#include "../net/chat-sockets.h"
#include <arpa/inet.h>

sockaddr_in Client::create_server_address(const std::string &server_ip,
                                          int port) {
  using namespace tt::chat;
  sockaddr_in address = net::create_address(port);
  // Convert the server IP address to a binary format
  auto err_code = inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);
  check_error(err_code <= 0, "Invalid address/ Address not supported\n");
  return address;
}