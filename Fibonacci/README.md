# Fibonacci Number Computation

A cross-platform C++ project that implements and benchmarks four different algorithms for computing the *n*-th Fibonacci number, demonstrating the trade-offs between time complexity and implementation practicality.

## Overview

The program computes Fibonacci numbers after the definition *F*(0) = 0, *F*(1) = 1, *F*(n) = *F*(n-1) + *F*(n-2) for n ≥ 2, and measures the execution time of each algorithm to compare their actual performance against their theoretical complexity.

## Project Structure

```
Fibonacci/
├── README.md
├── CMakeLists.txt              # Cross-platform build
└── src/
    ├── Fibonacci.h             # Declarations & Matrix class
    ├── Fibonacci.cpp           # Algorithm implementations & timing test
    └── main.cpp                # Main entry: reads n from stdin and runs the test
```

## Algorithms

| Algorithm | Approach | Time Complexity | Notes |
|-----------|----------|-----------------|-------|
| `Naive_Recursive` | Direct recursion *F*(n) = *F*(n-1) + *F*(n-2) | Θ(2ⁿ) | Exponential; skipped for n > 43 to avoid excessive runtime |
| `Botton_Up` | Dynamic programming, computes all values from bottom up | Θ(n) | Simple and always correct |
| `Binet_Recursive_Squaring` | Binet's formula *Fₙ* = φⁿ/√5 with divide-and-conquer exponentiation | Θ(lg n) | Fastest, but suffers from floating-point precision loss (incorrect for n ≥ 96) |
| `Matrix_Recursive_Squaring` | Matrix power *Aⁿ* where *A* = [[1,1],[1,0]], via divide-and-conquer | Θ(lg n) | Exact integer results, slightly slower than Binet due to matrix multiplication overhead |

The expected performance ranking is:

```
Binet_Recursive_Squaring < Matrix_Recursive_Squaring < Botton_Up < Naive_Recursive
```

## Building

The project uses [CMake](https://cmake.org/) as the only build system, which works natively on Windows, macOS, and Linux:

```bash
cd Fibonacci
cmake -S . -B build
cmake --build build
```

The executable is then located at `build/fibonacci` (or `build\Release\fibonacci.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running & Testing

```
./fibonacci        # or ./build/fibonacci
```

Then input a positive integer *n* when prompted:

```
input scale n: 43
Fibonacci Test:43
--------------------------------------------
Naive_Recursive: 
Answer: 433494437
Time cost: 3755.65ms
--------------------------------------------
Botton_Up: 
Answer: 433494437
Time cost: 0.0222ms
--------------------------------------------
Binet_Recursive_Squaring: 
Answer: 433494437
Time cost: 0.0002ms
--------------------------------------------
Matrix_Recursive_Squaring: 
Answer: 433494437
Time cost: 0.0047ms
--------------------------------------------
```

### Test Input Constraints

| Algorithm | Valid Range | Behavior Beyond Range |
|-----------|-------------|------------------------|
| `Naive_Recursive` | n ≤ 43 | Skipped with a "Too large scale" message |
| `Botton_Up` | Any valid n | Runs normally |
| `Binet_Recursive_Squaring` | n < 95 | Skipped with an "Out of computation Range" message |
| `Matrix_Recursive_Squaring` | Any valid n | Runs normally (subject to 64-bit overflow for very large n) |

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).