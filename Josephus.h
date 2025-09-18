#pragma once
// Josephus.h
#ifndef JOSEPHUS_H
#define JOSEPHUS_H

#include "Queue.h"
#include <vector>

inline std::vector<int> josephusOrder(int N, int M) {
    std::vector<int> out;
    if (N <= 0) return out;

    Queue<int> q;
    for (int i = 1; i <= N; ++i) q.enqueue(i);

    while (q.getSize() > 1) {
        for (int i = 0; i < M; ++i) { // pass M times
            int x = q.front();
            q.dequeue();
            q.enqueue(x);
        }
        out.push_back(q.front()); // eliminated
        q.dequeue();
    }
    return out; // does not include the winner
}

inline int josephusWinner(int N, int M) {
    if (N <= 0) return -1;
    Queue<int> q;
    for (int i = 1; i <= N; ++i) q.enqueue(i);

    while (q.getSize() > 1) {
        for (int i = 0; i < M; ++i) {
            int x = q.front();
            q.dequeue();
            q.enqueue(x);
        }
        q.dequeue(); // eliminate
    }
    return q.front();
}

#endif
