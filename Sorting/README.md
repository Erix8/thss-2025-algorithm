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
Time cost: 95.1306ms
-------------------------------------
Sorting method: Shell Sort
Time cost: 1.85942ms
-------------------------------------
Sorting method: Quick Sort
Time cost: 0.895792ms
-------------------------------------
Sorting method: Merge Sort
Time cost: 2.16754ms
-------------------------------------
Sorting method: Radix Sort
Time cost: 1.9975ms
-------------------------------------
```

> **Note**: Insertion Sort is skipped for input sizes ≥ 10⁶ and Shell Sort for sizes ≥ 10⁸, as their runtime becomes prohibitive at those scales.

## Benchmark Results

Random 32-bit unsigned integers, times in ms:

| n            | 10     | 10²    | 10³    | 10⁴     | 10⁵     | 10⁶     | 10⁷     | 10⁸      | 2×10⁸    |
| ------------ | ------ | ------ | ------ | ------- | ------- | ------- | ------- | -------- | -------- |
| Insertion    | 0.0008 | 0.0219 | 1.0565 | 101.876 | 10256.8 | /       | /       | /        | /        |
| Shell        | 0.0009 | 0.0126 | 0.1986 | 2.2249  | 33.1433 | 444.859 | 6650.4  | /        | /        |
| Quick        | 0.0008 | 0.0104 | 0.13   | 1.2388  | 16.7563 | 199.624 | 2390.1  | 26314.1  | 55129.1  |
| Merge        | 0.0044 | 0.0186 | 0.2453 | 2.2533  | 26.7495 | 300.827 | 3496.64 | 40386.3  | 83601.4  |
| Radix        | 0.071  | 0.1472 | 0.8805 | 2.3835  | 10.7179 | 82.7209 | 793.431 | 7729.53  | 15248.8  |

### Analysis

- **Insertion Sort**: fast on tiny inputs, but explodes to ~10 s at n = 10⁵ and cannot finish for n ≥ 10⁶, reflecting its O(n²) complexity. Not suitable for large data.
- **Shell Sort**: already clearly faster than insertion sort at n = 10³ (0.199 ms vs 1.057 ms). Handles up to 10⁷ but its O(n^1.3~2) growth shows at scale.
- **Quick Sort**: the fastest comparison-based sort across all scales evaluated; the gentlest growth among all five up to 10⁸, consistent with Θ(n lg n) average complexity.
- **Merge Sort**: slightly slower than quick sort on small inputs, but its stable Θ(n lg n) behavior makes it predictable and reliable on large datasets.
- **Radix Sort**: the fastest sort at n ≥ 10⁶ — at 2×10⁸ it takes only ~15 s versus ~55 s for quick sort, demonstrating the advantage of its near-linear time for fixed-width integers.

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).