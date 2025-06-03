#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server/chat-server.h"

int main() {
  const int kPort = 8080;

  tt::chat::server::Server server(kPort);
  server.handle_connections();

  return 0;
}
