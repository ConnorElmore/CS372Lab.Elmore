#include "LinkedList.h"
#include "Bag.h"
#include "CircularList.h"
#include <iostream>
#include <string>

int main() {
    // ----- LinkedList test -----
    LinkedList<int> L;
    L.insert(1, 10);
    L.insert(2, 20);
    L.insert(3, 30);

    std::cout << "[LinkedList test]\n";
    std::cout << "Length: " << L.getLength() << "\n";
    std::cout << "Entry 2: " << L.getEntry(2) << "\n";

    L.replace(2, 99);
    std::cout << "Entry 2 after replace: " << L.getEntry(2) << "\n";

    L.remove(1);
    std::cout << "First after remove: " << L.getEntry(1) << "\n\n";


    // ----- Bag test -----
    Bag<std::string> b;
    b.add("apple");
    b.add("banana");
    b.add("apple");

    std::cout << "[Bag test]\n";
    std::cout << "Bag size: " << b.getCurrentSize() << "\n";
    std::cout << "Contains banana? " << (b.contains("banana") ? "yes" : "no") << "\n";
    std::cout << "Frequency of apple: " << b.getFrequencyOf("apple") << "\n";

    b.remove("apple");
    std::cout << "After remove, frequency of apple: " << b.getFrequencyOf("apple") << "\n\n";


    // ----- CircularList test -----
    CircularList<int> C;
    C.insert(1, 10);
    C.insert(2, 20);
    C.insert(3, 30);
    C.insert(4, 40);

    auto order = C.traverseFrom(3); // expect 30, 40, 10, 20
    std::cout << "[CircularList test]\n";
    std::cout << "From 3: ";
    for (auto v : order) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
