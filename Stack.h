#pragma once
// Stack.h
#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"
#include <stdexcept>
#include <cstddef>

template <class T>
class Stack {
private:
    LinkedList<T> data;   // top of stack is position 1

public:
    bool isEmpty() const { return data.getLength() == 0; }
    std::size_t getSize() const { return data.getLength(); }
    void clear() { data.clear(); }

    void push(const T& x) {               // O(1)
        data.insert(1, x);
    }

    void pop() {                          // O(1)
        if (isEmpty()) throw std::out_of_range("pop on empty stack");
        data.remove(1);
    }

    T& top() {
        if (isEmpty()) throw std::out_of_range("top on empty stack");
        return data.getEntry(1);
    }
    const T& top() const {
        if (isEmpty()) throw std::out_of_range("top on empty stack");
        return data.getEntry(1);
    }
};

#endif
