#include <stdexcept>
#include <compare>
#include <utility>
#include <functional>

// -----------------------------
// Minimal List<T> (dynamic array)
// -----------------------------
template <typename T>
class List {
public:
    List() : _data(nullptr), _size(0), _cap(0) {}

    List(const List& other) : _data(nullptr), _size(0), _cap(0) {
        reserve(other._size);
        for (int i = 0; i < other._size; ++i) _data[i] = other._data[i];
        _size = other._size;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            reserve(other._size);
            for (int i = 0; i < other._size; ++i) _data[i] = other._data[i];
            _size = other._size;
        }
        return *this;
    }

    List(List&& other) noexcept
        : _data(other._data), _size(other._size), _cap(other._cap) {
        other._data = nullptr; other._size = 0; other._cap = 0;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            delete[] _data;
            _data = other._data; _size = other._size; _cap = other._cap;
            other._data = nullptr; other._size = 0; other._cap = 0;
        }
        return *this;
    }

    ~List() { delete[] _data; }

    void clear() { _size = 0; }

    void push_back(const T& value) {
        if (_size == _cap) reserve(_cap == 0 ? 4 : _cap * 2);
        _data[_size++] = value;
    }

    int size() const { return _size; }

    T& operator[](int idx) {
        if (idx < 0 || idx >= _size) throw std::out_of_range("List index");
        return _data[idx];
    }

    const T& operator[](int idx) const {
        if (idx < 0 || idx >= _size) throw std::out_of_range("List index");
        return _data[idx];
    }

private:
    void reserve(int newCap) {
        if (newCap <= _cap) return;
        T* nd = new T[newCap];
        for (int i = 0; i < _size; ++i) nd[i] = _data[i];
        delete[] _data;
        _data = nd;
        _cap = newCap;
    }

    T* _data;
    int _size;
    int _cap;
};

// -----------------------------
// Pair<Key,Value>
// -----------------------------
template <typename Key, typename Value>
struct Pair {
    Key   first{};
    Value second{};
    auto operator<=>(const Pair&) const = default;
};

// -----------------------------
// Internal BST<T>
// -----------------------------
template <typename T>
class BST {
public:
    BST() : _root(nullptr), _count(0) {}

    BST(const BST& other) : _root(nullptr), _count(0) {
        _root = clone(other._root);
        _count = other._count;
    }

    BST& operator=(const BST& other) {
        if (this != &other) {
            clear();
            _root = clone(other._root);
            _count = other._count;
        }
        return *this;
    }

    BST(BST&& other) noexcept : _root(other._root), _count(other._count) {
        other._root = nullptr; other._count = 0;
    }

    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            destroy(_root);
            _root = other._root; _count = other._count;
            other._root = nullptr; other._count = 0;
        }
        return *this;
    }

    ~BST() { destroy(_root); }

    int size() const { return _count; }
    bool empty() const { return _count == 0; }

    std::pair<T*, bool> insert_unique(const T& value) {
        Node** cur = &_root;
        while (*cur) {
            if (value < (*cur)->data) cur = &((*cur)->left);
            else if ((*cur)->data < value) cur = &((*cur)->right);
            else return { &((*cur)->data), false };
        }
        *cur = new Node(value);
        ++_count;
        return { &((*cur)->data), true };
    }

    T* find(const T& value) const {
        Node* cur = _root;
        while (cur) {
            if (value < cur->data) cur = cur->left;
            else if (cur->data < value) cur = cur->right;
            else return &(cur->data);
        }
        return nullptr;
    }

    bool erase(const T& value) {
        std::function<bool(Node*&)> rm = [&](Node*& root)->bool {
            if (!root) return false;
            if (value < root->data) return rm(root->left);
            if (root->data < value) return rm(root->right);

            Node* target = root;
            if (!root->left && !root->right) {
                root = nullptr;
            }
            else if (!root->left) {
                root = root->right;
            }
            else if (!root->right) {
                root = root->left;
            }
            else {
                Node* succParent = root;
                Node* succ = root->right;
                while (succ->left) { succParent = succ; succ = succ->left; }
                root->data = succ->data;
                if (succParent->left == succ) succParent->left = succ->right;
                else                          succParent->right = succ->right;
                delete succ;
                --_count;
                return true;
            }
            delete target;
            --_count;
            return true;
            };
        return rm(_root);
    }

    void clear() {
        destroy(_root);
        _root = nullptr;
        _count = 0;
    }

    template <typename Func>
    void inorder(Func f) const {
        inorder_visit(_root, f);
    }

private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* _root;
    int   _count;

    static Node* clone(Node* n) {
        if (!n) return nullptr;
        Node* nn = new Node(n->data);
        nn->left = clone(n->left);
        nn->right = clone(n->right);
        return nn;
    }

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    template <typename Func>
    static void inorder_visit(Node* n, Func f) {
        if (!n) return;
        inorder_visit(n->left, f);
        f(n->data);
        inorder_visit(n->right, f);
    }
};

// -----------------------------
// Dictionary<Key,Value>
// -----------------------------
template <typename Key, typename Value>
class Dictionary {
public:
    Dictionary() = default;

    Dictionary(const Dictionary& other)
        : _tree(other._tree),
        _keysCache(other._keysCache),
        _valuesCache(other._valuesCache),
        _cacheValid(other._cacheValid) {
    }

    Dictionary& operator=(const Dictionary& other) {
        if (this != &other) {
            _tree = other._tree;
            _keysCache = other._keysCache;
            _valuesCache = other._valuesCache;
            _cacheValid = other._cacheValid;
        }
        return *this;
    }

    Dictionary(Dictionary&& other) noexcept
        : _tree(std::move(other._tree)),
        _keysCache(std::move(other._keysCache)),
        _valuesCache(std::move(other._valuesCache)),
        _cacheValid(other._cacheValid) {
        other._cacheValid = false;
    }

    Dictionary& operator=(Dictionary&& other) noexcept {
        if (this != &other) {
            _tree = std::move(other._tree);
            _keysCache = std::move(other._keysCache);
            _valuesCache = std::move(other._valuesCache);
            _cacheValid = other._cacheValid;
            other._cacheValid = false;
        }
        return *this;
    }

    ~Dictionary() = default;

    Value& operator[](const Key& key) {
        Pair<Key, Value> probe{ key, Value{} };
        auto [ptr, inserted] = _tree.insert_unique(probe);
        if (inserted) _cacheValid = false;
        return ptr->second;
    }

    Value& at(const Key& key) {
        Pair<Key, Value> probe{ key, Value{} };
        auto* p = _tree.find(probe);
        if (!p) throw std::out_of_range("Dictionary::at: key not found");
        return p->second;
    }

    const Value& at(const Key& key) const {
        Pair<Key, Value> probe{ key, Value{} };
        auto* p = _tree.find(probe);
        if (!p) throw std::out_of_range("Dictionary::at: key not found");
        return p->second;
    }

    bool insert(const Key& key, const Value& value) {
        Pair<Key, Value> kv{ key, value };
        auto [p, inserted] = _tree.insert_unique(kv);
        if (inserted) _cacheValid = false;
        return inserted;
    }

    bool erase(const Key& key) {
        bool ok = _tree.erase(Pair<Key, Value>{ key, Value{} });
        if (ok) _cacheValid = false;
        return ok;
    }

    void clear() {
        _tree.clear();
        _cacheValid = false;
        _keysCache.clear();
        _valuesCache.clear();
    }

    List<Key>& keys() const {
        rebuildCachesIfNeeded();
        return _keysCache;
    }

    List<Value>& values() const {
        rebuildCachesIfNeeded();
        return _valuesCache;
    }

    bool isEmpty() const { return _tree.empty(); }
    int  size()    const { return _tree.size(); }

private:
    using P = Pair<Key, Value>;
    BST<P> _tree;

    mutable List<Key>   _keysCache;
    mutable List<Value> _valuesCache;
    mutable bool        _cacheValid{ false };

    void rebuildCachesIfNeeded() const {
        if (_cacheValid) return;
        _keysCache.clear();
        _valuesCache.clear();
        _tree.inorder([this](const P& kv) {
            _keysCache.push_back(kv.first);
            _valuesCache.push_back(kv.second);
            });
        _cacheValid = true;
    }
};
