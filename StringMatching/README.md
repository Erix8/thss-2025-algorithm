# String Matching Algorithms

A cross-platform C++ project that implements and benchmarks three classic string matching algorithms, demonstrating the trade-offs between different pattern matching strategies.

## Overview

The program reads a text string and a pattern string from files, then runs three algorithms to find all occurrences of the pattern in the text, measuring the execution time of each algorithm to compare their actual performance against their theoretical complexity.

## Project Structure

```
StringMatching/
├── README.md
├── CMakeLists.txt                       # Cross-platform build
└── src/
    ├── StringMatching.h                 # Algorithm declarations
    ├── StringMatching.cpp               # Algorithm implementations & timing test
    └── main.cpp                         # Main entry: reads text/pattern files and runs the test
```

## Algorithms

| Algorithm | Approach | Preprocessing Time | Matching Time | Notes |
|-----------|----------|--------------------|---------------|-------|
| `BruteForce` | Scan every position, compare character by character | None | O(nm) | Simplest; no preprocessing |
| `KMP` | Build LPS (Longest Proper Prefix which is also Suffix) array to skip redundant comparisons | O(m) | O(n) | Linear time; excellent for repetitive patterns |
| `Boyer-Moore` | Bad Character + Good Suffix rules to achieve large jumps | O(m + σ) | O(n/m) average, O(nm) worst | Fastest on average for large alphabets |

## Building

The project uses [CMake](https://cmake.org/) as the only build system, which works natively on Windows, macOS, and Linux:

```bash
cd StringMatching
cmake -S . -B build
cmake --build build
```

The executable is then located at `build/string_matching` (or `build\Release\string_matching.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running & Testing

Place the text file and pattern file in the same directory as the executable (or provide absolute paths when prompted):

```
./string_matching        # or ./build/string_matching
```

Then input the text file name and pattern file name when prompted:

```
Enter the text file name (e.g., text.txt): text.txt
Enter the pattern file name (e.g., pattern.txt): pattern.txt
-------------------------------------
Matching method: BruteForce
Answer: 0 5 12
Time cost: 0.0123ms
-------------------------------------
Matching method: KMP
Answer: 0 5 12
Time cost: 0.0056ms
-------------------------------------
Matching method: Boyer-Moore
Answer: 0 5 12
Time cost: 0.0034ms
-------------------------------------
```

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).