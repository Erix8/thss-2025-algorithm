# Finding Closest Pair of Points

A cross-platform C++ project that solves the **closest pair of points** problem — finding the minimum Euclidean distance among a set of 2D points — using two algorithms with different time complexities, and benchmarks them against each other.

## Overview

Given *n* points in the plane, the goal is to find the pair with the smallest distance. The project implements and compares:

- **Brute-force algorithm** — Θ(n²)
- **Divide-and-conquer algorithm** — Θ(n lg n)

The program generates random data or reads it from a file, runs both algorithms, and reports their execution time to demonstrate the complexity gap.

## Project Structure

```
ClosestPoints/
├── README.md
├── CMakeLists.txt              # Cross-platform build
└── src/
    ├── Points.hpp/.cpp         # Point class, distance & random data generation
    ├── FindNearestPoints.hpp/.cpp  # Brute-force & divide-and-conquer algorithms
    ├── FileIO.hpp/.cpp         # Read/write point data to files
    ├── Test.hpp/.cpp           # Test runner, benchmarking & interactive menu
    └── main.cpp                # Entry point
```

## Algorithms

### Brute Force — Θ(n²)

The brute-force algorithm iterates over every pair of points, computes the Euclidean distance, and keeps the global minimum.

```cpp
double Brutual_Finding(const std::vector<Point> &points, int left, int right)
{
    double minDist = std::numeric_limits<double>::max();
    for (int i = left; i < right; i++)
        for (int j = i + 1; j < right; j++)
        {
            double tmp = distance(points[i], points[j]);
            minDist = std::min(minDist, tmp);
        }
    return minDist;
}
```

It computes *n(n-1)/2* pairwise distances, hence Θ(n²) time.

### Divide and Conquer — Θ(n lg n)

1. Sort all points by **x** coordinate and split the set in half by the median line.
2. Recursively find the minimum distance `dLeft` and `dRight` in the left and right halves; for small subproblems (fewer than 20 points) fall back to brute force.
3. Take `d = min(dLeft, dRight)`.
4. Any closer pair must lie in a *d*-wide vertical strip around the median. Collect the strip points and sort them by **y**; for each point it suffices to check only the next 7 points, because at most 8 points can fit in a 2d × d rectangle.

```cpp
double Advanced_Finding(const std::vector<Point> &points, int left, int right)
{
    double minDist;
    // brute force for sufficiently small subproblems
    if (right - left < 20)
    {
        minDist = Brutual_Finding(points, left, right);
        return minDist;
    }

    // recurse on left and right halves
    int mid = left + (right - left) / 2;
    double dLeft = Advanced_Finding(points, left, mid);
    double dRight = Advanced_Finding(points, mid, right);
    minDist = std::min(dLeft, dRight);

    // build a narrow vertical strip around the median
    std::vector<Point> middleArea;
    double midX = points[mid].getX();
    for (int i = left; i < right; i++)
        if (std::fabs(points[i].getX() - midX) <= minDist)
            middleArea.push_back(points[i]);

    // sort strip points by Y and check nearby candidates
    SortListByY(middleArea);
    int len = middleArea.size();
    for (int i = 0; i < len; i++)
    {
        int l = std::min(i + 7, len);
        for (int j = i + 1; j < l; j++)
        {
            double dis = distance(middleArea[i], middleArea[j]);
            minDist = std::min(minDist, dis);
        }
    }
    return minDist;
}
```

The recurrence is *T(n) = 2T(n/2) + Θ(n)*, giving Θ(n lg n).

## Building

The project uses [CMake](https://cmake.org/) as the build system, which works natively on Windows, macOS, and Linux:

```bash
cd ClosestPoints
cmake -S . -B build
cmake --build build
```

The executable is then located at `build/closest_points` (or `build\Release\closest_points.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Running & Testing

```
./build/closest_points        # or build\Release\closest_points.exe on Windows
```

The program shows an interactive menu with three instructions:

```
Welcome to Finding Test Program!
1. Generate Random Points and Run the Test.
2. Read Points from File and Run the Test.
3. Generate Random Points and Save to File.
Your Instruction:
```

| Instruction | Behavior |
|-------------|----------|
| **1** | Prompts for the number of points and the coordinate range (default (0, max)), then runs both algorithms and reports the minimum distance and time for each |
| **2** | Prompts for a file path; reads points from the file (each line: `x y`) and runs the test |
| **3** | Prompts for the number of points, range, and an output file path; saves generated points to the file |

> **Note**: For point sets larger than 100,000, the brute-force algorithm is automatically skipped to avoid excessive runtime, and only the divide-and-conquer algorithm is benchmarked.

Example output:

```
-------------------------------------
Finding method: Brutual Finding
Answer: 0.0477828
Time cost: 12.6262ms
-------------------------------------
Finding method: Advanced Finding
Answer: 0.0477828
Time cost: 0.525542ms
-------------------------------------
```

## Benchmark Results (Apple M-series)

Points uniformly distributed in (0, 10000) × (0, 10000), times in ms.

### Brute Force

| n      | 1000    | 5000   | 10000  | 20000  | 30000  | 40000  | 50000  |
| ------ | ------- | ------ | ------ | ------ | ------ | ------ | ------ |
| time   | 0.94    | 20.05  | 46.25  | 144.30 | 326.83 | 598.58 | 910.84 |

The runtime grows quadratically (~Θ(n²)), consistent with the theoretical analysis. From n=1000 to n=50000 (50× points, 2500× pairs), actual time increases ~969×.

### Divide and Conquer

| n      | 1000   | 5000  | 10000 | 20000 | 30000 | 40000 | 50000  | 100000 | 500000 | 1000000 | 10000000 |
| ------ | ------ | ----- | ----- | ----- | ----- | ----- | ------ | ------ | ------ | ------- | -------- |
| time   | 0.08   | 0.38  | 0.52  | 1.04  | 1.62  | 2.41  | 3.23   | 7.70   | 36.40  | 69.96   | 601.01   |

The runtime grows near-linearly (~Θ(n lg n)), consistent with the theoretical analysis. From n=1000 to n=10M (10,000× points), actual time increases only ~7,500×.

### Comparison

| n      | 1000   | 5000   | 10000  | 20000  | 30000  | 50000   |
| ------ | ------ | ------ | ------ | ------ | ------ | ------- |
| Brute  | 0.94   | 20.05  | 46.25  | 144.30 | 326.83 | 910.84  |
| Divide | 0.08   | 0.38   | 0.52   | 1.04   | 1.62   | 3.23    |

For n=1000, divide-and-conquer is already ~12× faster. By n=50000, the gap widens to ~282×, demonstrating the O(n lg n) vs O(n²) complexity difference. Beyond n=50000, brute force is skipped entirely as it would take excessively long.

## Timing Measurement

Execution time is measured with the C++11 standard library `<chrono>` (`std::chrono::high_resolution_clock`), which is supported natively on Windows, macOS, and Linux with nanosecond-level resolution. Results are reported in milliseconds (ms).