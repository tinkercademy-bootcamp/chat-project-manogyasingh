#include <iostream>
#include <string>
#include <thread>

#include "client/xtchat-client.h"

int main() {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  xtc::client::Client client{kServerAddress, kPort};

  std::thread(
      [](xtc::client::Client &c) {
        while (true) {
          c.check_messages();
        }
      },
      std::ref(client))
      .detach();

  std::string payload = "";

  while (true) {
    std::getline(std::cin, payload);
    std::cout << "You entered: " << payload << "\n";
    ssize_t bytes_sent =
        send(client.socket_, payload.c_str(), payload.size(), 0);
    xtc::check_error(bytes_sent < 0, "Send error.\n");
    client.check_messages();
  }

  return 0;
}