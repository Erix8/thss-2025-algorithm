#include "Fibonacci.h"

extern void Fibonacci_test(int n)
{
    double time = 0;

    std::cout << "Fibonacci Test:" << n << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    unsigned long long ans = 0;
    std::cout << "Naive_Recursive: " << std::endl;
    if (n <= 43)
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = Naive_Recursive(n);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Answer: " << ans << std::endl;
        std::cout << "Time cost: " << time << "ms" << std::endl;
    }
    else
        std::cout << "Too large scale for Naive_Recursive." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    ans = 0, time = 0;
    std::cout << "Botton_Up: " << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = Botton_Up(n);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    std::cout << "Answer: " << ans << std::endl;
    std::cout << "Time cost: " << time << "ms" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    ans = 0, time = 0;
    std::cout << "Binet_Recursive_Squaring: " << std::endl;
    if (n < 95)
    {
        auto start = std::chrono::high_resolution_clock::now();
        ans = round(Binet_Recursive_Squaring(n));
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Answer: " << ans << std::endl;
        std::cout << "Time cost: " << time << "ms" << std::endl;
    }
    else
    {
        std::cout << "Out of computation Range of the program." << std::endl;
    }
    std::cout << "--------------------------------------------" << std::endl;

    ans = 0, time = 0;
    Matrix answer(0, 0, 0, 0);
    std::cout << "Matrix_Recursive_Squaring: " << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        answer = Matrix_Recursive_Squaring(n);
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
    }
    ans = answer.a12;
    std::cout << "Answer: " << ans << std::endl;
    std::cout << "Time cost: " << time << "ms" << std::endl;
}

extern unsigned long long Naive_Recursive(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return Naive_Recursive(n - 1) + Naive_Recursive(n - 2);
}

extern unsigned long long Botton_Up(int n)
{
    std::vector<unsigned long long> f(n + 1);
    f[0] = 0, f[1] = 1;
    for (int i = 2; i <= n; ++i)
    {
        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n];
}

double phi = (1 + sqrt(5)) / 2;
extern double Binet_Recursive_Squaring(int n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return phi / sqrt(5);
    double res = Binet_Recursive_Squaring(n / 2);
    return res * res * (n % 2 == 0 ? 1 : phi) * sqrt(5);
}

Matrix identity(1, 0, 0, 1);
Matrix base(1, 1, 1, 0);
extern Matrix Matrix_Recursive_Squaring(int n)
{
    if (n == 0)
        return identity;
    if (n == 1)
        return base;
    Matrix res = Matrix_Recursive_Squaring(n / 2);
    return res * res * (n % 2 == 0 ? identity : base);
}