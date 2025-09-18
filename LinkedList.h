#pragma once
// LinkedList.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "ListInterface.h"
#include <stdexcept>

template <class T>
class LinkedList : public ListInterface<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item, Node* n = nullptr) : data(item), next(n) {}
    };

    Node* head = nullptr;
    size_t sz = 0;

    Node* getNodeAt(size_t pos) const {
        if (pos < 1 || pos > sz) throw std::out_of_range("invalid pos");
        Node* cur = head;
        for (size_t i = 1; i < pos; ++i) cur = cur->next;
        return cur;
    }

    void copyFrom(const LinkedList& other) {
        Node* cur = other.head;
        Node* tail = nullptr;
        while (cur) {
            Node* n = new Node(cur->data);
            if (!head) head = n;
            else       tail->next = n;
            tail = n;
            cur = cur->next;
            ++sz;
        }
    }

public:
    LinkedList() = default;
    ~LinkedList() override { clear(); }

    LinkedList(const LinkedList& rhs) { copyFrom(rhs); }
    LinkedList& operator=(const LinkedList& rhs) {
        if (this != &rhs) { clear(); copyFrom(rhs); }
        return *this;
    }

    LinkedList(LinkedList&& rhs) noexcept : head(rhs.head), sz(rhs.sz) {
        rhs.head = nullptr; rhs.sz = 0;
    }
    LinkedList& operator=(LinkedList&& rhs) noexcept {
        if (this != &rhs) {
            clear();
            head = rhs.head; sz = rhs.sz;
            rhs.head = nullptr; rhs.sz = 0;
        }
        return *this;
    }

    bool   isEmpty() const override { return sz == 0; }
    size_t getLength() const override { return sz; }

    bool insert(size_t pos, const T& entry) override {
        if (pos < 1 || pos > sz + 1) return false;
        if (pos == 1) head = new Node(entry, head);
        else {
            Node* prev = getNodeAt(pos - 1);
            prev->next = new Node(entry, prev->next);
        }
        ++sz; return true;
    }

    bool remove(size_t pos) override {
        if (pos < 1 || pos > sz) return false;
        Node* doomed;
        if (pos == 1) { doomed = head; head = head->next; }
        else {
            Node* prev = getNodeAt(pos - 1);
            doomed = prev->next;
            prev->next = doomed->next;
        }
        delete doomed; --sz; return true;
    }

    void clear() override {
        while (head) { Node* n = head; head = head->next; delete n; }
        sz = 0;
    }

    T& getEntry(size_t pos) override { return getNodeAt(pos)->data; }
    const T& getEntry(size_t pos) const override { return getNodeAt(pos)->data; }

    bool replace(size_t pos, const T& entry) override {
        if (pos < 1 || pos > sz) return false;
        getNodeAt(pos)->data = entry; return true;
    }
};
#endif
