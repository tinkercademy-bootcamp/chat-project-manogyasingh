#ifndef XTC_CHANNEL_H
#define XTC_CHANNEL_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace xtc::server {

class Channel {
 public:
  Channel() = default;                                        //
  Channel(std::string name, std::string owner);               //
  bool addMember(const std::string& username);                //
  bool removeMember(const std::string& username);             //
  bool isMember(const std::string& username) const;           //
  bool transferOwnership(const std::string& newOwner);        //
  const std::unordered_set<std::string>& getMembers() const;  //
  const std::string& getName() const;                         //
  const std::string& getOwner() const;                        //

 private:
  std::string name_;
  std::string owner_;
  std::unordered_set<std::string> members_;
};

}  // namespace xtc::server

#endif  // XTC_CHANNEL_H