#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
    vector<vector<int>> adjTranspose;

public:
    Graph(int V) : V(V), adj(V), adjTranspose(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adjTranspose[v].push_back(u);
    }

    void dfs(int v, vector<bool>& visited, stack<int>& Stack) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u])
                dfs(u, visited, Stack);
        }
        Stack.push(v);
    }

    void dfsTranspose(int v, vector<bool>& visited, vector<int>& component) {
        visited[v] = true;
        component.push_back(v);
        for (int u : adjTranspose[v]) {
            if (!visited[u])
                dfsTranspose(u, visited, component);
        }
    }

    vector<vector<int>> kosaraju() {
        vector<bool> visited(V, false);
        stack<int> Stack;

        // Passo 1: DFS para preencher a pilha na ordem correta
        for (int i = 0; i < V; ++i) {
            if (!visited[i])
                dfs(i, visited, Stack);
        }

        // Passo 2: Inverter as arestas do grafo
        reverseEdges();

        // Passo 3: DFS no grafo transposto e encontrar componentes fortemente conexas
        fill(visited.begin(), visited.end(), false);
        vector<vector<int>> SCCs;
        while (!Stack.empty()) {
            int v = Stack.top();
            Stack.pop();
            if (!visited[v]) {
                vector<int> component;
                dfsTranspose(v, visited, component);
                SCCs.push_back(component);
            }
        }
        return SCCs;
    }

    void reverseEdges() {
        adjTranspose.clear();
        adjTranspose.resize(V);
        for (int v = 0; v < V; ++v) {
            for (int u : adj[v]) {
                adjTranspose[u].push_back(v);
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "Uso: " << argv[0] << " -f <arquivo>" << endl;
        return 1;
    }

    string filename;
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-f" && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (string(argv[i]) == "-h") {
            cout << "Componentes fortemente conexas\n"
                    "Para o problema de componentes fortemente conexos o algoritmo de kosaraju deve possuir os seguintes parâmetros:\n"
                    "-h : mostra o help\n"
                    "-o <arquivo> : redireciona a saida para o ‘‘arquivo’’\n"
                    "-f <arquivo> : indica o ‘‘arquivo’’ que contém o grafo de entrada\n"
                    "$ ./kosaraju -f arquivo-entrada.dat" << endl;
            return 0;
        }
    }

    if (filename.empty()) {
        cout << "Uso: " << argv[0] << " -f <arquivo>" << endl;
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo " << filename << endl;
        return 1;
    }

    int V, E;
    file >> V >> E;
    cout << "Vertices: " << V << ", Arestas: " << E << endl;

    Graph graph(V);

    for (int i = 0; i < E; ++i) {
    int u, v;
    file >> u >> v;
    cout << "Adicionando aresta: " << u << " -> " << v << endl;
    graph.addEdge(u - 1, v - 1); // Ajuste para 0-indexação
}


    file.close();

    // Encontrar componentes fortemente conexas
    vector<vector<int>> SCCs = graph.kosaraju();

    // Imprimir componentes fortemente conexas
    for (const auto& SCC : SCCs) {
        for (int i = 0; i < SCC.size(); ++i) {
            cout << SCC[i] + 1;
            if (i < SCC.size() - 1) cout << " ";
        }
        cout << endl;
    }

    return 0;
}
