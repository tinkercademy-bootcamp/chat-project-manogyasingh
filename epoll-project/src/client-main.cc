#include <cstdlib>
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
          bool cease = c.check_messages();
          if (cease){
            std::exit(0);
          }
        }
      },
      std::ref(client))
      .detach();

  std::string payload = "";

  while (true) {
    std::cout << " > ";
    std::getline(std::cin, payload);
    if (std::cin.eof()) {
      break;
    }
    client.send_message(payload);
  }

  return 0;
}