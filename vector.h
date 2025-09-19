#pragma once
// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>   // std::move

template <class T>
class Vector {
protected:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;

    // growth policy: start at 1, double until we have enough room
    virtual std::size_t nextCapacity(std::size_t want) const {
        std::size_t c = (cap_ == 0) ? 1 : cap_;
        while (c < want) c *= 2;
        return c;
    }

    // reallocate if needed; base class uses a simple for-loop copy
    virtual void growIfNeeded(std::size_t minCap) {
        if (cap_ >= minCap) return;
        std::size_t newCap = nextCapacity(minCap);
        T* nd = new T[newCap];
        for (std::size_t i = 0; i < size_; ++i) nd[i] = data_[i];
        delete[] data_;
        data_ = nd;
        cap_ = newCap;
    }

public:
    Vector() = default;
    ~Vector() { delete[] data_; }

    // copy/move
    Vector(const Vector& rhs) : data_(nullptr), size_(rhs.size_), cap_(rhs.cap_) {
        if (cap_) {
            data_ = new T[cap_];
            for (std::size_t i = 0; i < size_; ++i) data_[i] = rhs.data_[i];
        }
    }
    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            T* nd = nullptr;
            if (rhs.cap_) {
                nd = new T[rhs.cap_];
                for (std::size_t i = 0; i < rhs.size_; ++i) nd[i] = rhs.data_[i];
            }
            delete[] data_;
            data_ = nd; size_ = rhs.size_; cap_ = rhs.cap_;
        }
        return *this;
    }
    Vector(Vector&& rhs) noexcept : data_(rhs.data_), size_(rhs.size_), cap_(rhs.cap_) {
        rhs.data_ = nullptr; rhs.size_ = 0; rhs.cap_ = 0;
    }
    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            delete[] data_;
            data_ = rhs.data_;
            size_ = rhs.size_;
            cap_ = rhs.cap_;
            rhs.data_ = nullptr; rhs.size_ = 0; rhs.cap_ = 0;
        }
        return *this;
    }

    // basic queries
    std::size_t size()     const { return size_; }
    bool        empty()    const { return size_ == 0; }
    std::size_t capacity() const { return cap_; }

    // element access
    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    T& at(std::size_t i) {
        if (i >= size_) throw std::out_of_range("at");
        return data_[i];
    }
    const T& at(std::size_t i) const {
        if (i >= size_) throw std::out_of_range("at");
        return data_[i];
    }

    // modifiers
    void push_back(const T& x) {
        growIfNeeded(size_ + 1);
        data_[size_++] = x;
    }
    void push_back(T&& x) {
        growIfNeeded(size_ + 1);
        data_[size_++] = std::move(x);
    }
    void pop_back() {
        if (size_ == 0) throw std::out_of_range("pop_back on empty vector");
        --size_;
    }
    void clear() { size_ = 0; } // leave capacity alone
};

#endif
