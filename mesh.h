#ifndef MESH_H
#define MESH_H
#include <QFile>
#include "model\polygon.h"
#include <QDataStream>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    QList<Vertex*> getVertices()
    { return vertices; }

    QList<Polygon*> getPolys()
    { return polys; }

    inline Mesh &operator<< (Vertex *v)
    { if(v!=NULL) {v->setId(vertices.count()+1); vertices.append(v); return *this; }}

    inline Mesh &operator<< (Polygon *p)
    { polys.append(p); return *this; }

    void append(Vertex *v)
    { vertices.append(v); }

    void mirrorX();
    void mirrorY();

    void exportVertices(QTextStream *out);
    void exportPolys(QTextStream *out);

    Vertex* findVertex(const Vertex &v) const;

private:
    QList<Polygon*> polys;
    QList<Vertex*> vertices;

friend class Model;
};

#endif // MESH_H
