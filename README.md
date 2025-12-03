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

## Week 5 Exercise - Reflection and Introspection

This week implements a JAR file analyzer using Java Reflection API.

### Program

**JarClasses.java** - Analyzes classes in a JAR file and reports method/field statistics using reflection

### Features

The program inspects each class in a JAR file and displays:
- Number of public methods
- Number of private methods
- Number of protected methods
- Number of static methods
- Number of fields

### Compilation and Execution

```bash
cd "Week 5 exercise"

# Compile
javac JarClasses.java

# Run (using the included json-java.jar as example)
java JarClasses json-java.jar
```

## Week 6 Exercise - Concurrent Programming Styles

This week implements the term frequency program using concurrent programming patterns with multiple threads.

### Programs

1. **29.1.cpp** - Actor Model (Active Letterbox Style)
2. **30.1.cpp** - Data Spaces (Shared Queue with Worker Threads)
3. **32.1.cpp** - Map-Reduce Pattern

### Style #29: Actors (29.1.cpp)

**Compilation and Execution:**

```bash
cd "Week 6 exercise"
```

```bash
g++ -std=c++11 -pthread 29.1.cpp -o 29.1
```

```bash
./29.1 ../pride-and-prejudice.txt
```

### Style #30: Data Spaces (30.1.cpp)

**Compilation and Execution:**

```bash
cd "Week 6 exercise"
```

```bash
g++ -std=c++11 -pthread 30.1.cpp -o 30.1
```

```bash
./30.1 ../pride-and-prejudice.txt
```

### Style #32: Map-Reduce (32.1.cpp)

**Compilation and Execution:**

```bash
cd "Week 6 exercise"
```

```bash
g++ -std=c++11 32.1.cpp -o 32.1
```

```bash
./32.1 ../pride-and-prejudice.txt
```

---

## Week 8 Exercise - Plugin System (Style #20)

### Program

**twenty.java** - Main program that loads plugins dynamically based on configuration

### Plugins

**Word Extractors:**
1. `extractor_word_1` - Extracts all non-stop words (normal extraction)
2. `extractor_word_2` - Extracts only non-stop words containing the letter 'z'

**Word Counters:**
1. `counter_word_1` - Counts word frequencies and returns top 25 (normal counting)
2. `counter_word_2` - Counts words by their first letter (a, b, c, etc.)

### Configuration

The `config.properties` file controls which plugins are loaded:

```properties
extractor=plugins.extractor_word_1   # or plugins.extractor_word_2
counter=plugins.counter_word_1       # or plugins.counter_word_2
```

### Compilation and Execution

```bash
cd "Week 8 exercise"

# Compile main program
javac twenty.java

# Compile all plugins
javac plugins/*.java

# Run
java twenty ../pride-and-prejudice.txt
```

### Plugin Combinations

You can test all 4 combinations by editing `config.properties`:

1. `extractor_word_1` + `counter_word_1` - Normal extraction + word frequency
2. `extractor_word_1` + `counter_word_2` - Normal extraction + first letter count
3. `extractor_word_2` + `counter_word_1` - Words with 'z' + word frequency
4. `extractor_word_2` + `counter_word_2` - Words with 'z' + first letter count

---

## Week 9 Exercise - Lazy Rivers (Style #28)

This week implements the term frequency program using lazy evaluation patterns that process data on-demand rather than loading everything into memory at once.

### Programs

1. **Iterators.java** - Java implementation with custom Iterator classes
2. **Streams.java** - Java Stream API implementation with lazy evaluation

### Java Iterators (Iterators.java)

Custom Iterator classes that mimic Python generators:
- `CharacterIterator` - Yields characters from file
- `WordIterator` - Extracts words from character stream
- `NonStopWordIterator` - Filters stop words
- `CountAndSortIterator` - Counts and yields sorted results

**Compilation and Execution:**

```bash
cd "Week 9 exercise"

# Compile
javac Iterators.java

# Run
java Iterators ../pride-and-prejudice.txt
```

### Java Streams (Streams.java)

Uses Java 8+ Stream API for functional lazy evaluation:
- `Files.lines()` - Creates lazy stream of lines
- `flatMap()` - Splits lines into words lazily
- `filter()` - Removes stop words and empty strings
- `forEach()` - Terminal operation that triggers processing

**Compilation and Execution:**

```bash
cd "Week 9 exercise"

# Compile
javac Streams.java

# Run
java Streams ../pride-and-prejudice.txt
```

### Output Format

All three implementations produce identical output:
- Prints `-----------------------------` separator
- Shows top 25 most frequent words with counts
- Displays intermediate results every 5000 words processed
- Shows final results at the end

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
├── Week 3 exercise/
│   ├── eight.cpp
│   ├── seven.sh
│   └── eight (executable)
├── Week 5 exercise/
│   ├── JarClasses.java
│   ├── JarClasses.class
│   └── json-java.jar
├── Week 6 exercise/
│   ├── 29.1.cpp
│   ├── 30.1.cpp
│   └── 32.1.cpp
├── Week 8 exercise/
│   ├── twenty.java
│   ├── twenty.class
│   ├── config.properties
│   └── plugins/
│       ├── extractor_word_1.java
│       ├── extractor_word_2.java
│       ├── counter_word_1.java
│       └── counter_word_2.java
└── Week 9 exercise/
    ├── tf-28.py
    ├── Iterators.java
    ├── Streams.java
    ├── README
    └── .class files
```

