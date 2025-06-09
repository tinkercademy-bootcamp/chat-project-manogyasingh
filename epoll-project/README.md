# Creating a Chat Client/Server Using EPOLL

## Tasks
- Use the above resources and what you have learned to start a brand new C++ 
  project
- The server should be able to accept multiple connections at a time, as many
  as you can, to chat with each other
- Each client should be able to communicate with the server in some way so 
  that each client can be assigned a **username**
- Each client can send at least the following commands:
  - List channels
  - Go to channel
  - Create channel
- Whenever a user types in a message, it should be displayed to everyone else 
  in the channel
- You are free to come up with any other commands that you like
- You can use any directory structure or any makefile that you like
- You can use any 3rd party open source library that you like
- However for the purposes of this bootcamp, please stay with make and bash 
  scripts
- `cmake`, `ninja`, `conan` and `vcpkg` are all useful tools but are out of 
  scope for this bootcamp
- Some libraries that are likely to be useful:
- `sudo apt get ncurses-dev`
- For the more ambitious: `sudo apt get libsdl2-dev`

## Resources

- https://suchprogramming.com/epoll-in-3-easy-steps/ 
- https://github.com/onestraw/epoll-example/
- https://github.com/joakimthun/io-uring-echo-server
- https://man7.org/linux/man-pages/man7/epoll.7.html
- https://embetronicx.com/tutorials/linux/device-drivers/epoll-in-linux-device-driver/
- https://web.archive.org/web/20120504033548/https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/