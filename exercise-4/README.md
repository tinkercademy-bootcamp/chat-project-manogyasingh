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
  - compile translates the source code files to object files. linker links them to create the final executable
- What is the difference between creating an executable and creating a 
  library?
    - an executable is a single run-able program that does something
    - a library is a collection of executables and reusable code, its usually not meant to be run by itself but rather be reused in other projects.
- How do you compile a library, and then use that library to compile an
  executable?
  - static library ()`.a` file)
    - compile individual files to object files
    - create static lib `ar rcs <final_file> <objects>`
    - use it later `g++ main.cpp -L. -lnameoflib -o program`
  - dynamic
    - compile as position independent code `g++ -fPIC -c source.cc -o source.o`
    - create a shared lib `g++ -shared source.o -o libname.so`
    - linking is the same
    - `-L` specifies lib search path. `-l` specifies its name

### Reminder 
[Quickstart tutorial to make](https://makefiletutorial.com/) - Learn make 
fundamentals with practical examples and common patterns.

## Experiments in Performance

- Is it better to have many small files spread across many directories, or a few large files across fewer directories?
  - from a performance perspective, from what I learned in the last experiment, there will be an overhead from modularity. thus, few large files are better
  - those can probably be fixed with some clever preprocessor directive tricks, and the overhead may not be that much compared to the number of operations in the actual execution. in this case one would prefer the readibility and maintainability of modularity
- Is there a difference in compilation time between the two?
  - With larger files the whole project seems to build faster.
  - but in practice we usually only change a few components at a time, in this case smaller files will compile faster because make will only recompile those.
- How can you assess performance other than compilation speed?
  - bandwidth and space usage (memory and disk)
  - execuition performance including time and space tests