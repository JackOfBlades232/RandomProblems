#include <iostream>
#include <unordered_set>
#include <string.h>

int main()
{
    int v, e;
    std::cin >> v >> e;
    // I am sorry for the VLAs!
    int *adj_matr = new int[v*v];
    memset(adj_matr, 0, v*v*sizeof(int));
    int *min_conn = new int[v];
    for (int i = 0; i < e; i++) {
        int v1, v2, w;
        std::cin >> v1 >> v2 >> w;
        v1--, v2--;
        adj_matr[v1*v + v2] = w;
        adj_matr[v2*v + v1] = w;
    }
    for (int i = 0; i < v; i++)
        min_conn[i] = INT_MAX;

    std::unordered_set<int> visited;
    visited.insert(0);
    for (int i = 1; i < v; i++) {
        if (adj_matr[0+i])
            min_conn[i] = adj_matr[0+i];
    }

    std::unordered_set<int> left;
    for (int i = 1; i < v; i++)
        left.insert(i);

    bool *visited_indicators = new bool[v];
    memset(visited_indicators, false, v*sizeof(bool));
    visited_indicators[0] = true;
    
    int weight = 0;

    while (!left.empty()) {
        int min_c = INT_MAX;
        int next_vertex = -1;
        for (std::unordered_set<int>::iterator iter = left.begin(); iter != left.end(); iter++) {
            if (min_conn[*iter] < min_c) {
                min_c = min_conn[*iter]; 
                next_vertex = *iter;
            }
        }

        left.erase(next_vertex);
        visited.insert(next_vertex);
        visited_indicators[next_vertex] = true;
        weight += min_c;

        for (int i = 0; i < v; i++) {
            int w = adj_matr[next_vertex*v + i];
            if (w && !visited_indicators[i] && w < min_conn[i])
                min_conn[i] = w;
        }

    }

    printf("%d\n", weight);
    return 0;
}
