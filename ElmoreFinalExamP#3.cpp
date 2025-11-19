#include <vector>
#include <limits>

using Matrix = std::vector<std::vector<int>>;

// Floyd's algorithm: all-pairs shortest paths.
// W is the adjacency matrix of the graph.
// D is filled with the shortest path distances and returned by reference.
Matrix& floyd(const Matrix& W, Matrix& D) {
    const int INF = std::numeric_limits<int>::max();

    int n = static_cast<int>(W.size());
    D = W;  // copy W into D

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {

                // If either leg is "infinite", skip to avoid overflow
                if (D[i][k] == INF || D[k][j] == INF) {
                    continue;
                }

                int alt = D[i][k] + D[k][j];
                if (alt < D[i][j]) {
                    D[i][j] = alt;
                }
            }
        }
    }

    return D;
}
