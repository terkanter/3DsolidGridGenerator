#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QList>
#include <QObject>
#include <QDebug>
#define EPSILON 0.0001

static int lastindex = 0;

class Vertex : public QVector3D
{
private:
    int index;
public:
    Vertex();
    Vertex(float x, float y,  float z = 0, int id = 0);
    Vertex(const Vertex &v);
    Vertex(const Vertex &v, float z, int id);
    ~Vertex();

    float x() const
    {   return QVector3D::x(); }

    float y() const
    {   return QVector3D::y(); }

    float z() const
    {   return QVector3D::z(); }

    int id() const
    {   return index;   }

    void setId(int _id)
    {   index = _id;   }

    bool equal(float _x, float _y) const
    {   return (x() == _x) && (y() == _y); }

    bool equalWithEpsilon(float _x, float _y,  float _z = 0) const;
};

#endif // VERTEX_H
