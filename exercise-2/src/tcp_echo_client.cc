#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int create_socket() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Socket creation error\n";
    exit(EXIT_FAILURE);
  }
  return sock;
}

void set_binary_address(sockaddr_in &address, const std::string &server_ip) {
  if (inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported\n";
    exit(EXIT_FAILURE);
  }
}

sockaddr_in create_address(const std::string &server_ip, int port) {
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  set_binary_address(address, server_ip);
  return address;
}

void connect_to_server(int sock, sockaddr_in &server_address) {
  if (connect(sock, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    close(sock);
    std::cerr << "Connection Failed\n";
    exit(EXIT_FAILURE);
  }
}

void send_and_receive_message(int sock, const std::string &message) {
  const int kBufferSize = 1024;
  // #Question - is buffer the best name we can use?
  /*
  it's okay. although we could have called it
  message_buffer or response_buffer for further clarity
  */
  char buffer[kBufferSize] = {0};

  // Send the message to the server
  send(sock, message.c_str(), message.size(), 0);
  std::cout << "Sent: " << message << "\n";

  // Receive response from the server
  ssize_t read_size = read(sock, buffer, kBufferSize);
  if (read_size > 0) {
    std::cout << "Received: " << buffer << "\n";
  } else if (read_size == 0) {
    std::cout << "Server closed connection.\n";
  } else {
    std::cerr << "Read error\n";
  }
}

// #Question - what can be improved in this function?
// We can completely remove the whole exit logic. Given we have a graceful exit backup
// Perhaps in that case we would also want to print the result
// So, we should move the default message line inside the argc==1 condition and cancel the exit
// old version
// std::string read_args(int argc, char *argv[]) {
//   std::string message = "Hello from client";
//   if (argc == 1) {
//     std::cout << "Usage: " << argv[0] << " <message>\n";
//     exit(EXIT_FAILURE);
//   }
//   if (argc > 1) {
//     message = argv[1];
//   }
//   return message;
// }

// new version
std::string read_args(int argc, char *argv[]) {
  std::string message;
  if (argc == 1) {
    message = "Hello from client";
    std::cout << "Usage: " << argv[0] << " <message>\nDefaulting message to \""<<message<<"\"\n";
  } else {
    message = argv[1];
  }
  return message;
}

int main(int argc, char *argv[]) {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  std::string message = read_args(argc, argv);

  int my_socket = create_socket();
  sockaddr_in server_address = create_address(kServerAddress, kPort);

  connect_to_server(my_socket, server_address);
  send_and_receive_message(my_socket, message);
  close(my_socket);

  return 0;
}