# SWE 262P - Programming Styles

This repository contains programming exercises for SWE 262P Programming Styles course.

## Week 1 Exercise - Term Frequency Program

This program calculates the term frequency of words in a given text file, excluding common stop words.

### Prerequisites

- C++ compiler (g++ or similar)

### Input Files

- `stop_words.txt`: List of common stop words. Located in the root directory.
- `pride-and-prejudice.txt`: Sample text file to process. Located in the root directory.

### Compilation and Execution

Navigate to the `Week 1 exercise` directory and execute the following commands:

```bash
cd "Week 1 exercise"
```

```bash
g++ -o Main Main.cpp
```

```bash
./Main ../pride-and-prejudice.txt
```

### Expected Output

The program will output the 25 most frequent words in the text file, along with their frequencies, in the format:

```
word  -  frequency
```

### File Structure

```
SWE 262P- PROGRAMMING STYLES/
├── README.md
├── stop_words.txt
├── pride-and-prejudice.txt
└── Week 1 exercise/
    ├── Main.cpp
    └── Main (executable)
```

---

## Week 2 Exercise - Programming Styles

This week explores different programming styles for the term frequency problem.

### Programs

1. **Four.cpp** - Procedural style
2. **Five.cpp** - Pipeline/Functional style
3. **Main.java** - Object-oriented style

### Compilation and Execution

#### C++ Programs (Four.cpp, Five.cpp)

```bash
cd "Week 2 exercise"

# Compile Four.cpp
g++ -o Four Four.cpp
./Four ../pride-and-prejudice.txt

# Compile Five.cpp
g++ -o Five Five.cpp
./Five ../pride-and-prejudice.txt
```

#### Java Program (Main.java)

```bash
cd "Week 2 exercise"

# Compile
javac Main.java

# Run
java Main ../pride-and-prejudice.txt
```

---

## Week 3 Exercise - Recursion Style

This week implements the term frequency program using recursive functions.

### Programs

1. **eight.cpp** - Recursive implementation with batch processing
2. **seven.sh** - Shell script version

### Compilation and Execution

#### C++ Program (eight.cpp)

```bash
cd "Week 3 exercise"

# Compile
g++ -o eight eight.cpp -std=c++11

# Run
./eight ../pride-and-prejudice.txt
```

#### Shell Script (seven.sh)

```bash
cd "Week 3 exercise"

# Make executable
chmod +x seven.sh

# Run
./seven.sh ../pride-and-prejudice.txt
```

---

### File Structure

```
SWE 262P- PROGRAMMING STYLES/
├── README.md
├── stop_words.txt
├── pride-and-prejudice.txt
├── Week 1 exercise/
│   ├── Main.cpp
│   └── Main (executable)
├── Week 2 exercise/
│   ├── Four.cpp
│   ├── Five.cpp
│   ├── Main.java
│   └── executables
└── Week 3 exercise/
    ├── eight.cpp
    ├── seven.sh
    └── eight (executable)
```

