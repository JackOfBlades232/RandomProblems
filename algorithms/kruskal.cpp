#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>

struct edge_data {
    int d[3];
};

bool comp(const edge_data& e1, const edge_data& e2)
{
    return e1.d[2] < e2.d[2];
}

int main()
{
    int v, e;
    std::cin >> v >> e;
    // I am sorry for the VLAs!
    int *adj_matr = new int[v*v];
    std::vector<edge_data> edges(e);
    int *vertex_trees = new int[v];
    int *tree_weights = new int[v];

    memset(adj_matr, 0, v*v*sizeof(int));
    for (int i = 0; i < e; i++) {
        int v1, v2, w;
        std::cin >> v1 >> v2 >> w;
        v1--, v2--;
        adj_matr[v1*v + v2] = w;
        adj_matr[v2*v + v1] = w;
        edges[i].d[0] = v1;
        edges[i].d[1] = v2;
        edges[i].d[2] = -w;
    }
    std::make_heap(edges.begin(), edges.end(), comp);

    for (int i = 0; i < v; i++)
        vertex_trees[i] = i;
    memset(tree_weights, 0, v*sizeof(int));

    int weight = 0;

    for (int i = 0; i < e; i++) {
        edge_data edge = edges.front();
        std::pop_heap(edges.begin(), edges.end(), comp);
        edges.pop_back();

        int t1 = vertex_trees[edge.d[0]];
        int t2 = vertex_trees[edge.d[1]];
        if (t1 == t2)
            continue;
        tree_weights[t1] += tree_weights[t2] + (-edge.d[2]);
        if (tree_weights[t1] > weight)
            weight = tree_weights[t1];
        for (int i = 0; i < v; i++) {
            if (vertex_trees[i] == t2)
                vertex_trees[i] = t1;
        }
    }

    printf("%d\n", weight);
    return 0;
}
