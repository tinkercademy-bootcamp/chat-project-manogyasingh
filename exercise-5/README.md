# Exercise 5

**Update this README with your answers to the questions below.**

## Comparing File Changes

- What's an easy way to identify the changes in source code from one exercise
  to another?
  - I use a tool called `meld` which has a nice GUI and is built on GTK3 (for context I use GNOME)
  - from the CLI you can do `diff -ruwN exercise-4/src exercise-5/src` or install a tool `colordiff` then call `colordiff -ruwN exercise-4/src exercise-5/src`    
  - `colordiff` is a fork of `diff` with color and is therefore easier to read
  - the flags:
    - r = recursive
    - u = unified context (more readable)
    - w = ignore whitespaces
    - N = treat new files as previously existing but empty
## Using Classes

- Here is one way to separate out the code into multiple files
- Is this the best way to do it? 
  - Depends on the situation. Here, given that the methods seperated into namespaces are related to each other, we could have used a class too.
- What are the advantages and disadvantages?
  - Advantages of namespaces
    - can organise code
    - frees you from worrying about same variables names being accidentally reused
    - modularity = reusability, easier testing and maintainability
    - Advantage over classes: is open and extensible. more natural for unrelated functions and constants because they don't need inheritance or encapsulation
  - Disadvantages
    - can make the code look complicated at a first glance
    - Disadvantages over classes:
      - no access control unlike classes
      - no inheritance or encapsulation


## Introduction to Namespace

- There are different ways namespace is being used in this exercise
- Which way is better? What are the advantages and disadvantages?

## Abstracting Code into Classes

- Abstract the client and server logic into a `Client` and `Server` class
- **Note**: You don't have to use the code in this exercise as a starting point
- You can use the code you wrote from previous exercises instead
- How should you divide the code into files?
- What namespace and directory structure should you use? Why?

## Programming Sense of Taste

- How do you judge what makes a particular choice of namespace and directory
  structure? 
- How do you judge what makes a good naming convention or programming style?

## "Senses" in Programming

- Have you ever heard of programmers referring to "code smells"? How can code
  smell?
- What does it mean to have a sense of taste in programming? How can code
  taste?
- Is there an analogue for every sense?
- What other code senses can you think of?
- How many senses do humans have?
- When would you want to see something before you touch it?
- When would you want to touch something before you see it?