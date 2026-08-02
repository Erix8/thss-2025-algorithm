#include <iostream>
#include "Fibonacci.h"

int main()
{
    int n = 0;
    std::cout << "input scale n: ";
    std::cin >> n;
    if (n > 0)
    {
        Fibonacci_test(n);
    }
    else
        std::cout << "Invalid scale n." << std::endl;
    return 0;
}