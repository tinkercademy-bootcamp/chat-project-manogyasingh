#include "server/chat-server.h"

int main() {
  const int kPort = 8080;
  tt::chat::server::Server server(kPort);
  return 0;
}