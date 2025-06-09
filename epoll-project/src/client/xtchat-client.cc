#include "xtchat-client.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

#include "../common/net/socket_helper.h"

namespace xtc::client {

Client::Client(std::string server_address, int port) {
  socket_ = net::create_socket();
  connect_to_server(create_server_address(server_address, port));
};

Client::~Client() { close(socket_); };

sockaddr_in Client::create_server_address(const std::string &server_ip,
                                          int port) {
  sockaddr_in address = net::create_address(port);
  auto err_code = inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);
  check_error(err_code <= 0, "Invalid address/ Address not supported\n");
  return address;
}

void Client::connect_to_server(sockaddr_in server_address) {
  auto err_code =
      connect(socket_, (sockaddr *)&server_address, sizeof(server_address));
  check_error(err_code < 0, "Connection Failed.\n");
};

void Client::send_message(std::string payload) {
  if (payload.empty() || payload.back() != '\n') {
    payload.push_back('\n');
  }
  ssize_t bytes_sent = send(socket_, payload.c_str(), payload.size(), 0);
  check_error(bytes_sent < 0, "Send error.\n");
  std::cout << "Sent: " << payload;
  std::cout << "==================================================\n";
}

bool Client::check_messages() {
  // Receive response from the server
  // very very simple for now, literally just prints everything in the buffer
  const int kBufferSize = 1024;
  char recv_buffer[kBufferSize] = {0};

  ssize_t read_size = read(socket_, recv_buffer, kBufferSize);
  check_error(read_size < 0, "Read error.\n");
  if (read_size > 0) {
    std::cout << "Received: \n---\n" << recv_buffer << "\n";
  } else if (read_size == 0) {
    std::cout << "Server closed connection.\n";
    return true;
  }
  std::cout << "==================================================\n";
  return false;
}

}  // namespace xtc::client