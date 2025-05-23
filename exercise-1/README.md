# Exercise 1

**Update this README with your answers to the questions below.**

## Learning How to Learn

- Answer the following questions in this file and commit and push your changes.
- Bonus sections are more difficult and optional.
- How can you find the information required to complete these tasks?
  - Google and the big open internet!
- How can you tell if the source of your information is good?
  - Official sources are better. Otherwise the more viewed something is, the better. Reddit can always be a place to verify the sources
- How would you define "good" in this situation?
  - Clear, looks (and is!) trustworthy, exactly as concise or verbose as we need it to be

## Learn Basics of g++ CLI

- Compile the TCP client and server using `g++` from command line.
  - Done!
- What are the most important command line arguments to learn for `g++`?
  - input files, output file, and flags like these:
```
  -c        compiles without linking
  -o        specifies output file
  -Wall     show all warnings
  -Werror   warnings treated as errors
```
- What is the difference between debug vs release versions?
  - The difference is related to the fact that during debugging we usually want more logging and crude control over the program. We want to be able to enable and disable features and manually change its internal data and state for testing. The debug version being untested (or more like under testing) is unstable and much more likely to crash or fail in an unexpected way.
- What are the tradeoffs between debug and release versions?

| Debug    | Release |
| -------- | ------- |
|More Control|Stable|
|More Transparent|More Secure|
|Developer Tools|Friendly UI|
- What arguments would you use in a debug build?
```
-Wall     show all warnings
-Werror   treat warnings as errors
-Wextra   extra warnings
-g        shows some debug information
-O0       disable optimisations so that stuff compiles as expected
```
- What about for release?
```
-O2 to enable optimisations
-DNDEBUG to disable assert statements if needed
```
- What other kinds of build types are useful?

## Learn Basics of Make                          // TODO

- Create a Makefile that will speed up the process.
  - Done!
- [Quickstart tutorial to make](https://makefiletutorial.com/) - Learn make fundamentals with practical examples and common patterns.
  - Read enough to move forward, will read more later // TODO  
- How else can you learn about make?
  - so ther resource above is pretty useful. the gnu docs are also good for reference: https://www.gnu.org/software/make/manual/
- How can you tell if the resource you are using is correct?
  - reddit is usually a good place to find out if a resource is legit
- Create a makefile such that when you run `make` with no arguments, it will:
  - Create `build/` directory if it does not exist
  - Create executables **client** and **server** in `build/`, if needed
  - How does make know when it needs to rebuild the executables?
    - specify `requirements`
  - Change your Makefile such that `make clean` will remove `build/` and all its contents.
    - Done!
- What are the most important command line arguments to learn for make? // TODO
- What are the most important directives to learn about in Makefile? // TODO
- What are the most important commands to implement in your Makefile? // TODO
- Which ones are essential, which ones are nice to haves? // TODO

## Learn Basics of Git

- Read through the code in `src/`
    - Done!
- Answer any `#Questions` as a comment
    - TODO
- Commit and push your changes to git
    - TODO
- Each commit should be responding to a single task or question
- Why is it important to keep your commit to a single task or question?
  - its like the single responsibility principle applied to git. makes it easier to understand what each commit is about and also to trace faults later
- Is it better to have a lot of very small commits, or one big commit when everything is working?
  - Lots of small commits will make it easier to trace the fault if/when something eventually fails.
- What are the most important commands to know in git?
```sh
git init                            // initialise a new repo
git add <stuff>                     // track files
git clone <repo>                    // clone a repo
git commit -m "commit message yay"  // commit your changes
git remote add upstream <repo>      // add a remote address!
git push origin main                // push changes on your main to the origin
git branch -M newname               // rename current branch
git checkout (-b) branchname        // (create and) switch to branch branchname
git fetch                    // fetch changes in your remote branches without merging
git merge                           // merge two branches (using a certain "strategy")
git pull                            // apply changes on the remote branch to your branch. basically fetch and merge
git rebase                          // apply your commits to the newer version of the repo
```

/// EVERYTHING BELOW THIS IS A TODO

## Introduction to Sockets

- Read the code in `src/tcp-echo-client.cc` and add a way to change the 
  message sent using command line arguments
- **Example**: `./client "hello message from the command prompt"` should send
  `"hello message from the command prompt"` to the server
- Commit your changes into git
- What do all these headers do?
- How do you find out which part of the below code comes from which header?
- How do you change the code so that you are sending messages to servers
  other than localhost?
- How do you change the code to send to a IPv6 address instead of IPv4?
- **Bonus**: How do you change the client code to connect by hostname instead
  of IP address?
  
## Introduction to Memory Management

- What is happening in line 26 of `tcp-echo-client.cc`? 
  `if (inet_pton(AF_INET, kServerAddress.c_str(), &address.sin_addr) <= 0) {`
- What is happening in line 31 of `tcp-echo-client.cc`?
  `if (connect(my_sock, (sockaddr *)&address, sizeof(address)) < 0) {`
- What is the difference between a pointer and a reference?
- When is it better to use a pointer?
- When is it better to use a reference?
- What is the difference between `std::string` and a C-style string?
- What type is a C-style string?
- What happens when you iterate a pointer?
- What are the most important safety tips to know when using pointers?

## Learn Basics of Creating a C++ Project in Your IDE

- How do you compile and run your project in your IDE?
  - From the terminal as usual:
```
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1/src$ g++ tcp_echo_client.cc 
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1/src$ g++ tcp_echo_server.cc
```

## Improving Interactions with LLMs

- What is the most authoritative source of information about `socket()`
  from `<sys/socket.h>`?
- What is the most authoritative source of information about the TCP and IP
  protocols?
- What is the most authoritative source of information about the C++
  programming language?
- What information can you find about using Markdown when structuring prompts 
  to LLMs?

- What is the difference between LLM and AI?
AI refers to all forms of systems to have computer make decisions (or content)
- Is it grammatically correct in English to say "a LLM" or "an LLM"? Why?
"an" LLM because it starts with an "el" sound