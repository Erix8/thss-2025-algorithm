#include "ParallelSort.h"
#include <omp.h>
#include <algorithm>
#include <iomanip>
#include <random>
#include <cmath>
#include <sstream>

// ============================================================
// Helper: Insertion sort for small subarrays (base case)
// ============================================================
static void insertionSort(std::vector<uint32_t> &A, int left, int right)
{
    for (int i = left + 1; i < right; ++i)
    {
        uint32_t key = A[i];
        int j = i - 1;
        while (j >= left && A[j] > key)
        {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = key;
    }
}

// ============================================================
// Serial MergeSort
// ============================================================
static void serialMerge(std::vector<uint32_t> &A, int left, int mid, int right)
{
    int n1 = mid - left;
    int n2 = right - mid;
    std::vector<uint32_t> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = A[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = A[mid + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            A[k++] = L[i++];
        else
            A[k++] = R[j++];
    }
    while (i < n1)
        A[k++] = L[i++];
    while (j < n2)
        A[k++] = R[j++];
}

static void serialMergeSortRecursive(std::vector<uint32_t> &A, int left, int right)
{
    const int INSERTION_THRESHOLD = 16;
    if (right - left <= INSERTION_THRESHOLD)
    {
        insertionSort(A, left, right);
        return;
    }
    if (left + 1 >= right)
        return;
    int mid = left + (right - left) / 2;
    serialMergeSortRecursive(A, left, mid);
    serialMergeSortRecursive(A, mid, right);
    serialMerge(A, left, mid, right);
}

void serialMergeSort(std::vector<uint32_t> &A, int left, int right)
{
    serialMergeSortRecursive(A, left, right);
}

// ============================================================
// Serial QuickSort
// ============================================================
static int partition(std::vector<uint32_t> &A, int left, int right)
{
    // Median-of-three pivot selection for better performance on
    // partially sorted / worst-case inputs
    int mid = left + (right - left) / 2;
    if (A[mid] < A[left])
        std::swap(A[left], A[mid]);
    if (A[right - 1] < A[left])
        std::swap(A[left], A[right - 1]);
    if (A[right - 1] < A[mid])
        std::swap(A[mid], A[right - 1]);
    std::swap(A[mid], A[right - 1]);

    uint32_t pivot = A[right - 1];
    int i = left - 1;
    for (int j = left; j < right - 1; ++j)
    {
        if (A[j] <= pivot)
        {
            ++i;
            std::swap(A[i], A[j]);
        }
    }
    std::swap(A[i + 1], A[right - 1]);
    return i + 1;
}

static void serialQuickSortRecursive(std::vector<uint32_t> &A, int left, int right)
{
    const int INSERTION_THRESHOLD = 16;
    if (right - left <= INSERTION_THRESHOLD)
    {
        insertionSort(A, left, right);
        return;
    }
    if (left + 1 >= right)
        return;
    int pivotIdx = partition(A, left, right);
    serialQuickSortRecursive(A, left, pivotIdx);
    serialQuickSortRecursive(A, pivotIdx + 1, right);
}

void serialQuickSort(std::vector<uint32_t> &A, int left, int right)
{
    serialQuickSortRecursive(A, left, right);
}

// ============================================================
// Parallel MergeSort (P-MERGE-SORT from the PDF)
//
// Key components from the PDF:
// 1. P-MERGE-SORT: spawn two recursive sorts, sync, then P-MERGE
// 2. P-MERGE: uses FIND-SPLIT-POINT to binary-search the split
// 3. P-MERGE-AUX: recursive parallel merge with binary search
//    to split work, achieving span Θ(lg²n)
// ============================================================

// FIND-SPLIT-POINT from the PDF (Figure "Parallel merging")
// Given sorted subarray A[p..r-1] and value x,
// return the index q in [p, r] such that A[q-1] < x <= A[q]
static int findSplitPoint(const std::vector<uint32_t> &A, int p, int r, uint32_t x)
{
    int low = p;
    int high = r; // r is exclusive (r = right index of the subarray)
    while (low < high)
    {
        int mid = low + (high - low) / 2;
        if (x <= A[mid])
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

// P-MERGE-AUX from the PDF: recursive parallel merge helper
// Merges A[p1..r1) and A[p2..r2) into B starting at p3
static void parallelMergeAux(std::vector<uint32_t> &A,
                             int p1, int r1,
                             int p2, int r2,
                             std::vector<uint32_t> &B, int p3)
{
    // Base case: both subarrays empty
    if (p1 >= r1 && p2 >= r2)
        return;

    // Ensure the first subarray is the larger one (swap if needed)
    if (r1 - p1 < r2 - p2)
    {
        std::swap(p1, p2);
        std::swap(r1, r2);
    }

    // If the larger subarray is empty, both are empty -> return
    if (p1 >= r1)
        return;

    int q1 = p1 + (r1 - p1) / 2;
    uint32_t x = A[q1];

    // Binary search for split point in the second subarray
    int q2 = findSplitPoint(A, p2, r2, x);

    // Position in the output array
    int q3 = p3 + (q1 - p1) + (q2 - p2);

    B[q3] = x;

    // Recursively merge the left and right portions in parallel
    // Use threshold to avoid task overhead on small subproblems
    const int PARALLEL_CUTOFF = 500;
    int leftSize = (q1 - p1) + (q2 - p2);
    int rightSize = (r1 - q1 - 1) + (r2 - q2);

    if (leftSize > PARALLEL_CUTOFF && rightSize > PARALLEL_CUTOFF)
    {
#pragma omp task shared(A, B) firstprivate(p1, q1, p2, q2, p3)
        parallelMergeAux(A, p1, q1, p2, q2, B, p3);

#pragma omp task shared(A, B) firstprivate(q1, r1, q2, r2, q3)
        parallelMergeAux(A, q1 + 1, r1, q2, r2, B, q3 + 1);

#pragma omp taskwait
    }
    else
    {
        parallelMergeAux(A, p1, q1, p2, q2, B, p3);
        parallelMergeAux(A, q1 + 1, r1, q2, r2, B, q3 + 1);
    }
}

// P-MERGE from the PDF: parallel merge with O(n) work and O(lg²n) span
static void parallelMerge(std::vector<uint32_t> &A, int left, int mid, int right)
{
    int n = right - left;
    std::vector<uint32_t> B(n);

    parallelMergeAux(A, left, mid, mid, right, B, 0);

    // Copy back in parallel
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
        A[left + i] = B[i];
}

// P-MERGE-SORT from the PDF (Figure "Parallel merge sort")
static void parallelMergeSortRecursive(std::vector<uint32_t> &A, int left, int right)
{
    const int SERIAL_CUTOFF = 4096;
    if (right - left <= SERIAL_CUTOFF)
    {
        serialMergeSortRecursive(A, left, right);
        return;
    }

    if (left + 1 >= right)
        return;

    int mid = left + (right - left) / 2;

    // Spawn both recursive sorts as parallel tasks (as in the PDF pseudocode)
#pragma omp task shared(A) firstprivate(left, mid)
    parallelMergeSortRecursive(A, left, mid);

#pragma omp task shared(A) firstprivate(mid, right)
    parallelMergeSortRecursive(A, mid, right);

#pragma omp taskwait

    // P-MERGE: parallel merge using binary search split (O(lg²n) span)
    parallelMerge(A, left, mid, right);
}

void parallelMergeSort(std::vector<uint32_t> &A, int left, int right)
{
#pragma omp parallel
    {
#pragma omp single
        parallelMergeSortRecursive(A, left, right);
    }
}

// ============================================================
// Parallel QuickSort
//
// The standard QuickSort is well-suited for OpenMP tasks:
// - Partition is serial O(n) but the two recursive calls are
//   independent and can be spawned as parallel tasks
// - Unlike MergeSort, QuickSort works in-place and doesn't need
//   a temporary array
// - We use a cutoff threshold: switch to serial sort for small
//   subarrays to avoid excessive task overhead
// ============================================================

static void parallelQuickSortRecursive(std::vector<uint32_t> &A, int left, int right)
{
    const int SERIAL_CUTOFF = 4096;
    if (right - left <= SERIAL_CUTOFF)
    {
        serialQuickSortRecursive(A, left, right);
        return;
    }

    if (left + 1 >= right)
        return;

    int pivotIdx = partition(A, left, right);

    // Spawn left and right subproblem as parallel tasks
#pragma omp task shared(A) firstprivate(left, pivotIdx)
    parallelQuickSortRecursive(A, left, pivotIdx);

#pragma omp task shared(A) firstprivate(pivotIdx, right)
    parallelQuickSortRecursive(A, pivotIdx + 1, right);

#pragma omp taskwait
}

void parallelQuickSort(std::vector<uint32_t> &A, int left, int right)
{
#pragma omp parallel
    {
#pragma omp single
        parallelQuickSortRecursive(A, left, right);
    }
}

// ============================================================
// Test utilities
// ============================================================

void generateData(std::vector<uint32_t> &list, size_t num)
{
    list.clear();
    list.reserve(num);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(1, UINT32_MAX);

    for (size_t i = 0; i < num; ++i)
        list.push_back(dis(gen));

    std::cout << "Data Generating Complete! (" << num << " elements)" << std::endl;
}

bool isSorted(const std::vector<uint32_t> &A)
{
    for (size_t i = 1; i < A.size(); ++i)
    {
        if (A[i - 1] > A[i])
            return false;
    }
    return true;
}

void testBoth()
{
    // Test sizes: from small to large to show the speedup behavior
    std::vector<size_t> sizes = {10000, 50000, 100000, 500000, 1000000, 5000000};

    std::cout << std::left
              << std::setw(14) << "Size"
              << std::setw(18) << "SerialMerge(ms)"
              << std::setw(18) << "ParaMerge(ms)"
              << std::setw(18) << "Speedup(M)"
              << std::setw(18) << "SerialQuick(ms)"
              << std::setw(18) << "ParaQuick(ms)"
              << std::setw(18) << "Speedup(Q)"
              << std::endl;
    std::cout << std::string(122, '-') << std::endl;

    for (size_t size : sizes)
    {
        std::vector<uint32_t> original;
        generateData(original, size);

        double serialMergeMs = 0, parallelMergeMs = 0;
        double serialQuickMs = 0, parallelQuickMs = 0;

        // --- MergeSort comparison ---
        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            serialMergeSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            serialMergeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Serial MergeSort failed!" << std::endl;
        }
        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            parallelMergeSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            parallelMergeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Parallel MergeSort failed!" << std::endl;
        }

        // --- QuickSort comparison ---
        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            serialQuickSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            serialQuickMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Serial QuickSort failed!" << std::endl;
        }
        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            parallelQuickSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            parallelQuickMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Parallel QuickSort failed!" << std::endl;
        }

        double speedupM = (parallelMergeMs > 0) ? serialMergeMs / parallelMergeMs : 0;
        double speedupQ = (parallelQuickMs > 0) ? serialQuickMs / parallelQuickMs : 0;

        // Build speedup strings with "x" suffix for cleaner column alignment
        std::ostringstream sm, sq;
        sm << std::fixed << std::setprecision(2) << speedupM << "x";
        sq << std::fixed << std::setprecision(2) << speedupQ << "x";

        std::cout << std::left
                  << std::setw(14) << size
                  << std::setw(18) << std::fixed << std::setprecision(1) << serialMergeMs
                  << std::setw(18) << std::fixed << std::setprecision(1) << parallelMergeMs
                  << std::setw(18) << sm.str()
                  << std::setw(18) << std::fixed << std::setprecision(1) << serialQuickMs
                  << std::setw(18) << std::fixed << std::setprecision(1) << parallelQuickMs
                  << std::setw(18) << sq.str()
                  << std::endl;

        std::cout << std::flush;
    }
}

void testParallel()
{
    // Large-scale test for parallel algorithms only
    std::vector<size_t> sizes = {100000, 500000, 1000000, 5000000, 10000000, 20000000};

    std::cout << std::left
              << std::setw(16) << "Size"
              << std::setw(20) << "ParaMergeSort(ms)"
              << std::setw(20) << "ParaQuickSort(ms)"
              << std::endl;
    std::cout << std::string(56, '-') << std::endl;

    for (size_t size : sizes)
    {
        std::vector<uint32_t> original;
        generateData(original, size);

        double parallelMergeMs = 0, parallelQuickMs = 0;

        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            parallelMergeSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            parallelMergeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Parallel MergeSort failed at size " << size << "!" << std::endl;
        }
        {
            std::vector<uint32_t> data = original;
            auto t1 = std::chrono::high_resolution_clock::now();
            parallelQuickSort(data, 0, data.size());
            auto t2 = std::chrono::high_resolution_clock::now();
            parallelQuickMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
            bool ok = isSorted(data);
            if (!ok)
                std::cerr << "  ERROR: Parallel QuickSort failed at size " << size << "!" << std::endl;
        }

        std::cout << std::left
                  << std::setw(16) << size
                  << std::setw(20) << std::fixed << std::setprecision(0) << parallelMergeMs
                  << std::setw(20) << std::fixed << std::setprecision(0) << parallelQuickMs
                  << std::endl;

        std::cout << std::flush;
    }
}