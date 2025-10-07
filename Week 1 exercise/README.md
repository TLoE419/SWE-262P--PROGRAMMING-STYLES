# Term Frequency Program

This program calculates the term frequency of words in a given text file, excluding common stop words.

## Prerequisites

- C++ compiler (g++ or similar)

## Input Files

- `stop_words.txt`: List of common stop words. Place this file in the parent directory of this program.
- `pride-and-prejudice.txt`: Sample text file to process. Place this file in the parent directory of this program.

## Compilation and Execution

Navigate to the `week1` directory and execute the following commands:

```bash
g++ -o Main Main.cpp
```

```bash
./Main ../pride-and-prejudice.txt
```

Ensure that the `stop_words.txt` file is located in the parent directory (`../stop_words.txt`).

## Expected Output

The program will output the 25 most frequent words in the text file, along with their frequencies, in the format:

```
word  -  frequency
```

## File Structure

```
root/
├── stop_words.txt
├── pride-and-prejudice.txt
└── week1/
    ├── Main.cpp
    ├── Main (executable)
    └── README.md
```

