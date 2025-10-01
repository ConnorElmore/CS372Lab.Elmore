// MyVector.cpp
#include <iostream>
#include <cassert>

template <typename T>
class Vector {
private:
    T* data;
    int sz;
    int cap;

    void resize(int newCap) {
        T* newData = new T[newCap];
        for (int i = 0; i < sz; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    Vector() {
        data = nullptr;
        sz = 0;
        cap = 0;
    }

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (sz == cap) {
            int newCap = (cap == 0) ? 2 : cap * 2;
            resize(newCap);
        }
        data[sz++] = value;
    }

    int size() const {
        return sz;
    }

    T& operator[](int index) {
        assert(index >= 0 && index < sz);
        return data[index];
    }

    const T& operator[](int index) const {
        assert(index >= 0 && index < sz);
        return data[index];
    }

    //  Iterator class
    class iterator {
    private:
        Vector* vec;
        int index;
    public:
        iterator(Vector* v, int i) : vec(v), index(i) {}

        T& operator*() {
            return (*vec)[index];
        }

        iterator& operator++() { // prefix
            index++;
            return *this;
        }

        iterator operator++(int) { // postfix
            iterator temp = *this;
            index++;
            return temp;
        }

        bool operator==(const iterator& other) const {
            return vec == other.vec && index == other.index;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // begin() and end()
    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, sz);
    }
};


// Example main

int main() {
    Vector<int> v;

    for (int i = 1; i <= 5; i++) {
        v.push_back(i);
    }

    std::cout << "Elements using iterator: ";
    for (Vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    std::cout << "Sum = " << sum << "\n";

    return 0;
}
