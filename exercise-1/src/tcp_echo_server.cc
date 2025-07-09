#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  const int kPort = 8080;
  sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  const int kBufferSize = 1024;
  char buffer[kBufferSize] = {0};
  int my_sock;
  int opt = 1;
  // Creating socket file descriptor
  if ((my_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation erron\n";
    return -1;
  }
  // Attaching socket to port
  if (setsockopt(my_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    std::cerr << "setsockopt error\n";
    return -1;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(kPort);
  // Bind the socket to the network address and port
  if (bind(my_sock, (sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "bind failed\n";
    return -1;
  }
  // Start listening for incoming connections
  if (listen(my_sock, 3) < 0) {
    std::cerr << "listen failed\n";
    return -1;
  }
  std::cout << "Server listening on port " << kPort << "\n";
  // Accept incoming connection
  int new_sock;
  while (true) {
    new_sock = accept(my_sock, (struct sockaddr *)&address, &addrlen);
    if (new_sock < 0) {
      std::cerr << "accept error\n";
      return -1;
    }
    // Wait for read
    ssize_t read_size = read(new_sock, buffer, kBufferSize);
    std::cout << "Received: " << buffer << "\n";
    // Send reply
    send(new_sock, buffer, read_size, 0);
    std::cout << "Echo message sent" << "\n";
  }
  // Close the socket
  close(new_sock);
  close(my_sock);
  return 0;
}
