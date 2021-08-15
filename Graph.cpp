#include <map>
#include <set>
#include <string>
#include <limits>
#include <iostream>
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

using namespace std;

void Graph::addEdge(Edge* edge)
{
    _edges.push_back(edge);
}

vector<Edge*> Graph::getEdges()
{
    return _edges;
}

set<Vertex*> Graph::getVertices()
{
    set<Vertex*> vertices;
    for (size_t i = 0; i < _edges.size(); i++) {
        vertices.insert(_edges[i]->vertex1());
        vertices.insert(_edges[i]->vertex2());
    }

    return vertices;
}

int Graph::totalEdges()
{
    return _edges.size();
}

int Graph::totalVertices()
{
    return getVertices().size();
}

path Graph::shortestPath(Vertex* source, Vertex* target)
{
    set<Vertex*> unvisitedVertices = getVertices();

    map<Vertex*, float> distances;
    map<Vertex*, Vertex*> previousVertex;
    map<Vertex*, vector<Vertex*>> paths;
    for (auto vertex: unvisitedVertices) {
        distances[vertex] = numeric_limits<float>::max();
    }
    distances[source] = 0;

    while (unvisitedVertices.size() != 0) {
        // Current vertex = unvisited vertex with min distance
        Vertex* currentVertex = *unvisitedVertices.begin();
        float minDistance = numeric_limits<float>::max();
        for (auto vertex: unvisitedVertices) {
            if (distances[vertex] < minDistance) {
                minDistance = distances[vertex];
                currentVertex = vertex;
            }
        }

        // Remove current vertex from unvisited vertices
        unvisitedVertices.erase(currentVertex);

        // Stop calculation if target was reached
        if (currentVertex == target) {
            break;
        }

        // Unvisited neighbors from current vertex
        for (size_t i = 0; i < _edges.size(); i++) {
            Edge* edge = _edges[i];
            if (!edge->hasVertex(currentVertex)) {
                continue;
            }

            Vertex* neighbor = edge->adjacentVertex(currentVertex);
            if (unvisitedVertices.find(neighbor) == unvisitedVertices.end()) {
                continue;
            }

            float newDistance = distances[currentVertex] + edge->getWeight();
            if (newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                previousVertex[neighbor] = currentVertex;
            }
        }
    }

    vector<Vertex*> pathVertices = { target };
    Vertex* auxVertex = target;
    while (previousVertex.find(auxVertex) != previousVertex.end()) {
        auxVertex = previousVertex.find(auxVertex)->second;
        pathVertices.push_back(auxVertex);
    }

    return path {
        pathVertices,
        distances[target]
    };
}

path Graph::diameter()
{
    path maxDistancePath;
    vector<verticesPair> pairs = _allVerticesPairs();
    for (size_t i = 0; i < pairs.size(); i++) {
        path path = shortestPath(pairs[i].vertex1, pairs[i].vertex2);

        if (!maxDistancePath.distance) {
            maxDistancePath = path;
        }

        if (path.distance > maxDistancePath.distance) {
            maxDistancePath = path;
        }
    }

    return maxDistancePath;
}

Vertex* Graph::maximumDegreeCentrality()
{
    map<Vertex*, int> edgesByVertex;

    // O(n)
    for (size_t i = 0; i < _edges.size(); i++) {
        Edge* edge = _edges[i];

        // O(1)
        auto iterator1 = edgesByVertex.find(edge->vertex1());
        auto iterator2 = edgesByVertex.find(edge->vertex2());

        if (iterator1 != edgesByVertex.end()) {
            edgesByVertex[edge->vertex1()] += 1;
        } else {
            edgesByVertex[edge->vertex1()] = 1;
        }

        if (iterator2 != edgesByVertex.end()) {
            edgesByVertex[edge->vertex2()] += 1;
        } else {
            edgesByVertex[edge->vertex2()] = 1;
        }
    }

    Vertex* maxCentralityVertex = edgesByVertex.begin()->first;

    // O(n)
    for (auto it = edgesByVertex.begin(); it != edgesByVertex.end(); it++) {
        if (it->second > edgesByVertex[maxCentralityVertex]) {
            maxCentralityVertex = it->first;
        }
    }

    return maxCentralityVertex;
}

Vertex* Graph::maximumBetweennessCentrality()
{
    map<Vertex*, int> timesOnShortestPath;
    vector<verticesPair> pairs = _allVerticesPairs();
    for (size_t i = 0; i < pairs.size(); i++) {
        path path = shortestPath(pairs[i].vertex1, pairs[i].vertex2);

        for (auto vertex: path.vertices) {
            if (timesOnShortestPath.find(vertex) == timesOnShortestPath.end()) {
                timesOnShortestPath[vertex] = 0;
            }

            timesOnShortestPath[vertex] += 1;
        }
    }

    Vertex* maxBetweennessCentrality = timesOnShortestPath.begin()->first;
    int maxTimes = timesOnShortestPath.begin()->second;
    for (auto iterator: timesOnShortestPath) {
        if (iterator.second > maxTimes) {
            maxBetweennessCentrality = iterator.first;
            maxTimes = iterator.second;
        }
    }

    return maxBetweennessCentrality;
}

vector<verticesPair> Graph::_allVerticesPairs()
{
    vector<verticesPair> pairs;

    set<Vertex*> verticesSet = getVertices();
    vector<Vertex*> vertices(verticesSet.begin(), verticesSet.end());
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = i+1; j < vertices.size(); j++) {
            verticesPair pair = verticesPair { vertices[i], vertices[j] };
            pairs.push_back(pair);
        }
    }

    return pairs;
}
