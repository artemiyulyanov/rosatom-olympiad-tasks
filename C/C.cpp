#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

typedef long long ll;

/**
 *
 * Euler Tour + Segment Tree on minimums and maximums
 *
 * The asymptotic complexity of the algorithm is <b>O(N + Q·log N)</b>
 *
 * Applicable to N ≤ 10^6, Q ≤ 10^6
 *
 * **/

struct NodeTimer {
    int tin, tout;

    NodeTimer(int tin, int tout) : tin(tin), tout(tout) {}
};

struct Node {
    ll min1, min2;
    ll max1, max2;
    ll lazy;

    Node() {
        min1 = min2 = LLONG_MAX;
        max1 = max2 = LLONG_MIN;
        lazy = 0;
    }

    static const Node empty;
};

Node const Node::empty = Node();

struct SegmentTree {
    int n;
    std::vector<Node> tree;

    SegmentTree(int n) : n(n), tree(0) {
        tree.resize(4*n);
    }

    Node merge(const Node& a, const Node& b) {
        Node r;

        ll m[4] = {a.min1, a.min2, b.min1, b.min2};
        r.min1 = LLONG_MAX;
        r.min2 = LLONG_MAX;
        for (ll x : m) {
            if (x < r.min1) r.min2 = r.min1, r.min1 = x;
            else if (x < r.min2) r.min2 = x;
        }

        ll M[4] = {a.max1, a.max2, b.max1, b.max2};
        r.max1 = LLONG_MIN;
        r.max2 = LLONG_MIN;
        for (ll x : M) {
            if (x > r.max1) r.max2 = r.max1, r.max1 = x;
            else if (x > r.max2) r.max2 = x;
        }

        return r;
    }

    void build(int v, int tl, int tr, const std::vector<ll>& a) {
        if (tl == tr) {
            tree[v].min1 = tree[v].max1 = a[tl];
            tree[v].min2 = LLONG_MAX;
            tree[v].max2 = LLONG_MIN;
            return;
        }

        int tm = (tl + tr) / 2;

        build(2*v, tl, tm, a);
        build(2*v+1, tm+1, tr, a);
        tree[v] = merge(tree[2*v], tree[2*v+1]);
    }

    void apply(int v, ll add) {
        tree[v].min1 += add;

        if (tree[v].min2 != LLONG_MAX) tree[v].min2 += add;

        tree[v].max1 += add;

        if (tree[v].max2 != LLONG_MIN) tree[v].max2 += add;

        tree[v].lazy += add;
    }

    void push(int v) {
        if (tree[v].lazy != 0) {
            apply(2*v, tree[v].lazy);
            apply(2*v+1, tree[v].lazy);
            tree[v].lazy = 0;
        }
    }

    void rangeAdd(int v, int tl, int tr, int l, int r, ll d) {
        if (l > r) return;

        if (l <= tl && tr <= r) {
            apply(v, d);
            return;
        }

        push(v);

        int tm = (tl + tr) / 2;

        rangeAdd(2*v, tl, tm, l, std::min(tm, r), d);
        rangeAdd(2*v+1, tm+1, tr, std::max(tm+1, l), r, d);

        tree[v] = merge(tree[2*v], tree[2*v+1]);
    }

    void rangeAdd(int l, int r, ll d) {
        rangeAdd(1, 1, n, l, r, d);
    }

    Node query(int v, int tl, int tr, int l, int r) {
        if (l > r) return Node::empty;
        if (l == tl && r == tr) return tree[v];

        push(v);
        int tm = (tl + tr) / 2;

        return merge(
            query(2*v, tl, tm, l, std::min(r, tm)),
            query(2*v+1, tm+1, tr, std::max(l, tm+1), r)
        );
    }
};

void dfs(std::vector<std::vector<int>>& adj_list, std::vector<NodeTimer>& nodes_timing, std::vector<ll>& salaries, std::vector<ll>& flat, int& timer, int start, int parent) {
    nodes_timing[start].tin = timer;
    flat[timer] = salaries[start];
    timer++;

    for (int u : adj_list[start]) {
        if (u == parent) continue;
        dfs(adj_list, nodes_timing, salaries, flat, timer, u, start);
    }

    nodes_timing[start].tout = timer - 1;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::vector<int>> adj_list(n+1);

    for (int i = 1; i < n; i++) {
        int parent;
        std::cin >> parent;

        int child = i+1;

        adj_list[parent].push_back(child);
        adj_list[child].push_back(parent);
    }

    std::vector<ll> salaries(n+1, 0);
    for (int i = 1; i <= n; i++) {
        std::string salary;
        std::cin >> salary;

        if (i != 1) {
            salaries[i] = std::stoll(salary);
        }
    }

    std::vector<NodeTimer> nodes_timing(n+1, NodeTimer(0, 0));
    std::vector<ll> flat(n+1);

    int timer = 1;
    dfs(adj_list, nodes_timing, salaries, flat, timer, 1, -1);

    SegmentTree tree(n);
    tree.build(1, 1, n, flat);

    int q;
    std::cin >> q;

    for (int i = 0; i < q; i++) {
        int type, x;
        ll d;

        std::cin >> type >> x;
        if (type == 2) std::cin >> d;

        if (type == 1) {
            int l = nodes_timing[x].tin+1;
            int r = nodes_timing[x].tout;

            if (l >= r) {
                std::cout << 0 << '\n';
                continue;
            }

            Node res = tree.query(1, 1, n, l, r);
            std::cout << (res.max1 + res.max2) - (res.min1 + res.min2) << std::endl;
        } else {
            tree.rangeAdd(nodes_timing[x].tin, nodes_timing[x].tout, d);
        }
    }

    return 0;
}