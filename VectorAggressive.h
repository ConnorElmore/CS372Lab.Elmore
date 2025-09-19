#pragma once
// VectorAggressive.h
#ifndef VECTOR_AGGRESSIVE_H
#define VECTOR_AGGRESSIVE_H

#include "Vector.h"

template <class T>
class VectorAggressive : public Vector<T> {
protected:
    std::size_t nextCapacity(std::size_t want) const override {
        std::size_t c = (this->cap_ == 0) ? 128 : this->cap_;
        while (c < want) c *= 4;
        return c;
    }
    // copy loop is fine here (or std::copy if you prefer)
    void growIfNeeded(std::size_t minCap) override {
        if (this->cap_ >= minCap) return;
        std::size_t newCap = nextCapacity(minCap);
        T* nd = new T[newCap];
        for (std::size_t i = 0; i < this->size_; ++i) nd[i] = this->data_[i];
        delete[] this->data_;
        this->data_ = nd;
        this->cap_ = newCap;
    }
};

#endif
