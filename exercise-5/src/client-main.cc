#include <iostream>

#include "endpoints/Client.h"


std::string read_message(int argc, char *argv[]) {
  std::string message = "Hello from client";
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " <message>\n";
    exit(EXIT_FAILURE);
  } else {
    message = argv[1];
  }
  return message;
}

int main(int argc, char *argv[]) {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  std::string message = read_message(argc, argv);

  tt::chat::client::Client thisclient (kServerAddress,kPort);
  thisclient.send_and_receive_message(message);

  return 0;
}
