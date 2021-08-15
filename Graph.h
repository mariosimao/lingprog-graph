#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <set>
#include "Edge.h"

using namespace std;

struct verticesPair {
    Vertex* vertex1;
    Vertex* vertex2;
};

struct path {
    vector<Vertex*> vertices;
    float distance;
};

class Graph
{
    private:
        vector<Edge*> _edges;
        vector<verticesPair> _allVerticesPairs();
    public:
        void addEdge(Edge* edge);
        vector<Edge*> getEdges();
        set<Vertex*> getVertices();
        int totalEdges();
        int totalVertices();
        path shortestPath(Vertex* source, Vertex* target);
        path diameter();
        Vertex* maximumDegreeCentrality();
        Vertex* maximumBetweennessCentrality();
};
#endif
