#pragma once
#include <iostream>
#include <vector>
#include <chrono>

// Serial matrix multiplication: C += A * B (standard triple loop, O(n^3))
extern void serialMatrixMultiply(const std::vector<std::vector<int>> &A,
                                 const std::vector<std::vector<int>> &B,
                                 std::vector<std::vector<int>> &C,
                                 int n);

// Parallel divide-and-conquer matrix multiplication with OpenMP
// Uses block decomposition to split the work across threads
extern void parallelMatrixMultiply(const std::vector<std::vector<int>> &A,
                                   const std::vector<std::vector<int>> &B,
                                   std::vector<std::vector<int>> &C,
                                   int n);

// Compare serial vs parallel performance across matrix sizes
extern void testBoth();

// Test parallel algorithm on large-scale matrices
extern void testParallel();