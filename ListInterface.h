#pragma once
// ListInterface.h
#ifndef LIST_INTERFACE_H
#define LIST_INTERFACE_H

#include <cstddef>

template <class T>
class ListInterface {
public:
    virtual ~ListInterface() {}

    virtual bool   isEmpty() const = 0;
    virtual size_t getLength() const = 0;

    virtual bool insert(size_t position, const T& entry) = 0; // 1..len+1
    virtual bool remove(size_t position) = 0;                 // 1..len
    virtual void clear() = 0;

    virtual T& getEntry(size_t position) = 0;
    virtual const T& getEntry(size_t position) const = 0;

    virtual bool replace(size_t position, const T& entry) = 0;
};
#endif
