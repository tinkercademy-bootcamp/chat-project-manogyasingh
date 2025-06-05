#include "xtchat-client.h"

#include "../common/net/socket_helper.h"

namespace xtc::client {
Client::Client(std::string server_address, int port) {
  server_address_ = create_server_address(server_address, port);
  socket_ = net::create_socket();
  connect_to_server();
};
Client::~Client() {

};

sockaddr_in Client::create_server_address(const std::string &server_ip,
                                          int port) {
  sockaddr_in address = net::create_address(port);
  auto err_code = inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);
  check_error(err_code <= 0, "Invalid address/ Address not supported\n");
  return address;
}

void Client::connect_to_server() {
  auto err_code =
      connect(socket_, (sockaddr *)&server_address_, sizeof(server_address_));
  check_error(err_code < 0, "Connection Failed.\n");
};

std::string Client::send_message(std::string target_username,
                                 std::string message) {
  const int kBufferSize = 1024;
  char recv_buffer[kBufferSize] = {0};

  // Send the message to the server
  send(socket_, message.c_str(), message.size(), 0);
  std::cout << "Sent: " << message << "\n";

  // Receive response from the server
  ssize_t read_size = read(socket_, recv_buffer, kBufferSize);
  check_error(read_size < 0, "Read error.\n");
  if (read_size > 0) {
    std::cout << "Received: " << recv_buffer << "\n";
  } else if (read_size == 0) {
    std::cout << "Server closed connection.\n";
  }
  return std::string(recv_buffer);
}
}  // namespace xtc::client