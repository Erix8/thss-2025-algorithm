#include <iostream>
#include "ParallelSort.h"

int main()
{
    std::cout << "========================================================" << std::endl;
    std::cout << "  Serial vs Parallel MergeSort & QuickSort Comparison  " << std::endl;
    std::cout << "========================================================" << std::endl;
    std::cout << std::endl;

    testBoth();

    std::cout << std::endl;
    std::cout << "========================================================" << std::endl;
    std::cout << "       Large-Scale Parallel Sorting Comparison          " << std::endl;
    std::cout << "========================================================" << std::endl;
    std::cout << std::endl;

    testParallel();

    return 0;
}