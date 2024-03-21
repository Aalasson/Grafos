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
    Graph(int vertices) : V(vertices), adj(V), adjTranspose(V) {}

    void addEdge(int from, int to) {
        adj[from].push_back(to);
        adjTranspose[to].push_back(from);
    }

    void dfs(int vertex, vector<bool>& visited, stack<int>& Stack) {
        visited[vertex] = true;
        for (int neighbor : adj[vertex]) {
            if (!visited[neighbor])
                dfs(neighbor, visited, Stack);
        }
        Stack.push(vertex);
    }

    void dfsTranspose(int vertex, vector<bool>& visited, vector<int>& component) {
        visited[vertex] = true;
        component.push_back(vertex);
        for (int neighbor : adjTranspose[vertex]) {
            if (!visited[neighbor])
                dfsTranspose(neighbor, visited, component);
        }
    }

    vector<vector<int>> kosaraju() {
        vector<bool> visited(V, false);
        stack<int> Stack;

        // Step 1: Perform DFS to fill the stack with vertices in correct order
        for (int i = 0; i < V; ++i) {
            if (!visited[i])
                dfs(i, visited, Stack);
        }

        // Step 2: Reverse the edges of the graph
        reverseEdges();

        // Step 3: DFS on the transposed graph to find strongly connected components
        fill(visited.begin(), visited.end(), false);
        vector<vector<int>> SCCs;
        while (!Stack.empty()) {
            int vertex = Stack.top();
            Stack.pop();
            if (!visited[vertex]) {
                vector<int> component;
                dfsTranspose(vertex, visited, component);
                SCCs.push_back(component);
            }
        }
        return SCCs;
    }

    void reverseEdges() {
        adjTranspose.clear();
        adjTranspose.resize(V);
        for (int vertex = 0; vertex < V; ++vertex) {
            for (int neighbor : adj[vertex]) {
                adjTranspose[neighbor].push_back(vertex);
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "Usage: " << argv[0] << " -f <file>" << endl;
        return 1;
    }

    string filename;
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-f" && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (string(argv[i]) == "-h") {
            cout << "Strongly Connected Components\n"
                    "For the strongly connected components problem, the Kosaraju's algorithm requires the following parameters:\n"
                    "-h : display this help message\n"
                    "-o <file> : redirect output to 'file'\n"
                    "-f <file> : specify the input graph 'file'\n"
                    "$ ./kosaraju -f input-file.dat" << endl;
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
    //cout << "Vertices: " << V << ", Edges: " << E << endl;

    Graph graph(V);

    for (int i = 0; i < E; ++i) {
        int from, to;
        file >> from >> to;
        //cout << "Adding edge: " << from << " -> " << to << endl;
        graph.addEdge(from - 1, to - 1); // Adjusting for 0-indexing
    }

    file.close();

    // Find strongly connected components
    vector<vector<int>> SCCs = graph.kosaraju();

    // Print strongly connected components
    for (const auto& SCC : SCCs) {
        for (int i = 0; i < SCC.size(); ++i) {
            cout << SCC[i] + 1;
            if (i < SCC.size() - 1) cout << " ";
        }
        cout << endl;
    }

    return 0;
}
