#pragma once
// Array.h
#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <stdexcept>

template <class T, std::size_t N>
class Array {
private:
    T data_[N];

public:
    Array() = default;
    // keep this to match the assignment text; enforce N
    explicit Array(std::size_t n) {
        if (n != N) throw std::invalid_argument("Array size mismatch");
    }

    std::size_t size() const { return N; }
    bool empty() const { return N == 0; }

    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    T& at(std::size_t i) {
        if (i >= N) throw std::out_of_range("at");
        return data_[i];
    }
    const T& at(std::size_t i) const {
        if (i >= N) throw std::out_of_range("at");
        return data_[i];
    }

    // optional iteration helpers
    T* begin() { return data_; }
    T* end() { return data_ + N; }
    const T* begin() const { return data_; }
    const T* end()   const { return data_ + N; }
};

#endif
