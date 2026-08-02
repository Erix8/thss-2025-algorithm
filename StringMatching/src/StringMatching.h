#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

// Brute-force string matching, scans every position, O(nm)
extern std::vector<int> bruteForceSearch(const std::string &pattern, const std::string &text);

// KMP algorithm, builds LPS array for O(n) matching
extern std::vector<int> kmpSearch(const std::string &pattern, const std::string &text);

// Boyer-Moore algorithm, uses bad character and good suffix heuristics
extern std::vector<int> boyerMoore(const std::string &pattern, const std::string &text);

// Test each method and show their time cost
extern void StringMatching_test(const std::string &pattern, const std::string &text);