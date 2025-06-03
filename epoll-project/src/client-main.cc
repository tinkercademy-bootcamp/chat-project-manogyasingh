#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/chat-client.h"

std::string read_username () {
  std::cout << "Choose a username! @";
  std::string username;
  std::cin >> username;
  return username;
}

void get_message (std::string &target_user, std::string &message){
  std::cout << "Send to @";
  std::cin >> target_user;
  std::cout << "Enter your message: ";
  std::cin >> message;
}

int main() {
  const int kPort = 8080;
  const std::string kServerAddress = "127.0.0.1";

  std::string username = read_username();
  std::string target_user = "";
  std::string message = "";

  tt::chat::client::Client client{kPort, kServerAddress};

  while (true){
    get_message(target_user,message);
    std::string response = client.send_message(target_user,message);
  }

  return 0;
}