#include "Vector.h"
#include "VectorCopyAlgo.h"
#include "VectorAggressive.h"
#include "Array.h"
#include "ReceiptBag.h"

#include <iostream>
#include <chrono>

template <class V>
void time_pushes(const char* label, std::size_t maxPow) {
    using clock = std::chrono::high_resolution_clock;
    for (std::size_t p = 1; p <= maxPow; ++p) {
        std::size_t n = (std::size_t)1 << p; // 2^p
        V v;
        auto t0 = clock::now();
        for (std::size_t i = 0; i < n; ++i) v.push_back((int)i);
        auto t1 = clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << label << " n=" << n << " ms=" << ms << "\n";
    }
}

int main() {
    // ---- timing: base vs std::copy vs aggressive growth
    std::cout << "[timing]\n";
    time_pushes<Vector<int>>("base", 12);                 // up to 4096
    time_pushes<VectorCopyAlgo<int>>("copyAlgo", 12);
    time_pushes<VectorAggressive<int>>("aggressive", 12);

    // ---- Array quick check
    std::cout << "\n[array test]\n";
    Array<int, 5> A;
    for (std::size_t i = 0; i < A.size(); ++i) A[i] = (int)(i * 10);
    for (auto x : A) std::cout << x << " ";
    std::cout << "\n";

    // ---- ReceiptBag quick check
    std::cout << "\n[receipt bag test]\n";
    ReceiptBag<std::string> rb;
    int r1 = rb.insert("alpha");
    int r2 = rb.insert("beta");
    int r3 = rb.insert("gamma");
    std::cout << "size=" << rb.size() << " r1=" << r1 << " r2=" << r2 << " r3=" << r3 << "\n";
    std::cout << "removed by r2: " << rb.remove(r2) << "\n";
    std::cout << "size after: " << rb.size() << "\n";

    return 0;
}
