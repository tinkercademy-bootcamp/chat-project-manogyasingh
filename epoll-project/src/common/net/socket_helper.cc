#include "socket_helper.h"

int xtc::net::create_socket() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  xtc::check_error(sock < 0, "Socket creation error\n");
  return sock;
}

sockaddr_in xtc::net::create_address(int port) {
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  return address;
}
