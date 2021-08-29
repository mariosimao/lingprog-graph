#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

using namespace std;

vector<vector<string>> readCsv(string filename)
{
    vector<vector<string>> result;
    vector<string> row;
    string line, word;

    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file '" + filename + "'");
    }

    if (!file.good()) {
        throw runtime_error("Could not read file '" + filename + "'");
    }

    while (getline(file, line)) {
        stringstream ss(line);

        row.clear();
        while (getline(ss, word, ',')) {
            row.push_back(word);
        }

        result.push_back(row);
    }

    file.close();

    return result;
}

bool isFloat(string numberString) {
    istringstream iss(numberString);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}

int main(int argc, char const *argv[])
{
    try {
        vector<string> args(argv, argv + argc);
        vector<string> commands = {
            "help",
            "number",
            "vertices",
            "path",
            "diameter",
            "degree",
            "betweenness"
        };

        if (
            argc <= 1 ||
            args[1] == "help" ||
            count(commands.begin(), commands.end(), args[1]) == 0
        ) {
            cout << "Usage: graph <command> [<args>]" << endl;
            cout << endl;
            cout << "Available commands:" << endl << endl;
            cout << "\t" << "help"                              << "\t\t\t\t\t" << "Display this help page" << endl;
            cout << "\t" << "number <filename>"                 << "\t\t\t"     << "Number of edges and vertices" << endl;
            cout << "\t" << "vertices <filename>"               << "\t\t\t"     << "List all vertices" << endl;
            cout << "\t" << "path <filename> <source> <target>" << "\t"         << "Shortest path betwen source and target vertices" <<  endl;
            cout << "\t" << "diameter <filename>"               << "\t\t\t"     << "Graph diameter (longest path between all shortest paths)" << endl;
            cout << "\t" << "degree <filename>"                 << "\t\t\t"     << "Vertex with maximum degree centrality" << endl;
            cout << "\t" << "betweenness <filename>"            << "\t\t\t"     << "Vertex with maximum betweenness centrality" << endl;
            return 0;
        }

        Graph graph;
        map<string, Vertex*> vertices;

        if (argc < 3) {
            throw runtime_error("Missing argument. Use 'graph help' for usage information.");
        }

        string filename = args[2];
        vector<vector<string>> data = readCsv(filename);
        for (size_t i = 0; i < data.size(); i++) {
            vector<string> row = data[i];

            if (row.size() != 3) {
                throw runtime_error("Invalid CSV format. Number of columns must be 3.");
            }

            string vertex1Name = row[0];
            string vertex2Name = row[1];
            if (vertex1Name == "" || vertex2Name == "") {
                throw runtime_error("A vertex must not have an empty name.");
            }

            if (!isFloat(row[2])) {
                throw runtime_error("Edge weight is not a valid number.");
            }
            float weight = stof(row[2]);

            Vertex* vertex1 = new Vertex(vertex1Name);
            if (vertices.find(vertex1Name) != vertices.end()) {
                vertex1 = vertices.at(vertex1Name);
            } else {
                vertices[vertex1Name] = vertex1;
            }

            Vertex* vertex2 = new Vertex(vertex2Name);
            if (vertices.find(vertex2Name) != vertices.end()) {
                vertex2 = vertices.at(vertex2Name);
            } else {
                vertices[vertex2Name] = vertex2;
            }

            Edge* edge = new Edge(vertex1, vertex2, weight);
            graph.addEdge(edge);
        }

        string command = args[1];
        if (command == commands[1]) {
            cout << endl;
            cout << "Number of edges:    " << graph.totalEdges() << endl;
            cout << "Number of vertices: " << graph.totalVertices() << endl;
        } else if (command == commands[2]) {
            cout << endl;
            cout << "Vertices:" << endl;
            set<Vertex*> graphVertices = graph.getVertices();
            for (auto vertex: graphVertices) {
                cout << "- " << vertex->name() << endl;
            }
        } else if (command == commands[3]) {
            if (argc < 5) {
                throw runtime_error("Missing argument. Use 'graph help' for usage information.");
            }

            string sourceName = args[3];
            string targetName = args[4];

            if (vertices.find(sourceName) == vertices.end()) {
                throw runtime_error("Vertex '" + sourceName + "' is not present on the graph.");
            }
            if (vertices.find(targetName) == vertices.end()) {
                throw runtime_error("Vertex '" + targetName + "' is not present on the graph.");
            }

            Vertex* source = vertices.find(sourceName)->second;
            Vertex* target = vertices.find(targetName)->second;

            path path = graph.shortestPath(source, target);

            cout << endl;
            cout << "Shortest path between '" << sourceName << "' and '" << targetName << "'";
            cout << endl << endl;
            cout << "Distance: " << path.distance << endl;
            cout << "    Path: ";
            for (size_t i = 0; i < path.vertices.size(); i++) {
                cout << path.vertices[i]->name();
                if (i != path.vertices.size() - 1) {
                    cout << " <- ";
                }
            }
            cout << endl;
        } else if (command == commands[4]) {
            path path = graph.diameter();

            cout << endl;
            cout << "Graph diameter: longest path among the shortest paths between all vertices";
            cout << endl << endl;
            cout << "Distance: " << path.distance << endl;
            cout << "    Path: ";
            for (size_t i = 0; i < path.vertices.size(); i++) {
                cout << path.vertices[i]->name();
                if (i != path.vertices.size() - 1) {
                    cout << " <- ";
                }
            }

            cout << endl;
        } else if (command == commands[5]) {
            Vertex* vertex = graph.maximumDegreeCentrality();

            cout << endl;
            cout << "Maximum degree centrality" << endl;
            cout << "(vertex with the most number of edges)" << endl;
            cout << endl;
            cout << vertex->name();
            cout << endl;
        } else if (command == commands[6]) {
            Vertex* vertex = graph.maximumBetweennessCentrality();

            cout << endl;
            cout << "Maximum betweenness centrality" << endl;
            cout << "(vertex with highest presence in shortest paths)" << endl;
            cout << endl;
            cout << vertex->name();
            cout << endl;
        }

        return 0;
    } catch(const std::exception& e) {
        std::cerr << e.what() << endl;
        return 1;
    }
}
