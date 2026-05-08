/*
 * Practical 3: Min, Max, Sum and Average using Parallel Reduction (OpenMP)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
using namespace std;

int main() {
    int N = 10;
    vector<int> arr = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    // ─────────────────────────────────────────
    // MIN using parallel reduction
    // ─────────────────────────────────────────
    int minVal = INT_MAX;
    #pragma omp parallel for reduction(min : minVal)
    for (int i = 0; i < N; i++)
        if (arr[i] < minVal)
            minVal = arr[i];

    // ─────────────────────────────────────────
    // MAX using parallel reduction
    // ─────────────────────────────────────────
    int maxVal = INT_MIN;
    #pragma omp parallel for reduction(max : maxVal)
    for (int i = 0; i < N; i++)
        if (arr[i] > maxVal)
            maxVal = arr[i];

    // ─────────────────────────────────────────
    // SUM using parallel reduction
    // ─────────────────────────────────────────
    long long sum = 0;
    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < N; i++)
        sum += arr[i];

    // ─────────────────────────────────────────
    // AVERAGE
    // ─────────────────────────────────────────
    double avg = (double)sum / N;

    // ─────────────────────────────────────────
    // Results
    // ─────────────────────────────────────────
    cout << "Minimum Value : " << minVal << "\n";
    cout << "Maximum Value : " << maxVal << "\n";
    cout << "Sum           : " << sum    << "\n";
    cout << "Average       : " << avg    << "\n";

    return 0;
}

/*
 * Compile: g++ -fopenmp P3_Parallel_Reduction.cpp -o reduction
 * Run:     ./reduction
 */
