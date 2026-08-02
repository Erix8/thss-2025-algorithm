# Parallel MergeSort & QuickSort with OpenMP

A cross-platform C++ project that implements and benchmarks serial and parallel (OpenMP) versions of MergeSort and QuickSort, demonstrating the performance benefits of parallel computation in divide-and-conquer sorting algorithms.

## Overview

This project provides:
- **Serial MergeSort** — classic O(n log n) recursive implementation
- **Serial QuickSort** — recursive implementation with median-of-three pivot and insertion sort for small subarrays
- **Parallel MergeSort** — OpenMP-based implementation of P-MERGESORT, following the algorithm described in *Parallel Algorithms* (Chapter 26). Uses P-MERGE with FIND-SPLIT-POINT binary search to achieve O(lg² n) span and Θ(n/lg² n) parallelism.
- **Parallel QuickSort** — OpenMP-based task-parallel implementation that spawns recursive partitioning subtrees as independent OpenMP tasks.

The program compares execution times across various input sizes and reports speedup ratios.

## Project Structure

```
ParallelSorting/
├── README.md
├── CMakeLists.txt                     # Cross-platform build (requires OpenMP)
└── src/
    ├── ParallelSort.h                 # Algorithm declarations
    ├── ParallelSort.cpp               # Implementations & timing tests
    └── main.cpp                       # Entry point
```

## Algorithms

### MergeSort

| Variant | Work (T₁) | Span (T∞) | Parallelism (T₁/T∞) |
|---------|-----------|-----------|---------------------|
| Serial  | Θ(n log n)| Θ(n log n)| 1                   |
| Parallel| Θ(n log n)| Θ(lg³ n)  | Θ(n / lg² n)        |

**Parallel MergeSort** (P-MERGESORT):
1. Recursively spawn two parallel tasks for the left and right halves
2. Use `#pragma omp taskwait` to synchronize
3. Merge with **P-MERGE**: a parallel merge that uses `FIND-SPLIT-POINT` (binary search) to split the merge work, achieving O(lg² n) span for the merge step

### QuickSort

| Variant | Work (T₁) | Span (T∞) | Parallelism (T₁/T∞) |
|---------|-----------|-----------|---------------------|
| Serial  | O(n²) worst, O(n log n) avg | O(n²) worst | 1 |
| Parallel| O(n²) worst, O(n log n) avg | O(n) expected | O(log n) expected |

**Parallel QuickSort**:
1. Partition the array (serial, O(n))
2. Spawn two OpenMP tasks for the left and right subarrays
3. Synchronize with `taskwait`
4. Uses median-of-three pivot and switches to insertion sort for subarrays ≤ 16 elements
5. Falls back to serial QuickSort for subarrays ≤ 4096 to avoid task creation overhead

## Building

The project uses [CMake](https://cmake.org/) and requires **OpenMP**.

### Prerequisites

| Platform | Compiler | How to get OpenMP |
|----------|----------|--------------------|
| **macOS** | Apple Clang | `brew install libomp` |
| **Linux** | GCC | `sudo apt install libomp-dev` (Ubuntu) / `sudo dnf install libomp-devel` (Fedora) |
| **Windows** | MSVC (Visual Studio) | Built-in |

### Build

```bash
cd ParallelSorting
cmake -S . -B build
cmake --build build --config Release
```

The executable is located at `build/parallel_sorting` (or `build\Release\parallel_sorting.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running

```
./build/parallel_sorting
```

**Example output (Apple M-series, 10 threads)**:

```
========================================================
  Serial vs Parallel MergeSort & QuickSort Comparison
========================================================

Size          SerialMerge(ms)   ParaMerge(ms)     Speedup(M)        SerialQuick(ms)   ParaQuick(ms)     Speedup(Q)
--------------------------------------------------------------------------------------------------------------------------
10000         4.1               2.7               1.53x             1.7               3.7               0.45x
50000         15.8              4.3               3.71x             6.2               1.9               3.28x
100000        25.0              10.0              2.50x             10.8              3.5               3.05x
500000        123.0             42.9              2.87x             71.8              14.8              4.84x
1000000       276.2             85.2              3.24x             126.8             32.6              3.89x
5000000       1527.4            527.9             2.89x             708.8             154.5             4.59x

========================================================
       Large-Scale Parallel Sorting Comparison
========================================================

Size            ParaMergeSort(ms)  ParaQuickSort(ms)
--------------------------------------------------------
100000          10                 5
500000          43                 17
1000000         83                 27
5000000         533                158
10000000        1296               343
20000000        2314               716
```

## Results Analysis

### Serial vs Parallel Comparison

- For very small inputs (n = 10,000), parallel overhead (task creation/synchronization) dominates for QuickSort, resulting in a slowdown (0.45x). MergeSort still achieves 1.53x speedup due to the parallel merge.
- Starting from n = 50,000, both parallel algorithms show clear speedup over their serial counterparts (3-5x).
- Parallel QuickSort achieves the highest speedup of 4.84x at n = 500,000.
- Parallel MergeSort maintains a steady ~2.5-3.7x speedup across sizes.

### MergeSort vs QuickSort

- **Serial**: QuickSort is approximately 2-3x faster than MergeSort in serial execution (708.8ms vs 1527.4ms at n=5M) due to better cache locality (in-place sorting, no temporary array allocation).
- **Parallel**: QuickSort retains its edge (154.5ms vs 527.9ms at n=5M), running about 3.4x faster than parallel MergeSort.
- At the largest scale (n = 20M): parallel QuickSort finishes in 716ms while parallel MergeSort takes 2314ms.

### Scalability

- Speedup generally increases with input size as the ratio of useful work to overhead improves.
- The parallel slackness (T₁ / (P · T∞)) increases with n, allowing the greedy scheduler to achieve closer to linear speedup (consistent with Corollary 26.3 from the PDF).
- QuickSort benefits more from parallelism due to its lower span: expected O(n) span vs MergeSort's Θ(lg³ n) span.

## Key Implementation Details

### P-MERGE (Parallel Merge)
From the lecture: the P-MERGE-AUX function uses `FIND-SPLIT-POINT` to binary-search for the split in the second subarray, then recurses in parallel on the left and right portions. This yields:
- Span: T∞(n) = T∞(3n/4) + O(lg n) = O(lg² n)
- Parallelism: Θ(n / lg² n)

### Task Cutoff Strategy
Both parallel algorithms switch to serial sorting when subarray size ≤ 4096. This avoids excessive task creation overhead for small subproblems. Similarly, P-MERGE-AUX switches to serial merge when subproblem size ≤ 500.

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), providing nanosecond-level resolution. Results are reported in milliseconds (ms).