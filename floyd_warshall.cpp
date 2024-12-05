#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <iomanip>

using namespace std;

class Graph {
public:
    int V;
    vector<vector<float>> adjMatrix;
    bool isDirected;

    Graph(int V, bool isDirected) : V(V), isDirected(isDirected) {
        adjMatrix.assign(V, vector<float>(V, numeric_limits<float>::infinity()));
        for (int i = 0; i < V; ++i) {
            adjMatrix[i][i] = 0.0f;
        }
    }

    void addEdge(int u, int v, float weight) {
        adjMatrix[u][v] = weight;
        if (!isDirected) {
            adjMatrix[v][u] = weight;
        }
    }
};

void floydWarshall(Graph &graph) {
    int V = graph.V;
    vector<vector<float>> dist = graph.adjMatrix;

    cout << "Matriz inicial de distâncias:\n";
    for (const auto &row : dist) {
        for (float value : row) {
            cout << (value == numeric_limits<float>::infinity() ? "INF" : to_string(value)) << "\t";
        }
        cout << endl;
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        cout << "Atualizando dist[" << i << "][" << j << "] ("
                             << dist[i][j] << ") -> " << dist[i][k] + dist[k][j] << endl;
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
            }
        }
    }

    cout << "\nMatriz de distâncias após Floyd-Warshall:\n";
    for (const auto &row : dist) {
        for (float value : row) {
            cout << (value == numeric_limits<float>::infinity() ? "INF" : to_string(value)) << "\t";
        }
        cout << endl;
    }
}

void readGraphFile(const string &filename, Graph &graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo.");
    }

    int u, v;
    float w;
    string line;

    getline(file, line);
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        if (iss >> u >> v >> w) {
            if (u < 0 || u >= graph.V || v < 0 || v >= graph.V) {
                throw runtime_error("Aresta com vértices fora do intervalo.");
            }
            graph.addEdge(u, v, w);
        } else {
            cerr << "Linha ignorada: " << line << endl;
        }
    }
}


int main() {
    try {
        string filename = "input.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("");
        }

        int type, numVertices;
        file >> type >> numVertices;
        if (type != 0 && type != 1) {
            throw runtime_error("Tipo de grafo inválido.");
        }
        if (numVertices <= 0) {
            throw runtime_error("Número de vértices inválido.");
        }

        Graph graph(numVertices, type == 1);
        file.close();

        readGraphFile(filename, graph);

        floydWarshall(graph);

    } catch (const exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
