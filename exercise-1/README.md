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

## Learn Basics of Make

- Create a Makefile that will speed up the process.
  - Done!
- [Quickstart tutorial to make](https://makefiletutorial.com/) - Learn make fundamentals with practical examples and common patterns.
  - Done!
- How else can you learn about make?
  - so the resource above is pretty useful. the GNU docs are also good for reference: https://www.gnu.org/software/make/manual/
- How can you tell if the resource you are using is correct?
  - reddit is usually a good place to find out if a resource is legit
- Create a makefile such that when you run `make` with no arguments, it will:
  - Create `build/` directory if it does not exist
  - Create executables **client** and **server** in `build/`, if needed
  - How does make know when it needs to rebuild the executables?
    - specify `requirements`
  - Change your Makefile such that `make clean` will remove `build/` and all its contents.
    - Done!
- What are the most important command line arguments to learn for make?
```
target						specifies what to build
-f       					choose custom file as makefile
-n or --just-print			just print the commands instead of actually executing
-s or --silent				no prints
-q or --question							check whether target is upto date
-i or --ignore-errors		ignore all errors in the command
```
- What are the most important directives to learn about in Makefile?
```
include <file> tells make to read another makefile before continuing
.PHONY	tells make to not treat the target as a file, useful for, for example, clean or other commands which should be executed everytime they're called instead of first checking if a certain file exists
feq, ifneq, ifdef, ifndef, else, endif are similar to C++'s preprocessor directives
```
- What are the most important commands to implement in your Makefile?
```
all, compile everything
component specific compile commands
clean, remove all built files
install, which will copy the compiled result to something like /bin/ 
test, it's funcntion varies
```
- Which ones are essential, which ones are nice to haves?
```
all, clean, component wise compile are essential
install and test are nice to have
```

## Learn Basics of Git

- Read through the code in `src/`
    - Done!
- Answer any `#Questions` as a comment
    - Done!
- Commit and push your changes to git
    - Done!
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

## Introduction to Sockets

- Read the code in `src/tcp-echo-client.cc` and add a way to change the 
  message sent using command line arguments
  - Done!
- **Example**: `./client "hello message from the command prompt"` should send
  `"hello message from the command prompt"` to the server
- Commit your changes into git
  - Done!
- What do all these headers do?
```
#include <arpa/inet.h>      allows inet_pton and inet_ntop to convert string to/from network address
#include <iostream>         brings stdin stdout stderr etc
#include <netinet/in.h> brings sockaddr_in
#include <string> for std::string
#include <sys/socket.h> bind, socket, connect
#include <sys/types.h> datatype
#include <unistd.h> read/write/close syscalls
```
- How do you find out which part of the below code comes from which header?
  - Ctrl+Click when using cpp-tools extension. I guess without something like that you'd have to run grep in all the included Header files
- How do you change the code so that you are sending messages to servers
  other than localhost?
   - Mention their IP address instead of `127.0.0.1`
- How do you change the code to send to a IPv6 address instead of IPv4?
  - Address family `AF_INET6` instead of the current `AF_INET`. Then we can also use `sockaddr_in6` and `sin6_addr`,`sin6_port` etc 
- **Bonus**: How do you change the client code to connect by hostname instead
  of IP address?
   - `getaddrinfo()` and `getnameinfo()` convert hostname to IP and IP to hostname respectively. We can use that to convert the hostname into an IP and then connect using that.
  
## Introduction to Memory Management
- What is happening in line 26 of `tcp-echo-client.cc`? 
  `if (inet_pton(AF_INET, kServerAddress.c_str(), &address.sin_addr) <= 0) {`
   - pton (presentation to numeric) function is converting the ip address of the server to a numeric format. if it fails and returns -1, we can trigger an exit
- What is happening in line 31 of `tcp-echo-client.cc`?
  `if (connect(my_sock, (sockaddr *)&address, sizeof(address)) < 0) {`
  - connects to the server using sys/socket. if it fails  it returns -1
- What is the difference between a pointer and a reference?

|Pointer|Reference|
|-|-|
|Contains the memory address|Just an alias|
|Declared with `*`| Declared with `&`|
|Can be declared without initialisation|Must be initialised|
|Allowed to be NULL or Nullptr|Must point to a valid object|
|Pointer can be used like an int, eg, `ptr++`|Reference isn't like that|
|Pointer to a Pointer is valid| Ref to a ref not valid|


- When is it better to use a pointer?
  - when you need to access the memory directly, eg, to iterate. or when you need to reassign it later. also when the target is allowed to be null.
- When is it better to use a reference?
  - when you want to alias something to pass in functions for it to be able to control it.
- What is the difference between `std::string` and a C-style string?
  - `std::string` is a class and the `std::string` objects have dynamic sizes and automatic bound checking.
  - C-style string is a null terminated array of characters.
- What type is a C-style string?
  - `char[]`
- What happens when you iterate a pointer?
  - It moves to the next memory location. This usually isnt the next byte but rather jumps as many bytes as is the size of the object its pointing to.`address(p+k)=address(p)+k×sizeof(T)`
- What are the most important safety tips to know when using pointers?
  - Always initialise a pointer right when declaring otherwise it can point to random locations (dangling behavious) and lead to unpredictable behaviour.
  - never dereference `nullptr`. implement checks if the code allows `nullptr` to reach locations where it may be dereferenced.
  - Don't iterate beyond bounds.

## Learn Basics of Creating a C++ Project in Your IDE

- How do you compile and run your project in your IDE?
  - From the terminal as usual:

```sh
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1/src$ g++ tcp_echo_client.cc 
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1/src$ g++ tcp_echo_server.cc
# then chmod if needed and execute the binary
```

```sh
# better approach with make
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1$ make
mkdir -p build
g++ src/tcp_echo_server.cc -o build/server.o
mkdir -p build
g++ src/tcp_echo_client.cc -o build/client.o
user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-1$ ./build/server.o 
Server listening on port 8080
```

## Improving Interactions with LLMs

- What is the most authoritative source of information about `socket()` from `<sys/socket.h>`?
   - The POSIX Standard
- What is the most authoritative source of information about the TCP and IP protocols?
  - The Request for Comments Editor (RFC) of Internet Engineering Task Force (IETF)
- What is the most authoritative source of information about the C++
  programming language?
   - The International Organization for Standardization (ISO)
- What information can you find about using Markdown when structuring prompts to LLMs?
   - LLM training data contains a lot of markdown and so they've learned to use it effectively. One can mark section headings and subheadings with # or ## and put code or literals in backticks to strcutre the prompt better. can also try bolding emphatic parts with `**` 

- What is the difference between LLM and AI?
  - AI refers to all forms of systems to have computer make decisions (or content) but LLM is a primarily text based generative AI model tuned to converse in natural language.
- Is it grammatically correct in English to say "a LLM" or "an LLM"? Why?
  - "an" LLM because it starts with an "el" sound