#include <iostream>
#include "MatrixMultiplication.h"

int main()
{
    std::cout << "Comparison test of serial and parallel matrix multiplication..." << std::endl;
    testBoth();
    std::cout << "\nLarge-scale parallel matrix multiplication test..." << std::endl;
    testParallel();
    return 0;
}