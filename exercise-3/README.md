# Exercise 3

**Update this README with your answers to the questions below.**

## Re-using Code

- Read the code in `src/tcp_echo_client.cc`
- A new function `check_error()` has been created and `create_socket()` from 
  exercise-2 has been refactored to make use of it
- What are the benefits of writing code in this way?
  - Follows the DRY principle. This makes it more modular, compact, readable and maintainable.
- Are there any costs to writing code like this?
  - A very small (negligible) overhead from an additional function call in the stack
  - Can be time consuming to make all functions compatible to it
- Apply `check_error` to all the code in `src/`
  - Done!
  - I did not apply it to `send_and_receive_messages()` in `tcp_echo_client.cc` and to `handle_accept()` in `tcp_echo_server.cc`. Rationale is given in the code.

## Introduction to Compiler Explorer

- Try out the old `create_socket()` and the new `check_error()` and 
  `create_socket()` in [Compiler Explorer](https://godbolt.org) - Interactive 
  tool for exploring how C++ code compiles to assembly
- What is happening here?
  - Found some interesting observations here
  - originally, the modularised version seemed to be about 4 times longer
  - i noticed much of the assembly was just std::string constructors, so i modified the function to use const char* (C_Style strings) and now its just twice as long
  - of course, this is still not ideal and we see extra function calls causing performance overhead
- Can you think of any different approaches to this problem?
  - i think the substitution by `const char*` helped
  - for an even better approach i think we can preprocessor directives to write this logic in every function at compile time. this would cause no such performance overhead of repeated calls because to the compiler it will be indistinguishable from the approach which does not use multiple functions. for us, it will still be modular in a sense as the code itself is not repeated.
- How can you modify your Makefile to generate assembly code instead of
  compiled code?
  - we can use `-S` flag for this
- **Note**: You can save the generated assembly from Compiler Explorer
- **Bonus**: Can you view assembly code using your IDE?
- **Bonus**: How do you see the assembly when you step through each line in

  debugging mode from your IDE?
- [x86 assembly reference](http://ref.x86asm.net/) - Comprehensive reference 
  for x86 assembly language instructions and syntax

## More About Memory Management

- Make sure you have `-fsanitize=address` in both your `CXX_FLAGS` and 
  `LD_FLAGS` in your Makefile
  - Added
- What do `-fsanitize=address`, `CXX_FLAGS` and `LD_FLAGS` mean?
  - `CXX_FLAGS` are flags to the compiler
  - `LD_FLAGS` are flags to the linker
  - `-fsanatize=address` is a flag that can (and should) be passed as both a compiler and a linker flag. It is a runtime memory error detector. It can detect overflows, underflows, using a variable after it has been freed, double freeing, memory leaks and a few more different types of memory errors.
- With the new tool of the Compiler Explorer, and keeping in mind what you 
  have learned about how to use debug mode
- What happens when you look at a `std::string` using the above methods?
  - It has 32 bytes of space. the first 8 are a pointer. the next 8 mention the string length. after this the next 14 bytes contained the actual string followed by two nullbytes
- Where is the text in your `std::string`?
  - after the pointer and the size
  - in longer strings, without short strings optimisations, the memory will instead be allocated on the heap and the pointer will tell where it is
```
-exec x/32xb &message
0xfffff52090a0:	0xb0	0x90	0x20	0xf5	0xff	0xff	0x00	0x00 // data pointer to  start of string in big endian
0xfffff52090a8:	0x0e	0x00	0x00	0x00	0x00	0x00	0x00	0x00 // size
0xfffff52090b0:	0x73	0x61	0x6d	0x70	0x6c	0x65	0x5f	0x6d // text
0xfffff52090b8:	0x65	0x73	0x73	0x61	0x67	0x65	0x00	0x00 // remaining text and null bytes
```

- What is `std::optional`?
  - It represents objects that may not or may contain values of a certain (predefined) type 
- How do you find out the memory layout of a `std::optional`?
  - Inspecting it in the debugger
  - It has a boolean flag to indicate whether or not there is a value.
 and then thre is the value itself
  - In total it takes slightly more space than a normal object of that type
- Read https://en.cppreference.com/w/cpp/memory#Smart_pointers - Guide to 
  modern C++ memory management using smart pointers
- Which pointer types are the most important to know about?
  - `std::unique_ptr` : represents exclusive ownership of a dynamically allocated object. object is automatically deallocated when the `unique_ptr` goes out of scope
  - `std::shared_ptr` : represents shared ownership. the object is deallocated after all shared pointers are out of scope.
  - `std::weak_ptr`:: doesnt own the object
- Which smart pointer should you use by default if you can?
  - `std::unique_ptr` is the best by default.
- Does changing your optimization level in `CXXFLAGS` from `-O0` to `-O3` have
  any impact on the answers to any of the above questions?
  - `-O3` actually causes the compiled assembly to drop unused methods of `std::string`. at `-O0` I can see the constructor, destructor, allocator and even the error handler of the string.

## More Thinking About Performance

- After your experiments with Compiler Explorer, do you have any updates for
  your answers in exercise-2?
  - I now think that there's a significant overhead associated with repeated function calling with modular code. We need to find better ways to optimise it.

### Bonus: Do Not Watch Now 

- [More about Compiler Explorer](https://www.youtube.com/watch?v=bSkpMdDe4g4) - 
  Deep dive into compiler optimization and assembly analysis
  - Create a struct which contain many different data types
  - Look at the memory layout in the debugger
  - Create a `char` or `uint8_t` pointer to the beginning of the struct, 
    iterate to the end of the struct, printing out the value of each byte in 
    hexadecimal
  - Try accessing different parts of the struct and see in compiler explorer
    what the assembly looks like
  - What happens if you iterate the pointer to outside the bounds of your
    array?
  - Let's say your struct is called `Foo`
  - What is the difference between `std::vector<Foo>` and `std::vector<Foo*>`?
  - What are the tradeoffs between using `std::vector<Foo>` vs 
    `std::vector<Foo*>`? 
  - Give an example where `std::vector<Foo>` is a better choice than 
    `std::vector<Foo*>`
  - Give another example where the opposite is true
  - Can you create `std::vector<Foo&>`? 
  - Can you create `std::vector<std::optional<Foo>>`?
  - What happens if your struct contains another struct?
  - What is the difference between a struct and a class?