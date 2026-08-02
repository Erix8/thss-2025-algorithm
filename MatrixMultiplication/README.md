# Parallel Matrix Multiplication

A cross-platform C++ project that implements and benchmarks serial and parallel (divide-and-conquer with OpenMP) matrix multiplication algorithms, demonstrating the performance benefits of parallel computation for large-scale linear algebra.

## Overview

The program multiplies two n×n square matrices using a serial triple-loop algorithm and an OpenMP-based parallel divide-and-conquer algorithm, then compares their execution time across different matrix sizes.

## Project Structure

```
MatrixMultiplication/
├── README.md
├── CMakeLists.txt                            # Cross-platform build (requires OpenMP)
└── src/
    ├── MatrixMultiplication.h                # Algorithm declarations
    ├── MatrixMultiplication.cpp              # Implementations & timing tests
    └── main.cpp                              # Entry point
```

## Algorithms

### Serial Matrix Multiplication

Classical triple nested loop, computing `C[i][j] = Σ A[i][k] · B[k][j]` for all `i`, `j`.

- **Time complexity**: O(n³)
- Simple and straightforward; performance degrades significantly as matrix size grows.

### Parallel Matrix Multiplication

Divide-and-conquer approach using OpenMP for multi-threaded parallelism:

1. **Recursive decomposition**: Split each n×n matrix into four n/2 × n/2 submatrices, recursively.
2. **Base case**: Switch to serial triple-loop multiplication when n ≤ 20.
3. **Parallel tasks**: Use OpenMP `task` directives inside a single `parallel` region to spawn 8 sub-problems simultaneously (4 for C blocks, 4 for a temporary D accumulator), distributed across all threads via `taskwait`.
4. **Merge**: After all sub-problems complete, accumulate D into C.

- **Time complexity**: O(n³) — same asymptotic, lower constant via parallelism
- **Parallelism**: O(n³ / lg² n)

## Building

The project uses [CMake](https://cmake.org/) and requires **OpenMP**.

### Prerequisites

| Platform | Compiler | How to get OpenMP |
|----------|----------|--------------------|
| **Windows** | MSVC (Visual Studio) | Built-in, no extra steps. Use `x64 Native Tools Command Prompt` for CMake. |
| **macOS** | Apple Clang | `brew install libomp` |
| **Linux** | GCC | `sudo apt install libomp-dev` (Ubuntu) / `sudo dnf install libomp-devel` (Fedora) |

### Build

```bash
cd MatrixMultiplication
cmake -S . -B build
cmake --build build --config Release
```

The executable is located at `build/matrix_multiplication` (or `build\Release\matrix_multiplication.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running

```
./build/matrix_multiplication
```

**Example output (Apple M-series, 10 threads)**:

```
Comparison test of serial and parallel matrix multiplication...
    Matrix Size                   Serial (ms)                 Parallel (ms)
---------------------------------------------------------------------------
             64                             5                             7
            128                            20                             3
            256                            83                            15
            512                           606                           123
           1024                          5207                          1052

Large-scale parallel matrix multiplication test...
    Matrix Size                 Parallel (ms)
---------------------------------------------
            128                             2
            256                            16
            512                           120
           1024                           977
           2048                          8624
```

## Results Analysis

### Serial vs Parallel Comparison

- For small matrices (n=64), the parallel version is slightly slower due to task creation overhead.
- Starting from n=128, the parallel algorithm outperforms serial significantly (speedup ~6.7×).
- At n=256 and beyond, the parallel version maintains ~5× speedup over serial, demonstrating effective utilization of the 10-core CPU.

### Parallel Scalability

- The parallel algorithm's runtime grows approximately as O(n³), matching the theoretical complexity.
- From n=1024 to n=2048 (4× elements, 64× theoretical operations), actual runtime increases only ~8.8×, demonstrating that parallel computation effectively mitigates the computational burden for very large matrices.

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), providing nanosecond-level resolution. Results are reported in milliseconds (ms).