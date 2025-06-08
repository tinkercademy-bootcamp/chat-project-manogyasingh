#ifndef XTC_CHANNEL_H
#define XTC_CHANNEL_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace xtc::server {

class Channel {
 public:
  // create a channel with given name and make the creator owner
  Channel(std::string name, std::string owner);

  ///////////////////////
  // membership//////////
  //////////////////////
  // should return true if username was not already a member
  bool addMember(const std::string& username);
  // should return true if username was a member
  bool removeMember(const std::string& username);
  bool isMember(const std::string& username) const;
  const std::unordered_set<std::string>& getMembers() const;

  bool transferOwnership(
      const std::string&
          newOwner);  // should only work if proposed owner is a member

  // helper: map usernames to sockets and return sockets of all current members
  std::vector<int> getMemberSockets(
      const std::unordered_map<std::string, int>& socketMap) const;

 private:
  std::string name_;
  std::string owner_;
  std::unordered_set<std::string> members_;
};

}  // namespace xtc::server

#endif  // XTC_CHANNEL_H