// LazyList.cpp
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

template <typename T>
class LazyList {
private:
    struct Node {
        T data;
        bool deleted;
        Node* next;
        Node(const T& value, Node* nxt = nullptr)
            : data(value), deleted(false), next(nxt) {
        }
    };

    Node* head;
    std::size_t active_count;   // visible items
    std::size_t deleted_count;  // hidden (marked) items

    void compact_if_needed() {
        if (deleted_count == active_count && deleted_count > 0) {
            Node* prev = nullptr;
            Node* cur = head;
            while (cur) {
                if (cur->deleted) {
                    Node* to_delete = cur;
                    cur = cur->next;
                    if (prev) prev->next = cur;
                    else head = cur;
                    delete to_delete;
                    --deleted_count;
                }
                else {
                    prev = cur;
                    cur = cur->next;
                }
            }
        }
    }

    void copy_from(const LazyList& other) {
        head = nullptr;
        active_count = 0;
        deleted_count = 0;
        Node* tail = nullptr;
        for (Node* cur = other.head; cur; cur = cur->next) {
            Node* n = new Node(cur->data);
            n->deleted = cur->deleted;
            if (!head) { head = tail = n; }
            else { tail->next = n; tail = n; }
            if (n->deleted) ++deleted_count; else ++active_count;
        }
    }

public:
    LazyList() : head(nullptr), active_count(0), deleted_count(0) {}
    LazyList(const LazyList& rhs) : head(nullptr), active_count(0), deleted_count(0) { copy_from(rhs); }
    LazyList& operator=(const LazyList& rhs) {
        if (this != &rhs) { clear(); copy_from(rhs); }
        return *this;
    }
    ~LazyList() { clear(); }

    void clear() {
        Node* cur = head;
        while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
        head = nullptr; active_count = 0; deleted_count = 0;
    }

    void push_front(const T& value) {
        head = new Node(value, head);
        ++active_count;
        compact_if_needed();
    }

    void push_back(const T& value) {
        Node* n = new Node(value);
        if (!head) head = n;
        else {
            Node* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = n;
        }
        ++active_count;
        compact_if_needed();
    }

    // Mark-delete first visible occurrence
    bool mark_delete_first(const T& value) {
        for (Node* cur = head; cur; cur = cur->next) {
            if (!cur->deleted && cur->data == value) {
                cur->deleted = true;
                --active_count; ++deleted_count;
                compact_if_needed();
                return true;
            }
        }
        return false;
    }

    // Mark-delete all visible matches
    std::size_t mark_delete_all(const T& value) {
        std::size_t removed = 0;
        for (Node* cur = head; cur; cur = cur->next) {
            if (!cur->deleted && cur->data == value) {
                cur->deleted = true;
                --active_count; ++deleted_count; ++removed;
            }
        }
        compact_if_needed();
        return removed;
    }

    // Queries ignoring deleted
    std::size_t size() const { return active_count; }
    std::size_t hidden_count() const { return deleted_count; }
    bool empty() const { return active_count == 0; }

    bool contains(const T& value) const {
        for (Node* cur = head; cur; cur = cur->next)
            if (!cur->deleted && cur->data == value) return true;
        return false;
    }

    // Get i-th visible element (0-based). Returns false if out of range.
    bool at(std::size_t index, T& out) const {
        std::size_t i = 0;
        for (Node* cur = head; cur; cur = cur->next) {
            if (!cur->deleted) {
                if (i == index) { out = cur->data; return true; }
                ++i;
            }
        }
        return false;
    }

    std::vector<T> to_vector() const {
        std::vector<T> v;
        v.reserve(active_count);
        for (Node* cur = head; cur; cur = cur->next)
            if (!cur->deleted) v.push_back(cur->data);
        return v;
    }

    void print() const {
        std::cout << "[";
        bool first = true;
        for (Node* cur = head; cur; cur = cur->next) {
            if (!cur->deleted) {
                if (!first) std::cout << ", ";
                std::cout << cur->data; first = false;
            }
        }
        std::cout << "] (active=" << active_count << ", hidden=" << deleted_count << ")\n";
    }
};

int main() {
    LazyList<int> lst;
    lst.push_back(1); lst.push_back(2); lst.push_back(3); lst.push_back(4);
    lst.print(); // [1, 2, 3, 4] (active=4, hidden=0)
    lst.mark_delete_first(2);
    lst.mark_delete_first(4);
    lst.print(); // [1, 3] (active=2, hidden=2) -> auto compaction will run now
    // After compaction, hidden should be 0.
    std::cout << "hidden=" << lst.hidden_count() << "\n";

    // Basic checks
    assert(lst.contains(1));
    assert(!lst.contains(2));
    int x;
    bool ok = lst.at(0, x);
    assert(ok && x == 1);

    // Copy semantics
    LazyList<int> copy = lst;
    copy.push_back(99);
    copy.print();
    lst.print();

    std::cout << "Done.\n";
    return 0;
}
