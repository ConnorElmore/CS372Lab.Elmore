#include <iostream>
#include <random>
#include <chrono>
#include <utility>
#include <vector>
#include "Tree.hpp"
#include "avltree.hpp"
using namespace std;

// builds a regular BST with random integers
Tree<int> buildRandomBST(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, n * 10);

    Tree<int> t;
    for (int i = 0; i < n; ++i)
        t = t.insert(dist(gen));
    return t;
}

// builds an AVL tree from a BST using inorder traversal
AVLTree<int> buildAVLFromBST(const Tree<int>& bst) {
    AVLTree<int> avl;
    bst.inorder([&avl](int val) { avl.insert(val); });
    return avl;
}

// helper to measure elapsed time
template <typename Func>
double measureTime(Func&& func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> diff = end - start;
    return diff.count();
}

// compares search-like operations in BST vs. AVL
pair<double, double> compareSearchTimes(const Tree<int>& bst, AVLTree<int>& avl, int samples = 100) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, bst.size() * 10);

    double totalBST = 0, totalAVL = 0;

    for (int i = 0; i < samples; ++i) {
        int val = dist(gen);

        // measure BST member() time
        totalBST += measureTime([&]() {
            bst.member(val);
        });

        // measure AVL insert() time to simulate traversal cost
        totalAVL += measureTime([&]() {
            avl.insert(val);
        });
    }

    return { totalBST / samples, totalAVL / samples };
}

int main() {
    vector<int> sizes = {100, 500, 1000, 2500, 5000, 10000};

    cout << "Elements\tBST Avg (µs)\tAVL Avg (µs)\n";
    cout << "---------------------------------------\n";

    for (int n : sizes) {
        Tree<int> bst = buildRandomBST(n);
        AVLTree<int> avl = buildAVLFromBST(bst);

        auto [bstAvg, avlAvg] = compareSearchTimes(bst, avl);

        cout << n << "\t\t"
             << bstAvg << "\t\t"
             << avlAvg << endl;
    }

    return 0;
}