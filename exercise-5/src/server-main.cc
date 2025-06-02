#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "endpoints/Server.h"

int main() {
  namespace ttc = tt::chat;
  const int kPort = 8080;
  ttc::server::Server thisserverobject(kPort);
  return 0;
}
