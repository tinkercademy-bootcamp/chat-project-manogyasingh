#ifndef SERVER_H
#define SERVER_H

#include "../utils.h"

namespace tt::chat::server {
class Server {
    public:
  static void set_socket_options(int sock, int opt);
};
}

#endif