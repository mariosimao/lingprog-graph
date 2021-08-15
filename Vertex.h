#ifndef VERTEX_H
#define VERTEX_H
#include <string>

using namespace std;

class Vertex
{
    private:
        string _name;
    public:
        Vertex(string name);
        string name();
};
#endif