#include "vertex.h"

Vertex::Vertex() : QVector3D()
{
    index = ++lastindex;
}

Vertex::Vertex(float x, float y,  float z, int id )
    : QVector3D(x,y,z)
    , index(id)
{

}

Vertex::Vertex(const Vertex &v)
    : QVector3D(v.x(), v.y(), v.z())
{
    index = ++lastindex;
}

Vertex::Vertex(const Vertex &v, float z, int id)
    : QVector3D(v.x(), v.y(), z)
    , index(id)
{

}

bool Vertex::equalWithEpsilon(float _x, float _y,  float _z) const
{   return this->distanceToPoint(Vertex(_x, _y, _z)) < EPSILON; }

Vertex::~Vertex()
{
}
