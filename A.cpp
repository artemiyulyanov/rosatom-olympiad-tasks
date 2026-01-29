#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

/**
 *
 * The asymptotic complexity of the algorithm is <b>O(|E| · log |E|)</b>
 *
 * **/

struct DisjointSet {
    int n;
    std::vector<int> parents; // 1-based
    std::vector<int> ranks; // 1-based

    DisjointSet(int n) : n(n), parents(n+1), ranks(n+1, 0) {
        std::iota(std::begin(parents), std::end(parents), 0);
    }

    int get(int x) {
        if (parents[x] != x) {
            parents[x] = get(parents[x]);
        }

        return parents[x];
    }

    void unite(int x, int y) {
        x = get(x);
        y = get(y);

        if (x == y) return;
        if (ranks[x] == ranks[y]) ranks[x]++;

        if (ranks[x] < ranks[y]) {
            parents[x] = y;
        } else if (ranks[x] > ranks[y]) {
            parents[y] = x;
        }
    }
};

std::vector<std::vector<int>> kruskalAlgorithm(DisjointSet &disjointSet, const std::vector<std::vector<int>> &edges) {
    std::vector<std::vector<int>> mst_edges{};

    for (std::vector<int> edge : edges) {
        int u = edge[0], v = edge[1];

        if (disjointSet.get(u) != disjointSet.get(v)) {
            mst_edges.push_back(edge);
            disjointSet.unite(u, v);
        }

        if (mst_edges.size() == disjointSet.n-1) {
            return mst_edges;
        }
    }

    return mst_edges;
}

int main() {
    int v, e;
    std::cin >> v >> e;

    std::vector<std::vector<int>> edges(e, std::vector<int>());

    for (int i = 0; i < e; i++) {
        int x, y;

        std::cin >> x >> y;

        edges[i].push_back(x);
        edges[i].push_back(y);
        edges[i].push_back(1);
    }

    DisjointSet disjointSet(v);
    const std::vector<std::vector<int>> mst_edges = kruskalAlgorithm(disjointSet, edges);

    std::cout << e - mst_edges.size() << std::endl;

    return 0;
}