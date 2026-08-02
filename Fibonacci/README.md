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
Time cost: 688.288ms
--------------------------------------------
Botton_Up:
Answer: 433494437
Time cost: 0.00075ms
--------------------------------------------
Binet_Recursive_Squaring:
Answer: 433494437
Time cost: 0.004333ms
--------------------------------------------
Matrix_Recursive_Squaring:
Answer: 433494437
Time cost: 0.000042ms
--------------------------------------------
```

## Benchmark Results (Apple M-series)

Times in ms:

| n  | Naive_Recursive | Botton_Up | Binet_Recursive_Squaring | Matrix_Recursive_Squaring |
|----|-----------------|-----------|--------------------------|---------------------------|
| 30 | 3.88            | 0.00100   | 0.02092                  | 0.00013                   |
| 35 | 29.17           | 0.00088   | 0.00667                  | 0.00008                   |
| 40 | 167.70          | 0.00063   | 0.00350                  | 0.00004                   |
| 43 | 688.29          | 0.00075   | 0.00433                  | 0.00004                   |
| 45 | /               | 0.00025   | 0.00088                  | 0.00004                   |
| 90 | /               | 0.00046   | 0.00125                  | 0.00008                   |
| 92 | /               | 0.00029   | 0.00071                  | 0.00008                   |

### Analysis

- **Naive_Recursive**: Doubles with each increment of n, consistent with Θ(φⁿ). At n=43 it takes 688ms; by n=45 it's already too slow and skipped.
- **Botton_Up**: Linear Θ(n), extremely fast for practical n.
- **Binet_Recursive_Squaring**: Θ(lg n), sub-microsecond for large n. However, precision loss is visible at n=90 (answer 2880067194370809856 vs correct 2880067194370816120) and worsens at n=92.
- **Matrix_Recursive_Squaring**: Θ(lg n) with exact integer results—the best choice when both speed and accuracy matter. At ~0.00004-0.00008ms it's effectively instant.

### Test Input Constraints

| Algorithm | Valid Range | Behavior Beyond Range |
|-----------|-------------|------------------------|
| `Naive_Recursive` | n ≤ 43 | Skipped with a "Too large scale" message |
| `Botton_Up` | Any valid n | Runs normally |
| `Binet_Recursive_Squaring` | n < 95 | Skipped with an "Out of computation Range" message |
| `Matrix_Recursive_Squaring` | Any valid n | Runs normally (subject to 64-bit overflow for very large n) |

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).