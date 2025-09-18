#pragma once
// CircularList.h
// A simple circular traversal built on top of LinkedList

#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include "LinkedList.h"
#include <vector>
#include <stdexcept>

template <class T>
class CircularList : public LinkedList<T> {
public:
    // Return exactly one full loop of items starting at startIndex (1-based).
    // If n=4 and startIndex=3 -> [3,4,1,2]
    std::vector<T> traverseFrom(std::size_t startIndex) const {
        std::vector<T> out;
        std::size_t n = this->getLength();
        if (n == 0) return out;
        if (startIndex < 1 || startIndex > n) throw std::out_of_range("startIndex");

        out.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            std::size_t pos = ((startIndex - 1 + i) % n) + 1; // wrap with modulo
            out.push_back(this->getEntry(pos));
        }
        return out;
    }
};

#endif
