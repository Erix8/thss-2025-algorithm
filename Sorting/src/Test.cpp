#include "Test.hpp"

void GenerateData(std::vector<uint32_t> &list, int num)
{
    // 开辟空间
    list.reserve(num);

    // 创建一个随机数引擎，这里使用mt19937（梅森旋转算法）
    std::random_device rd;
    std::mt19937 gen(rd());

    // 定义一个分布，范围是(0, 2^32 - 1)
    std::uniform_int_distribution<uint32_t> dis(1, UINT32_MAX);

    for (int i = 0; i < num; i++)
    {
        uint32_t tmp = dis(gen);
        list.push_back(tmp);
    }
    std::cout << "Data Generating Complete!" << std::endl;
}

void TestSortingAlgorithm(std::vector<uint32_t> &list)
{
    double time = 0;
    std::vector<uint32_t> testlist = list;

    testlist = list;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Sorting method: Insertion Sort" << std::endl;
    if (list.size() < 1000000)
    {
        auto start = std::chrono::high_resolution_clock::now();
        InsertionSort(testlist, 0, list.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Time cost: " << time << "ms" << std::endl;
    }
    else
    {
        std::cout << "Too large scale for Insertion Sort." << std::endl;
    }

    testlist = list;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Sorting method: Shell Sort" << std::endl;
    if (list.size() < 100000000)
    {
        auto start = std::chrono::high_resolution_clock::now();
        ShellSort(testlist, 0, list.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Time cost: " << time << "ms" << std::endl;
    }
    else
    {
        std::cout << "Too large scale for Shell Sort." << std::endl;
    }

    testlist = list;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Sorting method: Quick Sort" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        QuickSort(testlist, 0, list.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;

    testlist = list;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Sorting method: Merge Sort" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        MergeSort(testlist, 0, list.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;

    testlist = list;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Sorting method: Radix Sort" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        RadixSort(testlist, 0, list.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Time cost: " << time << "ms" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

void RunSortTest()
{
    std::vector<uint32_t> datalist;
    size_t listsize = 0;
    std::cout << "Size of List: ";
    std::cin >> listsize;
    GenerateData(datalist, listsize);
    TestSortingAlgorithm(datalist);
}