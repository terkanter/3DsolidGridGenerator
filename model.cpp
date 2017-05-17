#include "model.h"
#include <math.h>
#define PI 3.14159265

Model::Model()
{
    r1 = 7;
    r2 = 8;
    setStep(0.5);
    setHeightParametrs(1, 6, 3);
}

Model::~Model()
{
    delete mesh;
    while (!layers.isEmpty())
        delete layers.takeFirst();
}

void Model::setStep(float _step)
{
    step = _step;
}

void Model::setStep(float _step, float _widthToFill)
{
    float s = (int)(_widthToFill/ _step);
    if(s < 1) {
        step = _widthToFill;
        return;
    }
    float so = fmod (_widthToFill, _step);
    step = _step*(1 + so/s/_step);
}

void Model::setFigure(float _r1, float _r2)
{
    if (r1 < 0 || r2 <0) return;
    if (_r1 < _r2) {
        r1 = _r1;
        r2 = _r2;
    }
    else {
        r2 = _r1;
        r1 = _r2;
    }
}

void Model::setHeightParametrs(float _h1, float _h2, float _h3)
{
    h1 = _h1;
    h2 = _h2;
    h3 = _h3;
}

void Model::separation()
{
    nVerticesInModel=0;
    float reStep = step;

    setStep(reStep, r1);
    nInnerCircles = (int)(r1/step)+1;

    setStep(reStep, r2-r1);
    nCircles = (int)((r2 - r1)/step) + 1;

    setStep(reStep, (r1+r2)/2);
    nRays = (int)((PI*(r2+r1)/4)/(step)); //! 2*Pi*r /2 (centerRadius) /4 (1/4 Circle)
    if (nRays%2 != 0) nRays++; // что бы получались четырехугольники

    step = reStep;
    separateBase();

    step = reStep;
    separateMiddle();

    step = reStep;
    separateTop();

    step = reStep;
}

void Model::separateBase()
{
    mesh = new Mesh();
    float reStep = step;

    //!inner part
    //!
    setStep(reStep, r1);
    qDebug() << "step" << step;

    //! mking triangles in the center
    //polarTriangleCirculation_withCenterPoint();
    qDebug() << "step" << step;

    //! Next part < less then r1
    polarQuadCirculation(step, r1);
    qDebug() << "step" << step;

    //! outer part r1 -> r2
    setStep(reStep, r2-r1);
    polarQuadCirculation(r1, r2);
    qDebug() << "step" << step;

    layers << mesh;
    nVerticesInModel += mesh->vertices.count();
    h1LastLevel = extrude(h1);
    h1Steps = h1LastLevel;
    nElements += layers[h1LastLevel]->getPolys().count()*h1Steps;
    step = reStep; // come back, my diar step
}

void Model::separateMiddle()
{
    float reStep = step;
    mesh = new Mesh();

    //! outer part r1 -> r2
    setStep(reStep, r2-r1);
    polarQuadCirculation(r1, r2, h1);

    copyMeshToLayer(h1);
    h2LastLevel = extrude(h2);
    h2Steps = h2LastLevel - h1Steps - 1;
    nElements += layers[h2LastLevel]->getPolys().count()*h2Steps;
    step = reStep; // come back, my diar step
}

void Model::separateTop()
{
    float reStep = step;
    mesh = new Mesh();

    //!inner part
    //!
    setStep(reStep, r1);

    //! mking triangles in the center
   // polarTriangleCirculation_withCenterPoint(h2);

    //! Next part < less then r1
    polarQuadCirculation(step, r1, h2);

    //! outer part r1 -> r2
    setStep(reStep, r2-r1);
    polarQuadCirculation(r1, r2, h2);

    copyMeshToLayer(h1+h2);
    h3LastLevel = extrude(h3);
    h3Steps = h3LastLevel - h2LastLevel-1;
    nElements += layers[h3LastLevel]->getPolys().count()*h3Steps;
    step = reStep; // come back, my diar step
}
void Model::copyMeshToLayer(float z)
{
    Mesh *nwLayer = new Mesh();
    int nPolys = mesh->getPolys().count();
    int nVertices = mesh->vertices.count();

    for (int i = 0; i < nVertices; i++)
        nwLayer->append( new Vertex(*mesh->vertices[i], z, (nVerticesInModel + i+1)));

    for (int i = 0; i < nPolys; i++)
    {
        int nVerticesInCurrentPoly = mesh->getPolys()[i]->getVertices().count();

        Polygon *p = new Polygon();
        for (int j = 0; j < nVerticesInCurrentPoly; j++)
        {
            int prevId = mesh->getPolys()[i]->getVertices()[j]->id();
            *p << nwLayer->vertices[prevId-1];
        }
        *nwLayer << p;
    }

    layers << nwLayer;
    nVerticesInModel += nVertices;
}

void Model::polarTriangleCirculation_withCenterPoint(float z)
{
    Vertex *center = new Vertex(0, 0); // Center of the Figure

    for (int j = 0; j < nRays; j+=2)
    {
        float psi = (PI/2)/(nRays);

        Polygon *poly =  new Polygon();
        *mesh << center;
        *poly << center;
        *mesh <<vertex(step, psi*(j+2), *poly, z);
        *mesh <<vertex(step, psi*(j+1), *poly, z);
        *mesh <<vertex(step, psi*j, *poly, z);
        *mesh << poly;
    }
}

void Model::polarQuadCirculation(float from, float to,  float z )
{
    for (float i = from; i < to; i+=step)
    {
        for (int j = 0; j < nRays; j++)
        {
            float psi = (PI/2)/(nRays) ;

            Polygon *poly =  new Polygon();
            *mesh <<vertex(i, psi*j, *poly, z);
            *mesh <<vertex(i, psi*(j+1), *poly, z);
            *mesh <<vertex(i+step, psi*(j+1), *poly, z);
            *mesh <<vertex(i+step, psi*j, *poly, z);
            *mesh << poly;
        }
    }
}

Vertex* Model::vertex(float radius, float psi, Polygon &p,  float z)
{
    float x = radius*cos(psi);
    float y = radius*sin(psi);

    //! check existed
    Vertex *vExisted= mesh->findVertex(Vertex(x, y, z));
   /* if (!layers.isEmpty())
        vExisted = layers[layers.count()-1]->findVertex(Vertex(x, y, z));*/
    if (vExisted != NULL) {
        p << vExisted;
        return NULL;
    }
    Vertex *nv = new Vertex(x, y, z);
    p << nv;
    return nv;
}

int Model::extrude(float height)
{
    float z = layers[layers.count()-1]->vertices[0]->z();
    float reStep = step;
    setStep(reStep, height);

    int hLastLevel = (int)(height/step);
    for (int i = 0; i < hLastLevel; i++)
        extrudeLevel(z + (i+1)*step);
    step = reStep;
    return layers.count()-1;
}

void Model::extrudeLevel(float z)
{
    Mesh *nwLayer = new Mesh();
    int nPolys = mesh->getPolys().count();
    int nVertices = mesh->vertices.count();

    for (int i = 0; i < nVertices; i++)
        nwLayer->append( new Vertex(*layers[layers.count()-1]->vertices[i], z, (nVerticesInModel + i+1)));
    //! Почему append а не << ? потому что << меняет ай ди на индекс занимаемой позиции
    //!
    for (int i = 0; i < nPolys; i++)
    {
        int nVerticesInCurrentPoly = mesh->getPolys()[i]->getVertices().count();

        Polygon *p = new Polygon();
        for (int j = 0; j < nVerticesInCurrentPoly; j++)
        {
            int prevId = mesh->getPolys()[i]->getVertices()[j]->id();
            *p << nwLayer->vertices[prevId-1];
        }
        *nwLayer << p;
    }

    layers << nwLayer;
    nVerticesInModel += nVertices;
}

QString Model::exportToTransf() const
{
    if (layers.isEmpty())
        return "empty";

    QFile file( QCoreApplication::applicationDirPath() + "/Builder/mesh.transf");
    if (!file.open(QIODevice::WriteOnly)) {
        return "Err";
    }

    //! write
    QTextStream  out(&file);
    out << nElements << " " << nVerticesInModel << "\r\n";

    qDebug() << nElements;
    qDebug()<< h1Steps << h2Steps << h3Steps;
    qDebug() << layers.count() << h1LastLevel << h2LastLevel<< h3LastLevel;

    for (int i = 1; i  <= h1LastLevel; i++)
        for (int j = 0; j < layers[i]->getPolys().count(); j++)
        {
            for (int k = 0; k < layers[i]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i-1]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            for (int k = 0; k < layers[i]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            out << "\r\n";
        }

    for (int i = h1LastLevel+2 ; i  <= h2LastLevel; i++)
        for (int j = 0; j < layers[i]->getPolys().count(); j++)
        {
            for (int k = 0; k < layers[i]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i-1]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            for (int k = 0; k < layers[i]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            out << "\r\n";
        }

    for (int i = h2LastLevel+2; i  <= h3LastLevel; i++)
        for (int j = 0; j < layers[i]->getPolys().count(); j++)
        {
            for (int k = 0; k < layers[i-1]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i-1]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            for (int k = 0; k < layers[i]->getPolys()[j]->getVertices().count(); k++)
                out << QVariant(layers[i]->getPolys()[j]->getVertices()[k]->id()).toString() << QString(" ") ;
            out << "\r\n";
        }

    out << "\r\n";

    for(int i = 0; i < layers.count(); i++)
        layers[i]->exportVertices(&out);

    file.close();
    return QString(QCoreApplication::applicationDirPath() + "/mesh.obj");
}


