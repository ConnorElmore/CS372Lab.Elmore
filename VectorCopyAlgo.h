#pragma once
// VectorCopyAlgo.h
#ifndef VECTOR_COPY_ALGO_H
#define VECTOR_COPY_ALGO_H

#include "Vector.h"
#include <algorithm> // std::copy

template <class T>
class VectorCopyAlgo : public Vector<T> {
protected:
    void growIfNeeded(std::size_t minCap) override {
        if (this->cap_ >= minCap) return;
        std::size_t newCap = this->nextCapacity(minCap);
        T* nd = new T[newCap];
        std::copy(this->data_, this->data_ + this->size_, nd);
        delete[] this->data_;
        this->data_ = nd;
        this->cap_ = newCap;
    }
};

#endif
