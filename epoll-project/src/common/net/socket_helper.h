#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../error_handling/error_handling.h"

namespace xtc::net {
int create_socket();
sockaddr_in create_address(int port);
} // namespace xtc::net

#endif // SOCKET_HELPER_H