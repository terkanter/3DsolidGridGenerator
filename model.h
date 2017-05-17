#ifndef MODEL_H
#define MODEL_H
#include <QApplication>
#include <QMesh>

#include "mesh.h"

class Model
{
public:
    Model();
    ~Model();
    QString exportToTransf() const;
    void separation();

    void setStep(float);
    void setFigure(float, float);
    void setHeightParametrs(float, float, float);


private:
    float step;
    float r1, r2;
    float h1, h2, h3;

    int nElements = 0;
    int h1Steps = 0, h2Steps = 0, h3Steps = 0;
    int h1LastLevel = 0, h2LastLevel = 0, h3LastLevel = 0;

    int nVerticesInModel;
    Mesh *mesh;
    QList<Mesh*> layers;

    int nInnerCircles;
    int nCircles;
    int nRays;

    void setStep(float _step, float _widthToFill);

    void separateBase();
    void separateTop();
    void separateMiddle();

    Vertex* vertex(float, float, Polygon &, float z = 0);
    void polarQuadCirculation(float from, float to, float z = 0);
    void polarTriangleCirculation_withCenterPoint(float z = 0);


    int extrude(float height);
    void extrudeLevel(float z);

    void copyMeshToLayer(float z);
};

#endif // MODEL_H
