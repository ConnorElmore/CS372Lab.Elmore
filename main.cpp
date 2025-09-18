#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Josephus.h"
#include <iostream>
#include <string>

int main() {
    // ----- Stack quick test -----
    Stack<int> st;
    st.push(10); st.push(20); st.push(30);
    std::cout << "[Stack] top: " << st.top() << " size: " << st.getSize() << "\n";
    st.pop();
    std::cout << "[Stack] top after pop: " << st.top() << "\n";

    // ----- Queue quick test -----
    Queue<std::string> q;
    q.enqueue("a"); q.enqueue("b"); q.enqueue("c");
    std::cout << "[Queue] front: " << q.front() << " size: " << q.getSize() << "\n";
    q.dequeue();
    std::cout << "[Queue] front after dequeue: " << q.front() << "\n";

    // ----- Josephus tests -----
    auto ord0 = josephusOrder(5, 0); // expect 1,2,3,4; winner 5
    std::cout << "[Josephus M=0, N=5] elim: ";
    for (int x : ord0) std::cout << x << " ";
    std::cout << " winner: " << josephusWinner(5, 0) << "\n";

    auto ord1 = josephusOrder(5, 1); // expect 2,4,1,5; winner 3
    std::cout << "[Josephus M=1, N=5] elim: ";
    for (int x : ord1) std::cout << x << " ";
    std::cout << " winner: " << josephusWinner(5, 1) << "\n";

    // one more
    std::cout << "[Josephus M=2, N=10] winner: " << josephusWinner(10, 2) << "\n";
}
