#include "xtchat-channel.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <spdlog/spdlog.h>
#include <sys/socket.h>

namespace xtc::server {
Channel::Channel(std::string name, std::string owner)
    : name_(std::move(name)), owner_(std::move(owner)) {
  members_.insert(owner_);
}

bool Channel::addMember(const std::string& username) {
    return members_.insert(username).second;
}

void Channel::sendMessageToAll(
    const std::string& message,
    const std::unordered_map<std::string, int>& socketMap) {
  for (const auto& member : members_) {
    auto it = socketMap.find(member);
    if (it != socketMap.end()) {
    int sockfd = it->second;
    send_to_user(sockfd, message);
    }
  }
}

void Channel::send_to_user(int sock, std::string payload) {
  const char* cpayload = payload.c_str();
  size_t cpl_len = payload.length();
  send(sock, cpayload, cpl_len, 0);
  SPDLOG_INFO("Sent payload to {}", sock);
}

};  // namespace xtc::server