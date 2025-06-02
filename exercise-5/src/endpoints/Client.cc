#include "Client.h"
#include "../net/chat-sockets.h"

namespace tt::chat::client {
  Client::Client(std::string server_address, int port) {
    server_address_ = create_server_address(server_address, port);
    socket_fd_ = tt::chat::net::create_socket();
    connect_to_server();
  };
  Client::~Client(){

  };

  sockaddr_in Client::create_server_address(const std::string &server_ip,
                                            int port) {
    using namespace tt::chat;
    sockaddr_in address = net::create_address(port);
    // Convert the server IP address to a binary format
    auto err_code = inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);
    check_error(err_code <= 0, "Invalid address/ Address not supported\n");
    return address;
  }

  void Client::connect_to_server() {
    using namespace tt::chat;
    auto err_code = connect(socket_fd_, (sockaddr *)&server_address_,
                            sizeof(server_address_));
    check_error(err_code < 0, "Connection Failed.\n");
  };

  void Client::send_and_receive_message(const std::string &message) {
    using namespace tt::chat;
    const int kBufferSize = 1024;
    char recv_buffer[kBufferSize] = {0};

    // Send the message to the server
    send(socket_fd_, message.c_str(), message.size(), 0);
    std::cout << "Sent: " << message << "\n";

    // Receive response from the server
    ssize_t read_size = read(socket_fd_, recv_buffer, kBufferSize);
    check_error(read_size < 0, "Read error.\n");
    if (read_size > 0) {
      std::cout << "Received: " << recv_buffer << "\n";
    } else if (read_size == 0) {
      std::cout << "Server closed connection.\n";
    }
  }
}