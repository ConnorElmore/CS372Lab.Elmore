#pragma once
// Queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"
#include <stdexcept>
#include <cstddef>

template <class T>
class Queue {
private:
    LinkedList<T> data;   // front is position 1

public:
    bool isEmpty() const { return data.getLength() == 0; }
    std::size_t getSize() const { return data.getLength(); }
    void clear() { data.clear(); }

    void enqueue(const T& x) {                    // O(n) with current LinkedList
        data.insert(data.getLength() + 1, x);     // append to back
    }

    void dequeue() {                               // O(1)
        if (isEmpty()) throw std::out_of_range("dequeue on empty queue");
        data.remove(1);
    }

    T& front() {
        if (isEmpty()) throw std::out_of_range("front on empty queue");
        return data.getEntry(1);
    }
    const T& front() const {
        if (isEmpty()) throw std::out_of_range("front on empty queue");
        return data.getEntry(1);
    }
};

#endif
