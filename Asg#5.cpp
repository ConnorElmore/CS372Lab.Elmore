#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
using namespace std;

// Bubble Sort (O(n^2))
void bubbleSort(vector<int>& a) {
    const size_t n = a.size();
    for (size_t pass = 0; pass + 1 < n; ++pass) {
        bool swapped = false;
        for (size_t i = 0; i + 1 < n - pass; ++i) {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // stops early if already sorted
    }
}

// Selection Sort (O(n^2))
void selectionSort(vector<int>& a) {
    const size_t n = a.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (a[j] < a[minIdx]) minIdx = j;
        }
        if (minIdx != i) swap(a[i], a[minIdx]);
    }
}

// Insertion Sort (O(n^2))
void insertionSort(vector<int>& a) {
    const size_t n = a.size();
    for (size_t i = 1; i < n; ++i) {
        int key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

// Quicksort (O(n log n) average case)
int partitionLomuto(vector<int>& a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        if (a[j] <= pivot) {
            ++i;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[hi]);
    return i + 1;
}

void quickSortRec(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int p = partitionLomuto(a, lo, hi);
    quickSortRec(a, lo, p - 1);
    quickSortRec(a, p + 1, hi);
}

void quickSort(vector<int>& a) {
    if (!a.empty()) quickSortRec(a, 0, (int)a.size() - 1);
}

// Utility: time how long a sort takes in milliseconds
using clock_t_ = chrono::high_resolution_clock;

template <typename SortFn>
long long timeSortMillis(vector<int> data, SortFn sorter) {
    auto t0 = clock_t_::now();
    sorter(data); // sorts the copy
    auto t1 = clock_t_::now();
    return chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
}

// Make a random vector of size n
vector<int> makeRandomVector(size_t n, int lo = 0, int hi = 1000000) {
    static mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(lo, hi);
    vector<int> v; v.reserve(n);
    for (size_t i = 0; i < n; ++i) v.push_back(dist(rng));
    return v;
}

int main() {
    // Sizes given in the assignment
    const vector<size_t> sizes = {
        10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 250000
    };

    cout << "algo,size,ms\n"; // header for easy CSV export

    for (size_t n : sizes) {
        auto base = makeRandomVector(n);

        // Time each algorithm on the same random data
        long long b_ms = timeSortMillis(base, [](auto& v) { bubbleSort(v); });
        long long s_ms = timeSortMillis(base, [](auto& v) { selectionSort(v); });
        long long i_ms = timeSortMillis(base, [](auto& v) { insertionSort(v); });
        long long q_ms = timeSortMillis(base, [](auto& v) { quickSort(v); });

        cout << "bubble," << n << "," << b_ms << "\n";
        cout << "selection," << n << "," << s_ms << "\n";
        cout << "insertion," << n << "," << i_ms << "\n";
        cout << "quicksort," << n << "," << q_ms << "\n";
    }

    return 0;
}
