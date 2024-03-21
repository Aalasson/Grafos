#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits> // for numeric_limits
#include <queue>  // for priority_queue

using namespace std;

class Graph {
    int V;
    vector<vector<pair<int, int>>> adj; // Alterado para armazenar pesos das arestas

public:
    Graph(int vertices) : V(vertices), adj(V) {}

    void addEdge(int from, int to, int weight) {
        adj[from].push_back({to, weight});
        adj[to].push_back({from, weight}); // Para grafos não direcionados
    }

    vector<pair<int, int>> primEdges(int startVertex = 0) {
        vector<bool> inMST(V, false);
        vector<int> key(V, numeric_limits<int>::max());
        vector<int> parent(V, -1);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        pq.push({0, startVertex});
        key[startVertex] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            inMST[u] = true;

            for (auto neighbor : adj[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (!inMST[v] && key[v] > weight) {
                    key[v] = weight;
                    pq.push({key[v], v});
                    parent[v] = u;
                }
            }
        }

        vector<pair<int, int>> edges;
        for (int i = 1; i < V; ++i) {
            edges.push_back({parent[i] + 1, i + 1}); // Adicionando 1 para ajustar índices para 1-indexado
        }
        return edges;
    }

    int primCost(int startVertex = 0) {
        vector<bool> inMST(V, false);
        vector<int> key(V, numeric_limits<int>::max());
        vector<int> parent(V, -1);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        pq.push({0, startVertex});
        key[startVertex] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            inMST[u] = true;

            for (auto neighbor : adj[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (!inMST[v] && key[v] > weight) {
                    key[v] = weight;
                    pq.push({key[v], v});
                    parent[v] = u;
                }
            }
        }

        int totalWeight = 0;
        for (int i = 0; i < V; ++i) {
            if (parent[i] != -1) {
                totalWeight += key[i];
            }
        }
        return totalWeight;
    }
};

int main(int argc, char *argv[]) {
    string filename;
    bool showSolution = false;
    int startVertex = 0;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-f" && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (string(argv[i]) == "-s") {
            showSolution = true;
        } else if (string(argv[i]) == "-i" && i + 1 < argc) {
            startVertex = atoi(argv[i + 1]) - 1; // Corrigindo índice 1-indexado
        } else if (string(argv[i]) == "-h") {
            cout << "-h           : mostra o help\n"
                 << "-o <arquivo> : redireciona a saida para o <arquivo>'\n"
                 << "-f <arquivo> : indica o <arquivo> que contem o grafo de entrada\n"
                 << "-s           : mostra a solucao\n"
                 << "-i           : vertice inicial (para o algoritmo de Prim)\n"
                 << endl;
            return 0;
        }
    }

    if (filename.empty()) {
        cout << "Usage: " << argv[0] << " -f <file>" << endl;
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file " << filename << endl;
        return 1;
    }

    int V, E;
    file >> V >> E;

    Graph graph(V);

    for (int i = 0; i < E; ++i) {
        int from, to, weight;
        file >> from >> to >> weight;
        graph.addEdge(from - 1, to - 1, weight); // Corrigindo índices 1-indexados
    }

    file.close();

    if (startVertex >= 0) {
        if (!showSolution) {
            int mstCost = graph.primCost(startVertex);
            cout << mstCost << endl;
        } else {
            vector<pair<int, int>> edges = graph.primEdges(startVertex);
            for (auto edge : edges) {
                cout << "(" << edge.first << "," << edge.second << ") ";
            }
            cout << endl;
        }
    }

    return 0;
}
