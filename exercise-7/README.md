# Exercise 7

**Update this README with your answers to the questions below.**

## Resources for Learning About EPOLL

- https://suchprogramming.com/epoll-in-3-easy-steps/ 
- https://github.com/onestraw/epoll-example/
- https://github.com/joakimthun/io-uring-echo-server
- https://man7.org/linux/man-pages/man7/epoll.7.html
- What other resources can you find about epoll?
  - https://linux.die.net/man/4/epoll : The epoll(4) man page. Turns out this isn't relevant as this is supposed to refer to the device files (in `/dev/`) but epoll doesn't use them. 
  - https://en.wikipedia.org/wiki/Epoll wiki page
  - found this tutorial that may be helpful: https://embetronicx.com/tutorials/linux/device-drivers/epoll-in-linux-device-driver/
  - found one more : https://web.archive.org/web/20120504033548/https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/

- Are the above code repos examples of good code or bad code?
  - looks okay
  - the first repo has a lot of stars so it's probably going to be good
- How do you decide what is good code or bad code?
  - if a codebase is easy to read, explain and extend then its good
  - usually you can look at metrics like stars on github repo to get an idea


## Creating a Chat Client/Server Using EPOLL

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

## Adding a Third Party Library from Source

- Not all useful open source libraries will be available as a debian package
- For the purposes of this workshop, your only options are to install a 
  shared library into `usr/include` and `usr/lib`, or to git clone
  the source code and build it
- What are the tradeoffs between using a library from `usr/lib` vs git cloning
  the source and using that instead?

  |/usr/lib/|git clone|
  |-|-|
  |Easier to manage (can use system package managers for it)|Will have to use `git pull` and then rebuild|
  |limited to the version availabe in your pm's repo (you could theoretically add new sources or just install it there yourself but that effort is a tradeoff in itself)|bleeding edge all the time|
  |you'll need root privileges|works without root|
  |integrates with your whole system|integrates only with your project|
- What is the difference between `-l` and `-L` when linking using `g++`?
  - `-L` specifies the directory to look in
  - `-l` specifies the library to look for