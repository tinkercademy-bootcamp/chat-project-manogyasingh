# Exercise 6

**Update this README with your answers to the questions below.**

## Adding Third Party Libraries or Source Code to Your Project

- There is a new file `src/spdlog-hello-world-main.cc`
- It uses the spdlog and fmt libraries. 
  [More info here](https://github.com/gabime/spdlog) - Fast C++ logging 
  library with various sink types and formatting options
- How do you compile this file using the `g++` CLI?
  - You'll need to link the libraries
```
g++ src/spdlog-hello-world-main.cc -lspdlog -lfmt -o loghello
```
- What do you need to change in your makefile to use this library?
  - Add the two library linking flags to the makefile
- How many different ways can this library be added into your project?
  - Can copy-paste and include the header folder.
    - Easy and portable but longer compile times
  - Can download compiled version (best option if can't use package manager)
  - Use system package manager (best)
  - System packages, but static
    - faster when running, more portable, longer time to compile
- What are the tradeoffs in the different ways?
  - explained above
- Why are there so many different ways to do it?
  - They're all essentially doing the same thing, ie, linking either statically or dynamically, but the method of downloading is different
  
## Static Linking vs Dynamic Linking

- What are the differences between static linking and dynamic linking?

|Static Linking|Dynamic Linking|
|---|---|
|Code bound at compile time|Code bound at runtime|
|The binary contains everything needed to run it|The binary calls the compiled library|
|Better for making smaller self contained programs|Better for ecosystems that can share some libs|
|Very slightly faster|.|
|.|Less disk usage, especially if ecosystem is large|
|Only need to ship 1 file|Need to ship the libs too|

- What are the tradeoffs?'
  - explained above
- How do you enable static linking or dynamic linking in your makefile?
  - Dynamic is the default
  - Static: add -static to LD flags


## Git Clone and Building from Source

- Where is `g++` looking for the include files and library files?
- How do you find out?

## C++ Package Managers

- Why are there so many C++ package managers?
- Which one is the best one?
- Which are the most notable ways to manage the compilation of C++ projects?