#include <string>
#include "Vertex.h"

using namespace std;

Vertex::Vertex(string name)
{
    _name = name;
}

string Vertex::name()
{
    return _name;
}