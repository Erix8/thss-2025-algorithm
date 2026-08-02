#pragma once
#include "Sort.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <stdint.h>

void GenerateData(std::vector<uint32_t> &list, int num);

void TestSortingAlgorithm(std::vector<uint32_t> &list);

void RunSortTest();
