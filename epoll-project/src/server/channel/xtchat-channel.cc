#include "xtchat-channel.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace xtc::server{
Channel::Channel(std::string name, std::string owner)
        : name_(std::move(name)), owner_(std::move(owner)) {
      members_.insert(owner_);
    }
};