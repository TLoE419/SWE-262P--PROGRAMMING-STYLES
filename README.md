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

