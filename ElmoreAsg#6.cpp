#include <iostream>
#include <memory>
#include <queue>
#include <functional>


template <typename T>
class List {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> head;

public:
    List() : head(nullptr) {}

    void push_front(const T& value) {
        auto newNode = std::make_unique<Node>(value);
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    void display() const {
        Node* current = head.get();
        while (current) {
            std::cout << current->data << " ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};


// Tree class using smart pointers with parent link
template <typename T>
class Tree {
public:
    struct Node {
        T data;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(const T& value) : data(value) {}
    };

private:
    std::shared_ptr<Node> root;

    // helper recursive insert
    std::shared_ptr<Node> insertRec(std::shared_ptr<Node> node, const T& value, std::shared_ptr<Node> parent) {
        if (!node) {
            auto newNode = std::make_shared<Node>(value);
            newNode->parent = parent;
            return newNode;
        }
        if (value < node->data)
            node->left = insertRec(node->left, value, node);
        else
            node->right = insertRec(node->right, value, node);
        return node;
    }

    // helper recursive display
    void inorderRec(std::shared_ptr<Node> node) const {
        if (!node) return;
        inorderRec(node->left);
        std::cout << node->data << " ";
        inorderRec(node->right);
    }

public:
    Tree() : root(nullptr) {}

    void insert(const T& value) {
        root = insertRec(root, value, nullptr);
    }

    void displayInOrder() const {
        inorderRec(root);
        std::cout << std::endl;
    }

    std::shared_ptr<Node> getRoot() const { return root; }

    void setRoot(const std::shared_ptr<Node>& newRoot) { root = newRoot; }
};
// Level Traversal (Breadth-First Traversal)
// Visits nodes level by level, applying the callable "visit" to each.
template <typename T>
void levelTraversal(const Tree<T>& tree, std::function<void(const T&)> visit) {
    auto root = tree.getRoot();
    if (!root) return;

    std::queue<std::shared_ptr<typename Tree<T>::Node>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        visit(current->data);
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}


// Prune function (returns a copy of the tree without leaf nodes)
template <typename T>
std::shared_ptr<typename Tree<T>::Node> pruneHelper(const std::shared_ptr<typename Tree<T>::Node>& node) {
    if (!node) return nullptr;

    // leaf node
    if (!node->left && !node->right) {
        return nullptr;
    }

    auto newNode = std::make_shared<typename Tree<T>::Node>(node->data);
    newNode->left = pruneHelper<T>(node->left);
    newNode->right = pruneHelper<T>(node->right);
    if (newNode->left) newNode->left->parent = newNode;
    if (newNode->right) newNode->right->parent = newNode;

    return newNode;
}

template <typename T>
Tree<T> prune(const Tree<T>& original) {
    Tree<T> newTree;
    auto newRoot = pruneHelper<T>(original.getRoot());
    newTree.setRoot(newRoot);
    return newTree;
}
// Main driver to test the classes

int main() {
    std::cout << "=== List using Smart Pointers ===" << std::endl;
    List<int> list;
    list.push_front(10);
    list.push_front(20);
    list.push_front(30);
    list.display();

    std::cout << "\n=== Tree with Smart Pointers and Parent Links ===" << std::endl;
    Tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(17);

    std::cout << "In-order Traversal: ";
    tree.displayInOrder();

    std::cout << "Level-order Traversal: ";
    levelTraversal<int>(tree, [](const int& value) {
        std::cout << value << " ";
        });
    std::cout << std::endl;

    std::cout << "\n=== Pruned Tree (without leaves) ===" << std::endl;
    Tree<int> prunedTree = prune<int>(tree);
    std::cout << "In-order Traversal of pruned tree: ";
    prunedTree.displayInOrder();

    return 0;
}


// Generative AI Feedback 

/*
Prompt used:
"Analyze the following C++ Tree class that uses shared_ptr for child nodes
and weak_ptr for parent links. What are its strengths, weaknesses, and potential issues?"

AI Tool Output Summary:
Strengths:
 - Smart pointers prevent most memory leaks and dangling pointers.
 - weak_ptr for parent breaks circular reference cycles.
 - Clear ownership model makes code safer for dynamic trees.

Weaknesses:
 - shared_ptr introduces small performance overhead due to reference counting.
 - Recursive functions could risk stack overflow on very deep trees.
 - No balancing or error checking in insert().

Potential Problems:
 - Cycles could still appear if parent pointers were accidentally made shared_ptr.
 - Copying trees can be expensive unless explicitly managed.

Overall: Design is safe and idiomatic for modern C++, suitable for teaching pointer ownership and recursion.
*/
