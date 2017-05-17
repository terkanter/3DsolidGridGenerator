#include "mesh.h"
#include <QCoreApplication>
Mesh::Mesh()
{}

Mesh::~Mesh()
{
    while (!vertices.isEmpty())
        delete vertices.takeFirst();
    while (!polys.isEmpty())
        delete polys.takeFirst();
}

void Mesh::mirrorX()
{
    for (int i = polys.count()-1; i >= 0 ; i--)
    {
        Polygon *p = new Polygon();
        for(int j =  polys[i]->getVertices().count()-1; j >= 0; j--)
            if (abs(polys[i]->getVertices()[j]->x()) < EPSILON) //! Epsilon
                *p << polys[i]->getVertices()[j];
            else
            {
                Vertex *vExisted = findVertex(Vertex(-polys[i]->getVertices()[j]->x(), polys[i]->getVertices()[j]->y()));
                if (vExisted != NULL)
                    *p << vExisted;
                else {
                    Vertex *v = new Vertex(-polys[i]->getVertices()[j]->x(), polys[i]->getVertices()[j]->y());
                    *p << v;
                    *this << v;
                }
            }
        *this << p;
    }
}

void Mesh::mirrorY()
{
    for (int i = polys.count()-1; i >= 0 ; i--)
    {
        Polygon *p = new Polygon();
        for(int j =  polys[i]->getVertices().count()-1; j >= 0; j--)
            if (abs(polys[i]->getVertices()[j]->y()) < EPSILON)
                *p << polys[i]->getVertices()[j];
            else
            {
                Vertex *vExisted = findVertex(Vertex(polys[i]->getVertices()[j]->x(), -polys[i]->getVertices()[j]->y()));
                if (vExisted != NULL)
                    *p << vExisted;
                else
                {
                    Vertex *v = new Vertex(polys[i]->getVertices()[j]->x(), -polys[i]->getVertices()[j]->y());
                    *p << v;
                    *this << v;
                }
            }
        *this << p;
    }
}

Vertex* Mesh::findVertex(const Vertex &v) const
{
    for (int i=0; i < vertices.count(); i++){
        // qDebug() << "search " << vertices[i]->x() << " =? " << v.x() << " | "
        //        << vertices[i]->y() << " =? " << v.y();
        if (vertices[i]->equalWithEpsilon(v.x(), v.y(), v.z()))
        {
            return vertices[i];
        }
    }
    return NULL;
}

void Mesh::exportVertices(QTextStream  *out)
{
    if (vertices.empty()) return;
    for(int i = 0; i < vertices.count(); i++)
    {
        *out << QString("")
            << QVariant((float)vertices[i]->x()).toString() << QString(" ")
            << QVariant((float)vertices[i]->y()).toString() << QString(" ")
            << QVariant((float)vertices[i]->z()).toString()
            << "\r\n";
    }
}

void Mesh::exportPolys(QTextStream  *out)
{
    if (polys.empty()) return;
    for (int i=0; i < polys.count(); i++)
    {
        *out << QString("\r\nf");
        for(int j = 0; j < polys[i]->getVertices().count(); j++)
        {
            *out << QString(" ") << QVariant(polys[i]->getVertices()[j]->id()).toString() ;
        }
    }
}
