#ifndef EDGE_H
#define EDGE_H
#include "Vertex.h"

using namespace std;

class Edge
{
    private:
        Vertex* _vertex1;
        Vertex* _vertex2;
        float _weight;
    public:
        Edge(Vertex* vertex1, Vertex* vertex2, float weight);
        Vertex* vertex1();
        Vertex* vertex2();
        float getWeight();
        bool hasVertex(Vertex* vertex);
        Vertex* adjacentVertex(Vertex* vertex);
};
#endif
