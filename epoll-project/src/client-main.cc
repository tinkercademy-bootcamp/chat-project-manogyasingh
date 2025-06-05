#include <iostream>
#include <string>

#include "client/xtchat-client.h"


int main() {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  xtc::client::Client client{kServerAddress, kPort};

  std::string payload = "";

  while (true) {
    std::cin >> payload;
    client.send_message(payload);
    client.check_messages();
  }

  return 0;
}