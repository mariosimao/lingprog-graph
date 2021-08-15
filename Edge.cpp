#include <string>
#include "Edge.h"
#include "Vertex.h"

using namespace std;

Edge::Edge(Vertex* vertex1, Vertex* vertex2, float weight)
{
    _vertex1 = vertex1;
    _vertex2 = vertex2;
    _weight = weight;
}

Vertex* Edge::vertex1()
{
    return _vertex1;
}

Vertex* Edge::vertex2()
{
    return _vertex2;
}

float Edge::getWeight()
{
    return _weight;
}

bool Edge::hasVertex(Vertex* vertex)
{
    if (vertex == _vertex1 || vertex == _vertex2) {
        return true;
    }

    return false;
}

Vertex* Edge::adjacentVertex(Vertex* vertex)
{
    if (!hasVertex(vertex)) {
        throw "The provided vertex is not on this edge.";
    }

    if (vertex == _vertex1) {
        return _vertex2;
    }

    return _vertex1;
}
