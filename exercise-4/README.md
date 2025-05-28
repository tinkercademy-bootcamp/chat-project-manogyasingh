# Exercise 4

**Update this README with your answers to the questions below.**

## Learning Multi-File C++ Projects

- Move code that can be shared between `tcp_echo_server.cc` and 
  `tcp_echo_client.cc` to separate `.h` and `.cc` files
  - Done!
- How would you compile from the command line?
  - use `-c` flag to make the .o file for utils
  - use `-o` and include the built object to get the final executable
  - overall,
  ```
  user@ip-172-31-25-35:~/chat-project-manogyasingh/exercise-4$ make
mkdir -p build
g++ -g -Wall -Wextra -fsanitize=address -O0 -c src/common_utils.cc -o build/common_utils.o
g++ -g -Wall -Wextra -fsanitize=address -O0 src/tcp_echo_server.cpp build/common_utils.o -o build/server -fsanitize=address
g++ -g -Wall -Wextra -fsanitize=address -O0 src/tcp_echo_client.cpp build/common_utils.o -o build/
  ```
- How would you compile using make?
  - use `$^` to include all dependencies in the make commands
  - add a target for the utils file
  - make sure utils is a prereq of client and server
- How would you compile using VS Code?
  - TODO

### Compiling vs Linking

- What is the difference between compiling and linking in C++?
- What is the difference between creating an executable and creating a 
  library?
- How do you compile a library, and then use that library to compile an
  executable?

### Reminder 
[Quickstart tutorial to make](https://makefiletutorial.com/) - Learn make 
fundamentals with practical examples and common patterns.

## Experiments in Performance

- Is it better to have many small files spread across many directories, or a few large files across fewer directories?
- Is there a difference in compilation time between the two?
- How can you assess performance other than compilation speed?