# Introduction to Algorithms

A collection of algorithm projects covering classic algorithm topics including divide and conquer, dynamic programming, parallel computing, sorting, and string matching. All projects are cross-platform C++ implementations built with CMake; some involve OpenMP parallel programming and Python bindings.

## Project List

| Project | Description | Core Algorithms |
|---------|-------------|-----------------|
| [**ClosestPoints**](ClosestPoints/README.md) | Closest pair of points | Brute force (Θ(n²)) + Divide and conquer (Θ(n lg n)) |
| [**Fibonacci**](Fibonacci/README.md) | Fibonacci number computation | Naive recursion / Bottom-up DP / Binet's formula / Matrix exponentiation |
| [**Sorting**](Sorting/README.md) | Comparison of classic sorting algorithms | Insertion / Shell / Quick / Merge / Radix sort |
| [**StringMatching**](StringMatching/README.md) | Comparison of string matching algorithms | Brute-Force / KMP / Boyer-Moore |
| [**MatrixMultiplication**](MatrixMultiplication/README.md) | Parallel matrix multiplication | Serial triple loop + OpenMP divide-and-conquer parallelization |
| [**ParallelSorting**](ParallelSorting/README.md) | Parallel sorting algorithms | P-MERGESORT + Parallel QuickSort (OpenMP) |
| [**SeamCarving**](SeamCarving/README.md) | Content-aware image resizing | Dynamic programming seam carving (C++ core + Python GUI) |

## Repository Structure

```
.
├── README.md
├── ClosestPoints/             # Closest pair of points — divide and conquer
├── Fibonacci/                 # Fibonacci — comparison of four algorithms
├── Sorting/                   # Comparison of five sorting algorithms
├── StringMatching/            # String matching — KMP & Boyer-Moore
├── MatrixMultiplication/      # Matrix multiplication — OpenMP parallelization
├── ParallelSorting/           # Sorting — OpenMP parallel merge/quick
└── SeamCarving/               # Seam Carving — C++ + Python GUI
```

## Building and Running

All C++ projects are built with CMake. Using ClosestPoints as an example:

```bash
cd ClosestPoints
cmake -S . -B build
cmake --build build
./build/closest_points
```

Projects involving OpenMP (MatrixMultiplication, ParallelSorting) require an additional OpenMP runtime; see each project's README for details.

SeamCarving additionally provides a Python package with CLI and PySide6 GUI support:

```bash
cd SeamCarving
pip install -e ".[gui]"
seamcarving-gui