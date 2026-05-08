/*
 * Practical 1: Parallel BFS and DFS using OpenMP
 * Uses an undirected graph represented as adjacency list
 */

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

const int NODES = 7; // Number of nodes in the graph

// Adjacency list representation of undirected graph
vector<int> graph[NODES];

// Add undirected edge
void addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// ─────────────────────────────────────────
// PARALLEL BFS using OpenMP
// ─────────────────────────────────────────
void parallelBFS(int start) {
    vector<bool> visited(NODES, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\nParallel BFS traversal: ";

    while (!q.empty()) {
        int size = q.size();
        vector<int> currentLevel;

        // Collect all nodes at current level
        for (int i = 0; i < size; i++) {
            int node = q.front(); q.pop();
            currentLevel.push_back(node);
            cout << node << " ";
        }

        // Process neighbors in parallel
        #pragma omp parallel for
        for (int i = 0; i < (int)currentLevel.size(); i++) {
            int node = currentLevel[i];
            for (int neighbor : graph[node]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

// ─────────────────────────────────────────
// PARALLEL DFS using OpenMP
// Uses a stack-based approach (iterative) to avoid deadlock
// Parallel threads check and mark neighbors
// ─────────────────────────────────────────
void parallelDFS(int start, vector<bool>& visited) {
    vector<int> stack;
    stack.push_back(start);
    visited[start] = true;

    while (!stack.empty()) {
        int node = stack.back();
        stack.pop_back();
        cout << node << " ";

        // Collect unvisited neighbors in parallel, then push to stack
        vector<int> toVisit;
        #pragma omp parallel for
        for (int i = 0; i < (int)graph[node].size(); i++) {
            int nb = graph[node][i];
            #pragma omp critical
            {
                if (!visited[nb]) {
                    visited[nb] = true;
                    toVisit.push_back(nb);
                }
            }
        }
        for (int nb : toVisit)
            stack.push_back(nb);
    }
}

int main() {
    // Build the undirected graph:
    //     0
    //    / \
    //   1   2
    //  / \   \
    // 3   4   5
    //          \
    //           6

    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(1, 4);
    addEdge(2, 5);
    addEdge(5, 6);

    cout << "Graph has " << NODES << " nodes (0 to 6)";
    cout << "\nStarting node: 0\n";

    // BFS
    parallelBFS(0);

    // DFS
    vector<bool> dfsVisited(NODES, false);
    cout << "Parallel DFS traversal: ";
    parallelDFS(0, dfsVisited);
    cout << endl;

    return 0;
}

/*
 * Compile: g++ -fopenmp P1_Parallel_BFS_DFS.cpp -o bfs_dfs
 * Run:     ./bfs_dfs
 */
