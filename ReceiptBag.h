#pragma once
// ReceiptBag.h
#ifndef RECEIPT_BAG_H
#define RECEIPT_BAG_H

#include "Vector.h"
#include <stdexcept>

template <class T>
class ReceiptBag {
private:
    Vector<int> recs;
    Vector<T>   items;
    int nextRec = 1;

public:
    bool isEmpty() const { return items.size() == 0; }
    std::size_t size() const { return items.size(); }

    int insert(const T& x) {
        recs.push_back(nextRec);
        items.push_back(x);
        return nextRec++;
    }

    // removes matching receipt; returns the removed item by value
    T remove(int receipt) {
        for (std::size_t i = 0; i < recs.size(); ++i) {
            if (recs[i] == receipt) {
                T out = items[i];

                // swap-remove last into i
                recs[i] = recs[recs.size() - 1];
                items[i] = items[items.size() - 1];

                recs.pop_back();
                items.pop_back();

                return out;
            }
        }
        throw std::invalid_argument("invalid receipt");
    }
};

#endif
