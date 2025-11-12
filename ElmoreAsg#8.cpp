
// Graph ADT + adjacency list + adjacency matrix + isSimpleCycle + Kevin Bacon BFS
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <cstdint>
#include <algorithm>

using std::cout;
using std::endl;
using std::size_t;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::queue;


// Graph ADT (representation-agnostic)

template <typename T>
class Graph {
public:
    virtual ~Graph() = default;

    // structure
    virtual bool addVertex(const T& v) = 0;
    virtual bool hasVertex(const T& v) const = 0;

    // undirected edges
    virtual bool addEdge(const T& u, const T& v) = 0;
    virtual bool removeEdge(const T& u, const T& v) = 0;

    // queries
    virtual bool adjacent(const T& u, const T& v) const = 0;   // true if edge(u,v) exists
    virtual vector<T> neighbors(const T& u) const = 0;         // list of v where (u,v) exists
};

// Adjacency List representation
template <typename T>
class AdjacencyListGraph : public Graph<T> {
public:
    bool addVertex(const T& v) override {
        return adj.emplace(v, unordered_set<T>{}).second;
    }

    bool hasVertex(const T& v) const override {
        return adj.find(v) != adj.end();
    }

    bool addEdge(const T& u, const T& v) override {
        if (!hasVertex(u)) addVertex(u);
        if (!hasVertex(v)) addVertex(v);
        bool a = adj[u].insert(v).second;
        bool b = adj[v].insert(u).second;
        // edge counted once (undirected); return true if it was new
        return a || b;
    }

    bool removeEdge(const T& u, const T& v) override {
        auto iu = adj.find(u), iv = adj.find(v);
        if (iu == adj.end() || iv == adj.end()) return false;
        bool a = iu->second.erase(v) > 0;
        bool b = iv->second.erase(u) > 0;
        return a && b;
    }

    bool adjacent(const T& u, const T& v) const override {
        auto it = adj.find(u);
        return it != adj.end() && it->second.count(v) > 0;
    }

    vector<T> neighbors(const T& u) const override {
        vector<T> out;
        auto it = adj.find(u);
        if (it == adj.end()) return out;
        out.reserve(it->second.size());
        for (const auto& n : it->second) out.push_back(n);
        return out;
    }

private:
    unordered_map<T, unordered_set<T>> adj;
};

// Adjacency Matrix representation
template <typename T>
class AdjacencyMatrixGraph : public Graph<T> {
public:
    bool addVertex(const T& v) override {
        if (index.count(v)) return false;
        int i = static_cast<int>(index.size());
        index[v] = i;
        // grow existing rows
        for (auto& row : m) row.push_back(0);
        // add new row
        m.push_back(vector<uint8_t>(index.size(), 0));
        return true;
    }

    bool hasVertex(const T& v) const override {
        return index.find(v) != index.end();
    }

    bool addEdge(const T& u, const T& v) override {
        if (!hasVertex(u)) addVertex(u);
        if (!hasVertex(v)) addVertex(v);
        int i = index.at(u), j = index.at(v);
        bool was = m[i][j] || m[j][i];
        m[i][j] = 1; m[j][i] = 1; // undirected
        return !was;
    }

    bool removeEdge(const T& u, const T& v) override {
        auto iu = index.find(u), iv = index.find(v);
        if (iu == index.end() || iv == index.end()) return false;
        int i = iu->second, j = iv->second;
        if (!m[i][j] && !m[j][i]) return false;
        m[i][j] = 0; m[j][i] = 0;
        return true;
    }

    bool adjacent(const T& u, const T& v) const override {
        auto iu = index.find(u), iv = index.find(v);
        if (iu == index.end() || iv == index.end()) return false;
        return m[iu->second][iv->second] != 0;
    }

    vector<T> neighbors(const T& u) const override {
        vector<T> out;
        auto iu = index.find(u);
        if (iu == index.end()) return out;
        int i = iu->second;
        out.reserve(index.size());
        for (const auto& kv : index) {
            if (m[i][kv.second]) out.push_back(kv.first);
        }
        return out;
    }

private:
    unordered_map<T,int> index;           // value -> matrix index
    vector<vector<uint8_t>> m;            // symmetric 0/1 matrix
};

// isSimpleCycle
// ---------------------------
// Path is a simple cycle if:
// - path.size() >= 2
// - path.front() == path.back()  (starts/ends same node)
// - every consecutive pair is an edge (neighbor step)
// - no vertex repeats internally (only final element can repeat the first)
bool isSimpleCycle(const Graph<int>& g, const std::vector<int>& path) {
    // simple cycle must have at least 3 distinct vertices -> path length >= 4 (u,v,w,u)
    if (path.size() < 4) return false;
    if (path.front() != path.back()) return false;

    std::unordered_set<int> seen; // internal vertices must be unique

    // verify each hop is along an edge and enforce no repeats for internal vertices
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        int u = path[i], v = path[i + 1];
        if (!g.adjacent(u, v)) return false;          // invalid step
        if (i < path.size() - 2) {                    // all vertices except the final closing one
            if (!seen.insert(u).second) return false; // repeated internal vertex
        }
    }
    // no need to check the closing vertex; it's allowed to equal the start
    return true;
}


// Kevin Bacon BFS
// Returns true if there is a path from `you` to "Kevin Bacon" and fills outPath.
// outPath contains vertices from you -> ... -> Kevin Bacon. If no path, returns false and outPath is empty.
bool pathToKevin(const Graph<string>& g,
                 const string& you,
                 vector<string>& outPath,
                 const string& target = "Kevin Bacon")
{
    outPath.clear();
    if (!g.hasVertex(you) || !g.hasVertex(target)) return false;

    unordered_map<string,string> parent;   // child -> parent
    unordered_set<string> visited;
    queue<string> q;

    visited.insert(you);
    q.push(you);

    bool found = false;
    while (!q.empty()) {
        string u = q.front(); q.pop();
        if (u == target) { found = true; break; }
        for (const auto& v : g.neighbors(u)) {
            if (!visited.count(v)) {
                visited.insert(v);
                parent[v] = u;
                q.push(v);
            }
        }
    }
    if (!found) return false;

    // rebuild path from target back to you, then reverse
    vector<string> rev;
    for (string cur = target; ; ) {
        rev.push_back(cur);
        if (cur == you) break;
        cur = parent[cur];
    }
    outPath.assign(rev.rbegin(), rev.rend());
    return true;
}

// Small helpers to keep main tidy
template <typename G>
void addTriangle(G& g, int a, int b, int c) {
    g.addVertex(a); g.addVertex(b); g.addVertex(c);
    g.addEdge(a,b); g.addEdge(b,c); g.addEdge(c,a);
}

template <typename G>
void buildSocialGraph(G& g) {
    vector<string> people = {
        "Connor Elmore","Kevin Bacon","Alice","Bob","Carol","Dave",
        "Eve","Frank","Grace","Heidi","Ivan","Judy","Mallory",
        "Niaj","Olivia","Peggy","Rupert","Sybil","Trent","Victor"
    };
    for (auto& p : people) g.addVertex(p);

    auto link = [&](const string& a, const string& b){ g.addEdge(a,b); };

    // Connor's local chain
    link("Connor Elmore","Alice");
    link("Alice","Bob");
    link("Bob","Carol");

    // bridge to another cluster
    link("Carol","Dave");
    link("Dave","Eve");

    // second cluster
    link("Frank","Grace");
    link("Grace","Heidi");
    link("Heidi","Ivan");
    link("Ivan","Judy");
    link("Judy","Mallory");

    // connect clusters
    link("Eve","Frank");

    // tail to Kevin Bacon
    link("Mallory","Niaj");
    link("Niaj","Olivia");
    link("Olivia","Peggy");
    link("Peggy","Rupert");
    link("Rupert","Sybil");
    link("Sybil","Trent");
    link("Trent","Victor");
    link("Victor","Kevin Bacon");
}

// Main: quick tests for both features
int main() {
    cout << "== Simple cycle checks ==\n";
    {
        AdjacencyListGraph<int> gl;
        AdjacencyMatrixGraph<int> gm;
        addTriangle(gl, 0,1,2);
        addTriangle(gm, 0,1,2);

        vector<int> good {0,1,2,0};
        vector<int> noReturn {0,1,2};
        vector<int> repeats {0,1,0,1,0};

        cout << "List  good? " << (isSimpleCycle(gl, good)     ? "true" : "false") << "\n";
        cout << "List  bad1? " << (isSimpleCycle(gl, noReturn)  ? "true" : "false") << "\n";
        cout << "List  bad2? " << (isSimpleCycle(gl, repeats)   ? "true" : "false") << "\n";

        cout << "Matrix good? " << (isSimpleCycle(gm, good)     ? "true" : "false") << "\n";
        cout << "Matrix bad1? " << (isSimpleCycle(gm, noReturn) ? "true" : "false") << "\n";
        cout << "Matrix bad2? " << (isSimpleCycle(gm, repeats)  ? "true" : "false") << "\n";
    }

    cout << "\n== Kevin Bacon search ==\n";
    {
        AdjacencyListGraph<string> g;   // BFS is nice on lists
        buildSocialGraph(g);

        vector<string> path;
        bool ok = pathToKevin(g, "Connor Elmore", path);
        if (ok) {
            cout << "Connected. Bacon number = " << (path.size() - 1) << "\n";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
            }
        } else {
            cout << "Not connected.\n";
        }
    }

    return 0;
}
