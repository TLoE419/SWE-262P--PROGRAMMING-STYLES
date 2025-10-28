# Week 4 Exercise - Programming Styles

This directory contains implementations of different programming styles for a word frequency counter program.

## Programs

### Style #9: Continuation-Passing Style (eight.rb)
**Language:** Ruby

Each function takes an additional parameter (the continuation function) which is applied at the end of the current function. The continuation receives what would be the output of the current function.

**Run:**
```bash
ruby eight.rb ../pride-and-prejudice.txt
```

**Key Features:**
- Each function passes its result to the next function as a parameter
- Creates a pipeline where the next function to apply is given as parameter
- Also known as "Kick your teammate forward" pattern

---

### Style #10: The One (Monadic Style) (ten.cpp)
**Language:** C++

A single class wraps a value and provides a `bind()` method for chaining operations.

**Compile & Run:**
```bash
g++ -std=c++17 -o ten ten.cpp
./ten ../pride-and-prejudice.txt
```

**Key Features:**
- `TFTheOne` class wraps any value using `std::any`
- `bind()` method applies functions and returns `*this` for chaining
- Fluent interface for functional pipeline
- Uses C++17's `std::any` for type flexibility

---

### Style #15: Hollywood (Event-Driven/Callback) (fifteen.cpp)
**Language:** C++

"Don't call us, we'll call you" - Framework-based approach with event handlers.

**Compile & Run:**
```bash
g++ -std=c++14 -o fifteen fifteen.cpp
./fifteen ../pride-and-prejudice.txt
```

**Key Features:**
- `WordFrequencyFramework` manages event lifecycle
- Components register handlers for `load`, `dowork`, and `end` events
- Preserves original recursive counting logic with `RECURSION_LIMIT`
- Event-driven architecture with loose coupling
- Uses lambda functions for callbacks

---

## Requirements

- **Ruby**: Ruby 2.x or higher (for eight.rb)
- **C++**:
  - C++17 compiler for ten.cpp (g++ 7+, clang++ 5+)
  - C++14 compiler for fifteen.cpp (g++ 5+, clang++ 3.4+)
