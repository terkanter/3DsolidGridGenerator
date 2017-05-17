#ifndef POLYGON_H
#define POLYGON_H

#include "vertex.h"

class Polygon
{
public:
    Polygon();
    Polygon(QList<Vertex*> v);
    ~Polygon();

     QList<Vertex*> getVertices() const
    { return vertices; }

     inline Polygon &operator<< (Vertex *v)
     { vertices.append(v); return *this; }

     inline Polygon &operator<< (QList<Vertex*> v)
     { vertices.append(v); return *this; }

private:
    QList<Vertex*> vertices;
};

#endif // POLYGON_H
