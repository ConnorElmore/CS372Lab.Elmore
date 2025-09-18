#pragma once
#ifndef BAG_H
#define BAG_H

#include "LinkedList.h"
#include <cstddef>

template <class T>
class Bag {
private:
    LinkedList<T> data;

public:
    bool        isEmpty() const { return data.getLength() == 0; }
    std::size_t getCurrentSize() const { return data.getLength(); }
    void        clear() { data.clear(); }

    bool add(const T& item) {
        return data.insert(data.getLength() + 1, item);
    }

    bool remove(const T& item) {
        for (std::size_t i = 1; i <= data.getLength(); ++i) {
            if (data.getEntry(i) == item) {
                return data.remove(i);
            }
        }
        return false;
    }

    bool contains(const T& item) const {
        for (std::size_t i = 1; i <= data.getLength(); ++i) {
            if (data.getEntry(i) == item) return true;
        }
        return false;
    }

    std::size_t getFrequencyOf(const T& item) const {
        std::size_t count = 0;
        for (std::size_t i = 1; i <= data.getLength(); ++i) {
            if (data.getEntry(i) == item) ++count;
        }
        return count;
    }
};

#endif
