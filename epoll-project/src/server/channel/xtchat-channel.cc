#include "xtchat-channel.h"
#include <string>
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

bool Channel::removeMember(const std::string& username) {
    return members_.erase(username) > 0;
}

bool Channel::isMember(const std::string& username) const {
    return members_.find(username) != members_.end();
}

bool Channel::transferOwnership(const std::string& newOwner) {
    if (isMember(newOwner)) {
        owner_ = newOwner;
        return true;
    }
    return false;
}

const std::unordered_set<std::string>& Channel::getMembers() const {
    return members_;
}

const std::string& Channel::getName() const {
    return name_;
}

const std::string& Channel::getOwner() const {
    return owner_;
}

};  // namespace xtc::server