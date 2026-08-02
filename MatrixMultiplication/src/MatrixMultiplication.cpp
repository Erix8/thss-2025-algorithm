#include "MatrixMultiplication.h"
#include <iomanip>
#include <omp.h>

// -----------------------------------------------------------
// Naive serial sub-block multiply: C += A * B  (triple loop)
// -----------------------------------------------------------
static void MatrixMultiply_Block(const std::vector<std::vector<int>> &A, int aRow, int aCol,
                                 const std::vector<std::vector<int>> &B, int bRow, int bCol,
                                 std::vector<std::vector<int>> &C, int cRow, int cCol,
                                 int n)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[cRow + i][cCol + j] += A[aRow + i][aCol + k] * B[bRow + k][bCol + j];
}

// -----------------------------------------------------------
// Parallel divide-and-conquer recursive multiply (OpenMP)
// C += A * B; D is a temporary accumulator matrix
// -----------------------------------------------------------
static void blockMultiplyRecursive(std::vector<std::vector<int>> &A, int aRow, int aCol,
                                   std::vector<std::vector<int>> &B, int bRow, int bCol,
                                   std::vector<std::vector<int>> &C, int cRow, int cCol,
                                   std::vector<std::vector<int>> &D, int dRow, int dCol,
                                   int n)
{
    if (n <= 20)
    {
        MatrixMultiply_Block(A, aRow, aCol, B, bRow, bCol, C, cRow, cCol, n);
        return;
    }

    int newSize = n / 2;

    // Use omp task to distribute sub-problems to worker threads.
    // The outermost call is inside a #pragma omp parallel / single region.
#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow, aCol, B, bRow, bCol, C, cRow, cCol, D, dRow, dCol, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow, aCol, B, bRow, bCol + newSize, C, cRow, cCol + newSize, D, dRow, dCol + newSize, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow + newSize, aCol, B, bRow, bCol, C, cRow + newSize, cCol, D, dRow + newSize, dCol, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow + newSize, aCol, B, bRow, bCol + newSize, C, cRow + newSize, cCol + newSize, D, dRow + newSize, dCol + newSize, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow, aCol + newSize, B, bRow + newSize, bCol, D, dRow, dCol, D, dRow, dCol, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow, aCol + newSize, B, bRow + newSize, bCol + newSize, D, dRow, dCol + newSize, D, dRow, dCol + newSize, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow + newSize, aCol + newSize, B, bRow + newSize, bCol, D, dRow + newSize, dCol, D, dRow + newSize, dCol, newSize);

#pragma omp task shared(A, B, C, D) firstprivate(aRow, aCol, bRow, bCol, cRow, cCol, dRow, dCol, newSize)
    blockMultiplyRecursive(A, aRow + newSize, aCol + newSize, B, bRow + newSize, bCol + newSize, D, dRow + newSize, dCol + newSize, D, dRow + newSize, dCol + newSize, newSize);

#pragma omp taskwait

    // Serial merge D into C (O(n²) vs O(n³) for multiply)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[cRow + i][cCol + j] += D[dRow + i][dCol + j];
}

// -----------------------------------------------------------
// Public API
// -----------------------------------------------------------

extern void serialMatrixMultiply(const std::vector<std::vector<int>> &A,
                                 const std::vector<std::vector<int>> &B,
                                 std::vector<std::vector<int>> &C,
                                 int n)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = 0;

    MatrixMultiply_Block(A, 0, 0, B, 0, 0, C, 0, 0, n);
}

extern void parallelMatrixMultiply(const std::vector<std::vector<int>> &A,
                                   const std::vector<std::vector<int>> &B,
                                   std::vector<std::vector<int>> &C,
                                   int n)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = 0;

    std::vector<std::vector<int>> D(n, std::vector<int>(n, 0));

#pragma omp parallel
    {
#pragma omp single
        blockMultiplyRecursive(
            const_cast<std::vector<std::vector<int>> &>(A), 0, 0,
            const_cast<std::vector<std::vector<int>> &>(B), 0, 0,
            C, 0, 0,
            D, 0, 0,
            n);
    }
}

// -----------------------------------------------------------
// Tests
// -----------------------------------------------------------

extern void testBoth()
{
    std::vector<int> sizes = {64, 128, 256, 512, 1024};

    std::cout << std::setw(15) << "Matrix Size"
              << std::setw(30) << "Serial (ms)"
              << std::setw(30) << "Parallel (ms)" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (int size : sizes)
    {
        std::vector<std::vector<int>> A(size, std::vector<int>(size, 1));
        std::vector<std::vector<int>> B(size, std::vector<int>(size, 2));
        std::vector<std::vector<int>> C_serial(size, std::vector<int>(size, 0));
        std::vector<std::vector<int>> C_parallel(size, std::vector<int>(size, 0));

        auto t1 = std::chrono::high_resolution_clock::now();
        serialMatrixMultiply(A, B, C_serial, size);
        auto t2 = std::chrono::high_resolution_clock::now();
        double serial_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

        auto t3 = std::chrono::high_resolution_clock::now();
        parallelMatrixMultiply(A, B, C_parallel, size);
        auto t4 = std::chrono::high_resolution_clock::now();
        double parallel_ms = std::chrono::duration<double, std::milli>(t4 - t3).count();

        std::cout << std::setw(15) << size
                  << std::setw(30) << std::fixed << std::setprecision(0) << serial_ms
                  << std::setw(30) << std::fixed << std::setprecision(0) << parallel_ms
                  << std::endl;
    }
}

extern void testParallel()
{
    std::vector<int> sizes = {128, 256, 512, 1024, 2048};

    std::cout << std::setw(15) << "Matrix Size"
              << std::setw(30) << "Parallel (ms)" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    for (int size : sizes)
    {
        std::vector<std::vector<int>> A(size, std::vector<int>(size, 1));
        std::vector<std::vector<int>> B(size, std::vector<int>(size, 2));
        std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));

        auto t1 = std::chrono::high_resolution_clock::now();
        parallelMatrixMultiply(A, B, C, size);
        auto t2 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

        std::cout << std::setw(15) << size
                  << std::setw(30) << std::fixed << std::setprecision(0) << ms
                  << std::endl;
    }
}