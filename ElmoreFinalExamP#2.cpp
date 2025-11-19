#include <vector>
#include <queue>
#include <limits>
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;

// Dijkstra: single-source shortest paths from "source".
// Assumes: undirected, weighted graph with all weights > 0.
vector<int> dijkstra(const Graph& g, int source) {
    const int INF = std::numeric_limits<int>::max();
    int n = g.numVertices();

    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    // min-heap of (distance so far, vertex)
    using State = pair<int,int>;
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // If we've already finalized u, skip
        if (visited[u]) continue;
        visited[u] = true;

        // Relax edges out of u
        for (const auto& edge : g.neighbors(u)) {
            int v = edge.first;
            int w = edge.second;      // weight(u, v)

            int alt = d + w;
            if (alt < dist[v]) {
                dist[v] = alt;
                pq.push({alt, v});
            }
        }
    }

    // dist[i] is the shortest distance from source to vertex i
    return dist;
}
