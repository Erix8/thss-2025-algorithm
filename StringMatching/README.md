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

### Generating test data

A Python script is provided to generate benchmark datasets covering four characteristic scenarios:

```bash
cd StringMatching
python3 gen_testdata.py
```

This creates `data/` with text and pattern files for all scenarios below.

### Running

```
./build/string_matching        # or build\Release\string_matching.exe on Windows
```

Then input the text file name and pattern file name when prompted:

```
Enter the text file name (e.g., text.txt): data/scenarioA_long_text.txt
Enter the pattern file name (e.g., pattern.txt): data/scenarioA_long_pattern.txt
-------------------------------------
Matching method: BruteForce
Answer: No Match.
Time cost: 2.327ms
-------------------------------------
Matching method: KMP
Answer: No Match.
Time cost: 0.284709ms
-------------------------------------
Matching method: Boyer-Moore
Answer: No Match.
Time cost: 0.736625ms
-------------------------------------
```

## Benchmark Results (Apple M-series)

### Scenario A: Repetitive near-matches — KMP Advantage

**Text**: 100,000 repeated `A` characters. **Pattern**: 40×`A` + `B`.

| Algorithm    | Time (ms) | Relative to BruteForce |
|-------------|-----------|------------------------|
| BruteForce  | 2.33      | 1.00× (baseline)       |
| KMP         | 0.28      | **8.18× faster**       |
| Boyer-Moore | 0.74      | 3.16× faster           |

**Why**: The pattern's prefix (40 As) matches at every position, forcing BruteForce to compare 40 characters before discovering the B mismatch — O(n·m) behavior. KMP's LPS array recognizes the self-similar prefix and skips redundant comparisons. BM's bad-character rule helps (A→B mismatch shifts by 1), but good-suffix provides limited benefit here.

### Scenario B: Long Pattern + Large Alphabet — Boyer-Moore Advantage

**Text**: 1,000,000 random characters from [a-zA-Z0-9] (62-character alphabet). One occurrence embedded at position 500,000.

| Pattern Length (m) | BruteForce (ms) | KMP (ms) | Boyer-Moore (ms) | BM Speedup vs BruteForce |
|-------------------|-----------------|----------|-------------------|--------------------------|
| 10                | 1.76            | 3.49     | 2.20              | 0.80×                    |
| 30                | 1.30            | 2.82     | 0.64              | **2.05×**                |
| 60                | 1.10            | 2.66     | 0.38              | **2.92×**                |
| 120               | 1.28            | 2.36     | 0.22              | **5.93×**                |
| 240               | 0.93            | 2.15     | 0.15              | **6.08×**                |

**Why**: With 62-character alphabet, most text characters are **not** in the pattern. BM's bad-character rule skips up to `m` positions per mismatch. As `m` grows, BM scans fewer and fewer positions (~n/m), while BruteForce and KMP must check every character position.

> Note: KMP appears slower due to LPS array construction overhead for patterns this long; its O(n) matching phase is comparable to BruteForce on uncorrelated text.

### Scenario C: Pattern Length Sweep (Fixed Text)

**Text**: 500,000 random lowercase letters. Pattern lengths 5–400, embedded at position 250,000.

| m    | BruteForce (ms) | KMP (ms) | Boyer-Moore (ms) |
|------|-----------------|----------|-------------------|
| 5    | 0.53            | 1.13     | 0.76              |
| 10   | 0.53            | 1.19     | 0.77              |
| 20   | 0.49            | 1.06     | 0.36              |
| 50   | 0.52            | 1.47     | 0.24              |
| 100  | 0.49            | 1.04     | 0.14              |
| 200  | 0.51            | 1.05     | 0.22              |
| 400  | 0.52            | 1.03     | 0.18              |

**Why**: BruteForce is flat across m (≈0.50ms) because the small alphabet (26 chars) causes early mismatches — most comparisons fail at the first character, so O(n) in practice. BM's advantage grows from m=20 onward, reaching ~3× faster at m=400.

### Scenario D: Worst-Case Last-Character Mismatch

**Text**: `"ABCDE"` repeated 100,000 times (500,000 chars). **Pattern**: `"ABCDZ"`.

| Algorithm    | Time (ms) | Relative to BruteForce |
|-------------|-----------|------------------------|
| BruteForce  | 0.97      | 1.00× (baseline)       |
| KMP         | 0.57      | 1.70× faster           |
| Boyer-Moore | 0.12      | **7.86× faster**       |

**Why**: At every position, 4 characters match before the 5th mismatches — worst-case for BruteForce. BM's bad-character rule: seeing `E` (not in `ABCDZ`) shifts by 5, effectively skipping 4/5 of all comparisons. KMP avoids re-comparison of the matched prefix via LPS.

### Summary

| Scenario | Best Algorithm | Speedup over BruteForce |
|----------|---------------|-------------------------|
| A (repetitive near-matches) | KMP | **8.18×** |
| B (long pattern + large alphabet, m=240) | Boyer-Moore | **6.08×** |
| C (sweep, m=400) | Boyer-Moore | **2.89×** |
| D (worst-case border mismatch) | Boyer-Moore | **7.86×** |

- **KMP** shines when the pattern has self-similar prefixes and the text contains many near-matches — its O(n) guarantee ensures predictable, stable performance.
- **Boyer-Moore** dominates when patterns are long and the alphabet is large, achieving sublinear scanning via bad-character skips. It also excels at worst-case border mismatch scenarios.
- **BruteForce** is competitive on random uncorrelated text due to early mismatch exits, but degrades severely in adversarial cases.

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).