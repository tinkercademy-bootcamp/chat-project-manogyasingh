#include <cstdlib>
#include <iostream>
#include <netinet/in.h>

#ifndef COMMON_UTILS
#define COMMON_UTILS

template <typename T, typename S>
void check_error(T test, S error_message) {
  if (test) {
    std::cerr << error_message << "\n";
    exit(EXIT_FAILURE);
  }
}

sockaddr_in create_address(int port);
sockaddr_in create_address(const std::string &ip, int port);
int create_socket();

const int kBufferSize = 1024;
const int kPort = 8080;

#endif