#include "server/xtchat-server.h"

int main() {
  const int kPort = 8080;
  xtc::server::Server server(kPort);
  server.run();
  return 0;
}