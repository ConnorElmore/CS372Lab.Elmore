#include <iostream>
#include <list>
#include <queue>
#include "Tree.hpp"

using namespace std;

// Returns true if this tree is just a single leaf node
template <typename T>
bool isLeaf(const Tree<T>& t) {
    return !t.isEmpty() && t.left().isEmpty() && t.right().isEmpty();
}

// Fills outFringe with the leaves of t in level-order (left-to-right)
template <typename T>
void fringe(const Tree<T>& t, list<T>& outFringe) {
    outFringe.clear();

    if (t.isEmpty()) {
        return;
    }

    queue<Tree<T>> q;
    q.push(t);

    while (!q.empty()) {
        Tree<T> cur = q.front();
        q.pop();

        if (isLeaf(cur)) {
            outFringe.push_back(cur.root());
        } else {
            Tree<T> l = cur.left();
            Tree<T> r = cur.right();

            if (!l.isEmpty()) q.push(l);
            if (!r.isEmpty()) q.push(r);
        }
    }
}

// Convenience version that actually returns the list
template <typename T>
list<T> fringe(const Tree<T>& t) {
    list<T> result;
    fringe(t, result);
    return result;
}

// Compare fringes of two trees
template <typename T>
bool hasSameFringeImpl(const Tree<T>& a, const Tree<T>& b) {
    list<T> fa, fb;
    fringe(a, fa);
    fringe(b, fb);
    return fa == fb;
}

// Non-template wrapper to match the requested signature for Tree<int>
bool hasSameFringe(const Tree<int>& a, const Tree<int>& b) {
    return hasSameFringeImpl<int>(a, b);
}

int main() {
    // Two example trees with different shapes
    Tree<int> t1 = { 5, 3, 7, 2, 4, 6, 8 };
    Tree<int> t2 = { 4, 2, 6, 3, 5, 7, 8 };

    list<int> f1, f2;
    fringe(t1, f1);
    fringe(t2, f2);

    cout << "Fringe of t1: ";
    for (int x : f1) cout << x << " ";
    cout << "\n";

    cout << "Fringe of t2: ";
    for (int x : f2) cout << x << " ";
    cout << "\n";

    cout << boolalpha;
    cout << "hasSameFringe(t1, t2)? " << hasSameFringe(t1, t2) << "\n";

    return 0;
}
