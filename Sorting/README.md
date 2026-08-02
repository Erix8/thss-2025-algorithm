# Sorting Algorithm Comparison

A cross-platform C++ project that implements and benchmarks five classic sorting algorithms, demonstrating their time complexity differences across various input scales.

## Overview

The program generates a random list of unsigned 32-bit integers and measures the execution time of each sorting algorithm on the same data:

- **Insertion Sort** — Θ(n²) average
- **Shell Sort** — O(n^1.3) to O(n²) depending on gap sequence
- **Quick Sort** — Θ(n lg n) average
- **Merge Sort** — Θ(n lg n)
- **Radix Sort** — O(k(n + r)) ≈ O(n)

## Project Structure

```
Sorting/
├── README.md
├── CMakeLists.txt              # Cross-platform build
└── src/
    ├── Sort.hpp/.cpp           # Five sorting algorithm implementations
    ├── Test.hpp/.cpp           # Data generation & benchmarking
    └── main.cpp                # Entry point
```

## Algorithms

### Insertion Sort — Θ(n²)

Iteratively inserts each element into its correct position in the already-sorted prefix of the array.

- **Complexity**: average O(n²) (≈ n(n-1)/4 comparisons), nearly O(n) for nearly sorted data
- **Stable** — space complexity O(1)
- Best suited for small or nearly sorted datasets

### Shell Sort — O(n^1.3) ~ O(n²)

An improved insertion sort that first compares and sorts elements separated by a gap, then gradually reduces the gap to 1. Each pass makes the array more sorted, so the final insertion sort runs much faster than plain insertion sort.

- **Complexity**: worst case O(n²), best case near O(n), average depends on the gap sequence (typically O(n^1.3) ~ O(n²))
- **Unstable** — space complexity O(1)

### Quick Sort — Θ(n lg n) average

Picks a pivot, partitions the array so all elements smaller than the pivot come before it and all larger ones after it, then recursively sorts the two partitions.

- **Complexity**: worst case O(n²) (unbalanced partitions), average Θ(n lg n)
- **Unstable** — requires extra stack space for recursion
- Uses insertion sort for sub-arrays smaller than 10 elements as an optimization

### Merge Sort — Θ(n lg n)

Recursively splits the array in half until each sub-array has one element, then repeatedly merges adjacent sorted halves into larger sorted arrays.

- **Complexity**: Θ(n lg n) regardless of input order
- **Stable** — space complexity O(n)

### Radix Sort — O(k(n + r)) ≈ O(n)

Sorts by processing digits from least significant to most significant. Uses 256 buckets, extracting 8 bits at a time from each 32-bit integer.

- **Complexity**: each of the 4 passes (8-bit digits) costs O(n + r), total O(k(n + r)) with k = 4, r = 256
- **Stable** — space complexity O(n + r)
- Particularly efficient for integers with a fixed bit width

## Building

The project uses [CMake](https://cmake.org/) as the build system, which works natively on Windows, macOS, and Linux:

```bash
cd Sorting
cmake -S . -B build
cmake --build build
```

The executable is then located at `build/sorting` (or `build\Release\sorting.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running & Testing

```
./build/sorting        # or build\Release\sorting.exe on Windows
```

The program prompts for the input size, then generates a random list of that size and times every algorithm:

```
Size of List: 10000
Data Generating Complete!
-------------------------------------
Sorting method: Insertion Sort
Time cost: 9.69975ms
-------------------------------------
Sorting method: Shell Sort
Time cost: 0.906333ms
-------------------------------------
Sorting method: Quick Sort
Time cost: 0.523ms
-------------------------------------
Sorting method: Merge Sort
Time cost: 0.58125ms
-------------------------------------
Sorting method: Radix Sort
Time cost: 0.38625ms
-------------------------------------
```

> **Note**: Insertion Sort is skipped for input sizes ≥ 10⁶ and Shell Sort for sizes ≥ 10⁸, as their runtime becomes prohibitive at those scales.

## Benchmark Results (Apple M-series)

Random 32-bit unsigned integers, times in ms:

| n            | 10      | 10²     | 10³     | 10⁴     | 10⁵      | 10⁶     | 10⁷      | 10⁸      | 2×10⁸     |
| ------------ | ------- | ------- | ------- | ------- | -------- | ------- | -------- | -------- | --------- |
| Insertion    | 0.0004  | 0.0036  | 0.138   | 9.70    | 654.14   | /       | /        | /        | /         |
| Shell        | 0.0002  | 0.0059  | 0.075   | 0.91    | 7.06     | 87.49   | 1137.05  | /        | /         |
| Quick        | 0.0003  | 0.0052  | 0.050   | 0.52    | 4.07     | 45.80   | 524.80   | 6045.72  | 12875.0   |
| Merge        | 0.0011  | 0.0074  | 0.067   | 0.58    | 4.25     | 44.35   | 532.78   | 6068.89  | 13059.5   |
| Radix        | 0.0113  | 0.0435  | 0.180   | 0.39    | 0.69     | 4.64    | 51.58    | 723.86   | 2291.74   |

### Analysis

- **Insertion Sort**: O(n²) growth is clear. Fast at n=10³ (0.138ms), but jumps to 654ms at n=10⁵. Skipped for n ≥ 10⁶.
- **Shell Sort**: ~10× faster than insertion at n=10⁵. Its O(n^1.3~2) growth allows it to handle up to 10⁷ (1137ms) before becoming impractical.
- **Quick Sort**: The fastest comparison-based sort. At 2×10⁸, 12.9s vs Merge's 13.1s—the slight edge comes from in-place operations and better cache locality.
- **Merge Sort**: Nearly identical to Quick Sort in performance, with predictable Θ(n lg n) behavior and stability guarantees. The consistent performance makes it reliable for any input distribution.
- **Radix Sort**: Dominates at large scales due to near-linear O(n) time. At n=10⁶, it's 10× faster than Quick/Merge (4.6ms vs ~45ms). At 2×10⁸, 2.3s vs 12.9s—a 5.6× advantage. The bucket overhead makes it slower for tiny inputs (n ≤ 100).

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).