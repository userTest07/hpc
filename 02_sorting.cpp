#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2)
            if (arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2)
            if (arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> L(arr.begin()+l, arr.begin()+m+1);
    vector<int> R(arr.begin()+m+1, arr.begin()+r+1);
    int i=0, j=0, k=l;
    while (i<L.size() && j<R.size())
        arr[k++] = (L[i]<=R[j]) ? L[i++] : R[j++];
    while (i<L.size()) arr[k++]=L[i++];
    while (j<R.size()) arr[k++]=R[j++];
}

void mergeSort(vector<int>& arr, int l, int r, int depth=0) {
    if (l >= r) return;
    int m = (l+r)/2;
    if (depth <= 3) {
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr, l, m, depth+1);
            #pragma omp section
            mergeSort(arr, m+1, r, depth+1);
        }
    } else {
        mergeSort(arr, l, m, depth+1);
        mergeSort(arr, m+1, r, depth+1);
    }
    merge(arr, l, m, r);
}

int main() {
    vector<int> a1 = {9,3,7,1,5,8,2,6,4,0};
    vector<int> a2 = a1, b1 = a1, b2 = a1;

    double t;

    t = omp_get_wtime();
    // sequential bubble
    int n = a1.size();
    for (int i=0;i<n-1;i++)
        for(int j=0;j<n-i-1;j++)
            if(a1[j]>a1[j+1]) swap(a1[j],a1[j+1]);
    cout << "Bubble Sequential: " << omp_get_wtime()-t << " sec" << endl;

    t = omp_get_wtime();
    bubbleSort(a2);
    cout << "Bubble Parallel  : " << omp_get_wtime()-t << " sec" << endl;

    t = omp_get_wtime();
    mergeSort(b1, 0, b1.size()-1);
    cout << "Merge Sequential : " << omp_get_wtime()-t << " sec" << endl;

    t = omp_get_wtime();
    mergeSort(b2, 0, b2.size()-1);
    cout << "Merge Parallel   : " << omp_get_wtime()-t << " sec" << endl;

    cout << "Sorted: ";
    for (int x : b2) cout << x << " ";
    cout << endl;

    return 0;
}
// Compile: g++ -fopenmp P2_Sorting.cpp -o sorting
// Run:     ./sorting
