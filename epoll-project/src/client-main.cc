#include <iostream>
#include <string>

#include "client/xtchat-client.h"

void get_message(std::string &target_user, std::string &message) {
  std::cout << "Send to @";
  std::cin >> target_user;
  std::cout << "Enter your message: ";
  std::cin >> message;
}

int main() {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  xtc::client::Client client{kServerAddress, kPort};

  std::string target_user = "";
  std::string message = "";

  while (true) {
    get_message(target_user, message);
    std::string response = client.send_message(target_user, message);
  }

  return 0;
}