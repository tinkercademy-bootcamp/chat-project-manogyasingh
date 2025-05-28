# Exercise 2

**Update this README with your answers to the questions below.**

## Sources of Information for Questions from Before

### Socket 
- https://man7.org/linux/man-pages/man2/socket.2.html - System call reference
  for creating communication endpoints
- Or type `man socket` in terminal
    - This seems to be the actual syscall with the description `int socket(int domain, int type, int protocol);`
    - I actually tried to call this from the command line too but it turned out its an API **in the kernel** and not an installed component or tool
- https://man7.org/linux/man-pages/man7/socket.7.html - Socket interface 
  overview and protocol families
- Or type `man 7 socket` in terminal
  - Had to install `mandb` and `manpages` but got it working
  - This seems to be the interface for C++
- When would you want to use a `SOCK_RAW` stream?
  - Instead of interacting with an OSI level 4 interface, `SOCK_RAW` presents an interface directly for the lower level protocols like IP or ICMP. It can be used for implementing new protocols, raw inspection of internet data (like when sniffing with wireshark) 

### TCP and IP Protocols
- [IPv4](https://www.rfc-editor.org/info/rfc791) - Internet Protocol 
  specification defining packet structure and routing
- [IPv6](https://www.rfc-editor.org/info/rfc8200) - Next-generation Internet 
  Protocol with expanded address space
- [TCP](https://datatracker.ietf.org/doc/html/rfc9293) - Transmission Control 
  Protocol providing reliable, ordered data delivery
    
### C++
- [C++23 ISO standard draft](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf) - 
  Working draft of the C++ language specification
- Is the above the official C++23 spec? 
  - This is a working draft. It even says "Note: this is an early draft. It’s known to be incomplet and incorrekt, and it has lots of bad formatting." XD
  - But its the final draft and is technically identical to the official version.
- Where is the official C++23 spec?
  - https://www.iso.org/standard/83626.html
- Why was this link chosen instead?
  - Official standards in ISO are paywalled to support the institute's administration costs
  - The final draft given above is the closest document to it (in fact it's identical) that is free.
- Is this a helpful reference for learning C++?
  - Definitely a helpful resource but not for learning.
  - It's too technical as unambiguity takes precedence over readability.
  - It is however the best and most authoritative source on C++ standards 
- Can the various implementations of C++ compilers be different from the C++ standard?
  - None of them can differ from it in the sense that none can violate the established standard
  - However, the spec can still leave gaps. Notably, it doesn't mention the "how" of implementations, just the "what"
  - This means the actual implementations can differ from one another, while still agreeing the official standard
   
- What are the most widely used and most significant C++ compilers?
  - GCC
  - Clang
  - MS Visual C++
  - Intel C++
  - MinGW-GCC
- Where is the equivalent spec for C++26?
  - It's in development. Here's the latest draft: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/n5008.pdf
   And this how I know: https://isocpp.org/blog/category/standardization
- Where do you find the spec for the HTTP protocol?
  - RFC9110: https://www.rfc-editor.org/rfc/rfc9110.html
  - RFC9110 through 9114 decribe HTTP's semantics, caching, version 1.1, 2 and 3 in detail.
- What about HTTPS? Is there a spec for that protocol?
  - HTTPS isn't a seperate protocol in itself but instead HTTP wrapped in Transport Layer Security (which does have a protocol spec).
  - RFC8446 describes the latest TLS 1.3


## Introduction to C++ and Sockets Programming

- Read the code in `src/`
- Are there any bugs in this code? 
  - There was one in the read_args() question
  - it was causing it to never run the base case for zero arguments but instead to exit.
- What can you do to identify if there are bugs in the code?
  - Test it on a lot of inputs including potential edge cases
  - Get it "Audited" by someone else
  - Read it again very very carfully and see if you find something
## Refactoring: Extract Function

- What is different in this code compared to exercise-1?
  - The monolithic main() function has been broken down into smaller functions
``` 
    create_socket() - handles socket creation
    set_binary_address() - converts IP address
    create_address() - creates sockaddr_in 
    connect_to_server() - handles connection
    send_and_receive_message() - handles communication over the socket
    read_args() - CLI argument parser
```
  - Server now accepts communications indefinitely
- Is this code better or worse than exercise-1?
  - It's better
  - more readable, more maintainable
  - easier to test individual components or trace a failure
- What are the tradeoffs compared to exercise-1?
  - we got above listed pros and below listed cons
  - code got larger
  - control flow is slightly harder to track with all the different functions
- Are you able to spot any mistakes or inconsistencies in the changes?
  - there was one in the read_args which I described in the previous section
  - this version actually fixes a bug instead of creating more. the server response buffer wasn't cleaned for every message in the previous version. this one does it.
  
## Thinking About Performance

- Does writing code this way have any impact on performance?
  - The extra functions called should in theory add some overhead
  - But its negligible compared to the time of actually running the functions
- What do we mean when we say performance?
  - It refers to the complexities (in time and space) and also to the overhead of running it
  - more accurately it refers to how many operations and how much memory are needed to run the program with input of various sizes
- How do we measure performance in a program?
  - timing the program is one option (time or std::chrono)
  - however this may not tell the complete story due to things like hardware differences, and also it may not be accurate enough for what we need it to be
  - we need to profile it with tools like `gprof` to understand *where* its spending it's time

## Play with Git

- There isn't necessarily a single correct answer for how to abstract the 
  code from exercise-1 into functions
- Try different ways to refactor the code from exercise-1 to make it more
  readable.
- Make sure to commit each change as small and self-contained commit
- This will make it easier to revert your code if you need to
- What is `git tag`? How is `git tag` different from `git branch`?
  - you can tag specific versions of the project in the tree to be specific versions using git tag, eg, `git tag v204.2.31`
  - it's different in the sense that a tag is just a marker, it's not a parallel branch of development which can be merged or worked on without affecting other branches
- How can you use `git tag` and `git branch` to make programming easier and
  more fun?
  - setup naming conventions for the different branches and versions.
  - say for example the `x.y.0` names will be reserved for the main branch. `x.y.50+` will be for beta testing releases as experiments
  - this will help keep the codebase organised and make it easier to track changes across branches and versions without confusion about what should be merged where and in what circumstances

## Learn Basics of Debugging in Your IDE

- How do you enable debug mode in your IDE?
  - F5 launches the run and debug panel.
  - Had to configure launch.json and tasks.json but it seems too be working now
- In debug mode, how do you add a watch?
  - Go to watch panel and click on "+". It will ask for the expression to watch. Enter the variable name or expression, eg, `message` and it will be added to the watch list
- In debug mode, how do you add a breakpoint?
  - click on the left of the line number
- In debug mode, how do you step through code?
  - F5 - Continueruns normally till it hits the next breakpoint
  - F10 - Step Over - skips the next line entirely and moves to the one after that. Notably it will skip the entire function if the next line has one starting there.
  - F11 - Step Into - Move inside the current function or even the line.
  - Shift F11 - Step Out - Get out of the current function

### Memory Management and Debug Mode in Your IDE

- How do you see the memory layout of a `std::string` from your IDE debug mode?
  - Add a breakpoint at a place where the `std::string` object is in scope
  - Add it to watch
  - Expand the object in the panel, it will show the whole memory layout
- How do you see the memory layout of a struct from your IDE debug mode?
  - The same way. Tried it for `sockaddr_in address` and it worked