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
- Is this code better or worse than exercise-1?
- What are the tradeoffs compared to exercise-1?
- Are you able to spot any mistakes or inconsistencies in the changes?
  
## Thinking About Performance

- Does writing code this way have any impact on performance?
- What do we mean when we say performance?
- How do we measure performance in a program?

## Play with Git

- There isn't necessarily a single correct answer for how to abstract the 
  code from exercise-1 into functions
- Try different ways to refactor the code from exercise-1 to make it more
  readable.
- Make sure to commit each change as small and self-contained commit
- This will make it easier to revert your code if you need to
- What is `git tag`? How is `git tag` different from `git branch`?
- How can you use `git tag` and `git branch` to make programming easier and
  more fun?

## Learn Basics of Debugging in Your IDE

- How do you enable debug mode in your IDE?
- In debug mode, how do you add a watch?
- In debug mode, how do you add a breakpoint?
- In debug mode, how do you step through code?

### Memory Management and Debug Mode in Your IDE

- How do you see the memory layout of a `std::string` from your IDE debug mode?
- How do you see the memory layout of a struct from your IDE debug mode?