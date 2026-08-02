#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>

// ============================================================
// Serial baseline algorithms
// ============================================================

// Serial MergeSort (classic recursive divide-and-conquer)
void serialMergeSort(std::vector<uint32_t> &A, int left, int right);

// Serial QuickSort (classic recursive with Lomuto partition)
void serialQuickSort(std::vector<uint32_t> &A, int left, int right);

// ============================================================
// Parallel OpenMP algorithms
// ============================================================

// Parallel MergeSort using OpenMP tasks + parallel merge (P-MERGE from the PDF)
// Implements the P-MERGESORT algorithm: spawn two recursive sorts,
// then call parallel merge (P-MERGE) that uses FIND-SPLIT-POINT binary search
void parallelMergeSort(std::vector<uint32_t> &A, int left, int right);

// Parallel QuickSort using OpenMP tasks for recursive calls
// Uses the standard partition as base, spawns tasks for left/right subarrays
void parallelQuickSort(std::vector<uint32_t> &A, int left, int right);

// ============================================================
// Testing utilities
// ============================================================

// Generate random data
void generateData(std::vector<uint32_t> &list, size_t num);

// Verify that array is sorted
bool isSorted(const std::vector<uint32_t> &A);

// Compare serial vs parallel MergeSort and QuickSort across sizes
void testBoth();

// Test parallel algorithms on large-scale data
void testParallel();