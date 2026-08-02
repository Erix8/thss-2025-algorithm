#pragma once
#include <iostream>
#include <vector>
#include <stdint.h>

void InsertionSort(std::vector<uint32_t> &list, int left, int right);

void ShellSort(std::vector<uint32_t> &list, int left, int right);

int partition(std::vector<uint32_t> &list, int left, int right);
void QuickSort(std::vector<uint32_t> &list, int left, int right);

void merge(std::vector<uint32_t> &list, int left, int mid, int right);
void MergeSort(std::vector<uint32_t> &list, int left, int right);

uint32_t get8Bits(uint32_t num, int startBit);
void RadixSort(std::vector<uint32_t> &list, int left, int right);