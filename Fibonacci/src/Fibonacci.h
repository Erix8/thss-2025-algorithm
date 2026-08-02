#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

class Matrix
{
public:
	unsigned long long a11, a12, a21, a22;
	Matrix(unsigned long long a, unsigned long long b, unsigned long long c, unsigned long long d) : a11(a), a12(b), a21(c), a22(d) {}
	Matrix operator*(const Matrix &other) const
	{
		Matrix res(0, 0, 0, 0);
		res.a11 = a11 * other.a11 + a12 * other.a21;
		res.a12 = a11 * other.a12 + a12 * other.a22;
		res.a21 = a21 * other.a11 + a22 * other.a21;
		res.a22 = a21 * other.a12 + a22 * other.a22;
		return res;
	}
};

// most naive recursive method, cost Θ(2^n)
extern unsigned long long Naive_Recursive(int n);

// individually compute each F(n), cost Θ(n)
extern unsigned long long Botton_Up(int n);

// using the formula to compute F(n)
// using divide and conquer at the same time, cost Θ(lgn)
extern double Binet_Recursive_Squaring(int n);

// using matrix multiplication to compute F(n),
// using divide and conquer at the same time, cost Θ(lgn)
extern Matrix Matrix_Recursive_Squaring(int n);

// test each method with specific n and show their time cost
extern void Fibonacci_test(int n);