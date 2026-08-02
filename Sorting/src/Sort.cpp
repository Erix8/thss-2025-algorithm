#include "Sort.hpp"

void InsertionSort(std::vector<uint32_t> &list, int left, int right)
{
    for (int i = left + 1; i < right; ++i)
    {
        uint32_t key = list[i];
        int j = i - 1;
        while (j >= left && list[j] > key)
        {
            list[j + 1] = list[j];
            --j;
        }
        list[j + 1] = key;
    }
}

void ShellSort(std::vector<uint32_t> &list, int left, int right)
{
    int n = right - left;
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = left + gap; i < right; ++i)
        {
            uint32_t temp = list[i];
            int j;
            for (j = i; j >= left + gap && list[j - gap] > temp; j -= gap)
            {
                list[j] = list[j - gap];
            }
            list[j] = temp;
        }
    }
}

int partition(std::vector<uint32_t> &list, int left, int right)
{
    uint32_t pivot = list[right - 1];
    int i = left - 1;
    for (int j = left; j < right - 1; ++j)
    {
        if (list[j] < pivot)
        {
            ++i;
            std::swap(list[i], list[j]);
        }
    }
    std::swap(list[i + 1], list[right - 1]);
    return i + 1;
}

void QuickSort(std::vector<uint32_t> &list, int left, int right)
{
    const int INSERTION_THRESHOLD = 10;
    if (right - left < INSERTION_THRESHOLD)
        InsertionSort(list, left, right);
    else if (left < right)
    {
        int pivotIndex = partition(list, left, right);
        QuickSort(list, left, pivotIndex);
        QuickSort(list, pivotIndex + 1, right);
    }
}

void merge(std::vector<uint32_t> &list, int left, int mid, int right)
{
    int n1 = mid - left;
    int n2 = right - mid;

    std::vector<uint32_t> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = list[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = list[mid + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            list[k] = L[i];
            ++i;
        }
        else
        {
            list[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1)
    {
        list[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2)
    {
        list[k] = R[j];
        ++j;
        ++k;
    }
}

void MergeSort(std::vector<uint32_t> &list, int left, int right)
{
    const int INSERTION_THRESHOLD = 10;
    if (right - left < INSERTION_THRESHOLD)
        InsertionSort(list, left, right);
    else if (left < right)
    {
        int mid = left + (right - left) / 2;
        MergeSort(list, left, mid);
        MergeSort(list, mid, right);
        merge(list, left, mid, right);
    }
}

// 获取数字指定位数开始的连续 8 位的值
uint32_t get8Bits(uint32_t num, int startBit)
{
    return (num >> startBit) & 0xFF;
}

void RadixSort(std::vector<uint32_t> &list, int left, int right)
{
    if (left >= right)
        return;
    for (int startBit = 0; startBit < 32; startBit += 8)
    {
        std::vector<std::vector<uint32_t>> buckets(256);
        for (int i = left; i < right; ++i)
        {
            uint32_t bits = get8Bits(list[i], startBit);
            buckets[bits].push_back(list[i]);
        }
        int index = left;
        for (const auto &bucket : buckets)
            for (uint32_t num : bucket)
            {
                list[index] = num;
                index++;
            }
    }
}