# Group Exercise
#
# Come up with an interesting group project centered around a chat app
# over the TCP protocol
#
#  - Come up with a detailed timeline for the features you want to support
#  - Which team will work on which feature?
#  - How will you handle branching and Pull Requests?
#  - How do you determine which PRs get accepted by the group, and which PRs
#    get accepted into the main branch?
#  - What data structures would you need to send and recieve from the server? 
#  - What data format should the data structures be sent in?
#  - Keeping in mind that in HFT, we are seeking maximum ultra low latency
#    programming, what data format should we use to send our data?
#  - What if you had to scale this chat program up to support millions of
#    simultaneous users?
#  - I recommend experimenting with different data formats and measuring the
#    performance, instead of guessing.
#  - I will be curious to learn what you find out about the limitations of
#    various forms of measurement and what can be done about them.
#  - The Bonus section from exercise-3 will be helpful in this process
#  - It will be relevant to learn about why structs and classes need to
#    have padding and byte alignment
#  - It will be relevant to learn about `#pragma pack` and 
#    `__attribute__((packed))` 
#  - It will be relevant to learn more about `inet_ntop()` and `inet_pton()`.
#  - What is network byte order and what is system byte order, in the context
#    of these two functions?
#  - It will be relevant to watch this video 
#    [Type Punning in C++](https://www.youtube.com/watch?v=_qzMpk-22cc)
#  - With an additional explanation that for HFT, for performance reasons
#    the preference is to std::memcpy only a small metadata header
#      - Which includes a checksum. 
#      - If the checksum passes and the size of the received data matches 
#        the metadata 
#      - It is ok to use reinterpret_cast on the remaining payload.
#      - This is ok in a HFT context because the same company controls
#        both sides of the client and server and can control the platform
#        and compiler flags used for both builds