#include <iostream>

#include "endpoints/Server.h"

int main() {

  const int kPort = 8080;
  std::cout << "Starting Server...\n";
  tt::chat::server::Server thisserver(kPort);

  return 0;
}