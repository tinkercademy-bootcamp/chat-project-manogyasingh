#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const int kBufferSize = 1024;

int create_socket() {
  int my_sock;
  if ((my_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error\n";
    exit(EXIT_FAILURE);
  }
  return my_sock;
}

bool set_socket_options(int sock, int opt) {
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) < 0) {
    std::cerr << "setsockopt() error\n";
    close(sock);
    exit(EXIT_FAILURE);
  }
  return true;
}

sockaddr_in create_address(int port) {
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  return address;
}

void bind_address_to_socket(int sock, sockaddr_in &address) {
  if (bind(sock, (sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "bind failed\n";
    close(sock);
    exit(EXIT_FAILURE);
  }
}

void listen_on_socket(int sock) {
  if (listen(sock, 3) < 0) {
    std::cerr << "listen failed\n";
    close(sock);
    exit(EXIT_FAILURE);
  }
}

void start_listening_on_socket(int my_socket, sockaddr_in &address) {
  const int kSocketOptions = 1;
  set_socket_options(my_socket, kSocketOptions);

  bind_address_to_socket(my_socket, address);
  listen_on_socket(my_socket);
}

void handle_accept(int client_socket) {
  char buffer[kBufferSize] = {0};
  ssize_t valread = read(client_socket, buffer, kBufferSize);

  if (valread > 0) {
    std::cout << "Received: " << buffer << "\n";
    send(client_socket, buffer, valread, 0);
    std::cout << "Echo message sent\n";
  } else if (valread == 0) {
    std::cout << "Client disconnected.\n";
  } else {
    std::cerr << "Read error on client socket " << client_socket << "\n";
  }
  close(client_socket);
}

void handle_connections(int sock, int port) {
  sockaddr_in address = create_address(port);
  socklen_t address_size = sizeof(address);

  // #Question - is it good to have an infinite loop?
  /*
  At current scale and scope it's good because it fits exactly what we're trying to do.
  We're doing:
  1. Create a socket
  2. Accept connections to it
  3. For any connection, allow client to send a message
  4. echo the message back
  5. if connection breaks, restart a connection
  6. also, restart socket and accept connections if the current one fails during initialisation

  This constant restart behaviour is what justifies using an infinite while loop for now.

  As a best practice though, infinite loops are not recommnded because of several reasons.
  These reasons include the fact that infinite loops are harder to debug.
  Also also the design principle that it implies that the content is suppose to run indefinitely under normal conditions (which no code should be doing)
  */
  while (true) {
    int accepted_socket = accept(sock, (sockaddr *)&address, &address_size);
    if (accepted_socket < 0) {
      std::cerr << "accept error\n";
      // we continue to accept new connections if possible.
      continue;
    }
    handle_accept(accepted_socket);
  }
}

int main() {
  const int kPort = 8080;
  int my_socket = create_socket();
  sockaddr_in address = create_address(kPort);

  // #Question - is there a better name for this function?
  // This is a good name. Descriptive and doesn't violate capitalisation principles.
  // It is bit long however. Could have also called it socket_listen_start() 
  start_listening_on_socket(my_socket, address);
  std::cout << "Server listening on port " << kPort << "\n";
  handle_connections(my_socket, kPort);
  close(my_socket);

  return 0;
}