#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void check_error(bool test, const char *error_message) {
  if (test) {
    std::cerr << error_message << "\n";
    exit(EXIT_FAILURE);
  }
}

int create_socket() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  check_error(sock < 0, "Socket creation error\n");
  return sock;
}

void set_binary_address(sockaddr_in &address, const std::string &server_ip) {
  auto err_code = inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);
  check_error(err_code <= 0, "Invalid address/ Address not supported\n");
}

sockaddr_in create_address(const std::string &server_ip, int port) {
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  set_binary_address(address, server_ip);
  return address;
}

void connect_to_server(int sock, sockaddr_in &server_address) {
  auto err_code =
      connect(sock, (sockaddr *)&server_address, sizeof(server_address));
  check_error(err_code < 0, "Connection Failed\n");
}

void send_and_receive_message(int sock, const std::string &message) {
  const int kBufferSize = 1024;
  char buffer[kBufferSize] = {0};

  // Send the message to the server
  send(sock, message.c_str(), message.size(), 0);
  std::cout << "Sent: " << message << "\n";

  // this can be refactored to use the check_error() function but I'm choosing
  // not to do that this is because we want this function to just output to
  // stderr and then gracefully close the socket instead of just exiting from
  // execution Receive response from the server
  ssize_t read_size = read(sock, buffer, kBufferSize);
  if (read_size > 0) {
    std::cout << "Received: " << buffer << "\n";
  } else if (read_size == 0) {
    std::cout << "Server closed connection.\n";
  } else {
    std::cerr << "Read error\n";
  }
}

std::string read_args(int argc, char *argv[]) {
  std::string message;
  std::string error_msg = std::string("Usage: ") + argv[0] + " < message >\n";
  check_error(argc == 1, error_msg.c_str());
  message = argv[1];
  return message;
}

int main(int argc, char *argv[]) {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";
  std::string message = read_args(argc, argv);
  int my_socket = create_socket();sockaddr_in server_address = create_address(kServerAddress, kPort);
  connect_to_server(my_socket, server_address);
  send_and_receive_message(my_socket, message);
  close(my_socket);

  return 0;
}