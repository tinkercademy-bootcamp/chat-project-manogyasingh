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
  - I can see these:
    - Namespace declaration inside a function (`send_and_receive_message()`) in `client-main.cc`
    - functions inside namespace `tt::chat::server` in server-main.cc
    - namespace aliasing in the `main()` function of `server-main,cc`
- Which way is better? What are the advantages and disadvantages?
  - For namespace inside function:
    - Adv: This won't pollute the rest of the code
    - Disadv: Can't be reused without being rewritten
  - for functions inside namespace:
    - Adv: Very natural heirarchical organisation, reusable code.
    - Disadv: No access control, it will pollute every codespace where its included
  - aliasing a namespace
    - Adv: concise, easier to write multiple times
    - Disadv: no additional disadvantages on its own, except perhaps making it harder to remember what the namespace originally referred to

## Abstracting Code into Classes

- Abstract the client and server logic into a `Client` and `Server` class
- **Note**: You don't have to use the code in this exercise as a starting point
- You can use the code you wrote from previous exercises instead
- How should you divide the code into files?
- What namespace and directory structure should you use? Why?

## Programming Sense of Taste

- How do you judge what makes a particular choice of namespace and directory
  structure? 
  - look for the following attributes
    - Cohesion: "birds of a feather should flock together", in practice this means keeping related methods in the same part of the codebase
    - There should be a sensible dependency tree arising from the organisation. Avoid circular dependencies at all costs
    - There should be a predictable place for modules, this relates to the cohesion point but it extends it to cohesion across levels.
      - for example, `graphics/rendering/opengl/shader.h` makes sense but `utils/graphics/socket.h` doesnt
- How do you judge what makes a good naming convention or programming style?
  - clarity and consistency in naming is super important
  - in the name, describe the intent, that is, what something is intended to do, not what it technically does. eg, use `UserAuth()` instead of `UserCredentialSHA256HashChecker()`. In general, one should describe the intent in name, the why in comments and the how in the code
  - Overall if a codebase is easy to navigate, understand, expand, maintain and explain, both programming style and naming are good


## "Senses" in Programming

- Have you ever heard of programmers referring to "code smells"? How can code
  smell?
  - I think it refers to the programming style being suboptimal (or optimal in case it smells good!)
  - Programming style doesn't include things like performance or bugs, it's about the quality of code in the sense of how readable and maintainable it is
  - It's analogous to smell in the sense that even before you actually read it (see it well enough) you can say something's wrong with it
  - Some principles like SOLID, KISS, YAGNI etc were created as guides on how to keep the programming style optimal
- What does it mean to have a sense of taste in programming? How can code
  taste?
  - I looked it up and apparently there's a whole system of idiomatically describing code attributes
    - readibility tastes like something easy to digest
    - consistency tastes like a balanced meal
    - elegance and inventive solutions taste like exotic meals
    - copy pasted code tastes stale
- Is there an analogue for every sense?
  - yes!
    - code that appeals to vision is analogous to sensibly written and well formatted code
    - smell discussed above
    - taste discussed above
    - hearing is about how the compiler and debugger react to your code, what signals it generates
    - touch is how easy something is to work with, smooth vs rough interfaces. fast/slow function calls
- What other code senses can you think of?
  - intuition
  - the sense of where you are and in what orientation, referring to how well you understand your position in code and its significance
  - sense of time: code can feel dated/ not updated to the latest standards
- How many senses do humans have?
  - Five (vision taste smell hearing touch) are the classic ones
  - orientation, location, intuition, idea of the timeline, temperature are some more (not strictly senses but close enough)
- When would you want to see something before you touch it?
  - when you have a well documented project and you want to know the codebase before working with it

- When would you want to touch something before you see it?
  - when playing with new things where you can probably learn faster by doing than by reading