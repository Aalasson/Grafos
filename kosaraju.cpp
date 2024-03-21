#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::string;
using std::vector;
using std::stack;
using std::unordered_set;
using std::ifstream;

// Classe para representar o grafo
class Graph {
 public:
  Graph(int V) : V_(V), adj_(V), adjTranspose_(V) {}

  void addEdge(int u, int v) {
    adj_[u].push_back(v);
    adjTranspose_[v].push_back(u);
  }

  // Função auxiliar para preencher a ordem de visitação
  void fillOrder(int v, unordered_set<int>& visited, stack<int>& Stack) {
    visited.insert(v);
    for (int i : adj_[v]) {
      if (!visited.count(i)) {
        fillOrder(i, visited, Stack);
      }
    }
    Stack.push(v);
  }

  // Função utilitária de busca em profundidade
  void DFSUtil(int v, unordered_set<int>& visited) {
    visited.insert(v);
    cout << v << " ";
    for (int i : adjTranspose_[v]) {
      if (!visited.count(i)) {
        DFSUtil(i, visited);
      }
    }
  }

  // Imprimir as componentes fortemente conectadas
  void printSCCs() {
    stack<int> Stack;
    unordered_set<int> visited;
    for (int i = 0; i < V_; i++) {
      if (!visited.count(i)) {
        fillOrder(i, visited, Stack);
      }
    }
    swap(adj_, adjTranspose_);
    visited.clear(); // Limpar o conjunto de visitados
    int numSCCs = 0;
    while (!Stack.empty()) {
      int v = Stack.top();
      Stack.pop();
      if (!visited.count(v)) {
        cout << "SCC #" << ++numSCCs << ": ";
        DFSUtil(v, visited);
        cout << endl;
      }
    }
  }

  // Função para adicionar arestas a partir de um arquivo
  void addEdgesFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
      cerr << "Erro ao abrir o arquivo: " << filename << endl;
      exit(1);
    }
    int u, v;
    while (file >> u >> v) {
      V_ = std::max(V_, std::max(u, v) + 1); // Atualiza o tamanho do grafo
      addEdge(u, v);
    }
    file.close();
  }

 private:
  int V_;
  vector<vector<int>> adj_;
  vector<vector<int>> adjTranspose_;
};

// Função para imprimir o menu de ajuda
void printHelp() {
  cout << "Uso: ./kosaraju [opções] -f <arquivo>" << endl;
  cout << endl;
  cout << "Opções:" << endl;
  cout << "  -h : Mostra este menu de ajuda." << endl;
  cout << "  -o <arquivo> : Redireciona a saída para o arquivo especificado." << endl;
  cout << "  -f <arquivo> : Indica o arquivo que contém o grafo de entrada." << endl;
}

// Função principal
int main(int argc, char* argv[]) {
  string inputFile = "";
  string outputFile = "";

  // Processamento de argumentos de linha de comando
  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    if (arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "-o") {
      if (i + 1 < argc) {
        outputFile = argv[i + 1];
        i++;
      } else {
        cerr << "Erro: Opção -o requer um nome de arquivo." << endl;
        return 1;
      }
    } else if (arg == "-f") {
      if (i + 1 < argc) {
        inputFile = argv[i + 1];
        i++;
      } else {
        cerr << "Erro: Opção -f requer um nome de arquivo." << endl;
        return 1;
      }
    } else {
      cerr << "Erro: Opção inválida: " << arg << endl;
      return 1;
    }
  }

  if (inputFile.empty()) {
    cerr << "Erro: Arquivo de entrada não especificado." << endl;
    return 1;
  }

  // Redirecionar a saída para o arquivo especificado, se fornecido
  if (!outputFile.empty()) {
    ofstream out(outputFile);
    if (!out) {
      cerr << "Erro ao abrir o arquivo de saída: " << outputFile << endl;
      return 1;
    }
    cout.rdbuf(out.rdbuf());
  }

  // Criar o objeto Graph e ler o grafo do arquivo
  Graph graph(0); // Tamanho do grafo será ajustado dinamicamente
  graph.addEdgesFromFile(inputFile);
  graph.printSCCs();
cout << "final!" << endl;
  return 0;
}
