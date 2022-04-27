#include "streamlinemapper.h"
#include <iostream>
#include <math.h>

StreamLineMapper::StreamLineMapper()
{
    generateSeeds();
}

StreamLineMapper::~StreamLineMapper()
{
    Evector.clear();
    RKvector.clear();
    seeds.clear();
}

void StreamLineMapper::setDataSource(FlowDataSource *flowDataSource)
{
    dataSource = flowDataSource;
    makeStreamLinesEuler();
    makeStreamLinesRungeKutta();
}

void StreamLineMapper::makeStreamLinesEuler()
{
    Evector.clear();
    QVector3D v1, v2;

    // Calculate streamlines from seeds.
    for(QVector3D vec: seeds)
    {
        v1 = vec;
        for(int i=0; i < slSegments; i++)
        {
            // Get the new vector starting at th given vector position.
            v2 = getDataVector(v1.z(), v1.y(), v1.x());
            v2 = v2*delta + v1;

            // Check if the line wold be out of bounds.
            if(0<v1.x() && v1.x()<15 && 0<v1.y() && v1.y()<15 && 0<v1.z() && v1.z()<15
            && 0<v2.x() && v2.x()<15 && 0<v2.y() && v2.y()<15 && 0<v2.z() && v2.z()<15)
            {
                Evector.append(v1/15);
                Evector.append(v2/15);
            }
            // Set the new starting point for the next vector.
            v1 = v2;
        }
    }
}

void StreamLineMapper::makeStreamLinesRungeKutta()
{
    RKvector.clear();
    QVector3D v1, v2;
    QVector3D k1, k2, k3, k4;

    // Calculate streamlines from seeds.
    for(QVector3D vec: seeds)
    {
        v1 = vec;
        for(int i=0; i < slSegments; i++)
        {
            // Get the new vector starting at th given vector position by calculating 4
            // smaller vectors to more accuratly detemine the direction of the new vector
            k1 = delta * getDataVector(v1.z(), v1.y(), v1.x());
            k2 = delta * getDataVector(v1.z()+k1.z()/2, v1.y()+k1.y()/2, v1.x()+k1.x()/2);
            k3 = delta * getDataVector(v1.z()+k2.z()/2, v1.y()+k2.y()/2, v1.x()+k2.x()/2);
            k4 = delta * getDataVector(v1.z()+k3.z(), v1.y()+k3.y(), v1.x()+k3.x());

            v2 = v1 + k1/6 + k2/3 + k3/3 + k4/6;

            // Check if the line wold be out of bounds.
            if(0<v1.x() && v1.x()<15 && 0<v1.y() && v1.y()<15 && 0<v1.z() && v1.z()<15
            && 0<v2.x() && v2.x()<15 && 0<v2.y() && v2.y()<15 && 0<v2.z() && v2.z()<15)
            {
                RKvector.append(v1/15);
                RKvector.append(v2/15);
            }
            // Set the new starting point for the next vector.
            v1 = v2;
        }
    }
}

void StreamLineMapper::recalculateStreamLines()
{
    makeStreamLinesEuler();
    makeStreamLinesRungeKutta();
}

QVector<QVector3D> StreamLineMapper::getEVector()
{
    // Return the vector calculated by the Euler method.
    return Evector;
}

QVector<QVector3D> StreamLineMapper::getRKVector()
{
    // Return the vector calculated by the RungeKutta method.
    return RKvector;
}

void StreamLineMapper::generateSeeds()
{
    QVector3D seed;

    // Generate seeds.
    for(float z= firstSeedz; z < 1; z+= nextSeedz)
    {
        for(float y= firstSeedy; y < 1; y+= nextSeedy)
        {
            for(float x= firstSeedx; x < 1; x+= nextSeedx)
            {
                seed.setX(x);
                seed.setY(y);
                seed.setZ(z);
                seeds.append(seed*15);
            }
        }
    }
}

QVector3D StreamLineMapper::getDataVector(float iz, float iy, float ix)
{
    int x1 = floor(ix);
    int y1 = floor(iy);
    int z1 = floor(iz);
    int x2 = ceil(ix);
    int y2 = ceil(iy);
    int z2 = ceil(iz);

    // Value of the funktion at the point f(z,y,x).
    QVector3D f3[2][2][2];
    f3[0][0][0] = dataSource->getDataVector(z1, y1, x1);
    f3[0][0][1] = dataSource->getDataVector(z1, y1, x2);
    f3[0][1][0] = dataSource->getDataVector(z1, y2, x1);
    f3[0][1][1] = dataSource->getDataVector(z1, y2, x2);
    f3[1][0][0] = dataSource->getDataVector(z2, y1, x1);
    f3[1][0][1] = dataSource->getDataVector(z2, y1, x2);
    f3[1][1][0] = dataSource->getDataVector(z2, y2, x1);
    f3[1][1][1] = dataSource->getDataVector(z2, y2, x2);

    // Value of the funktion at the edge between x1 and x2 f(z,y).
    QVector3D f2[2][2];
    if(x1==x2)
    {
        f2[0][0] = f3[0][0][0];
        f2[0][1] = f3[0][1][0];
        f2[1][0] = f3[1][0][0];
        f2[1][1] = f3[1][1][0];
    }
    else
    {
        f2[0][0] = (ix-x1)/(x2-x1) * (f3[0][0][1]-f3[0][0][0]) + f3[0][0][0];
        f2[0][1] = (ix-x1)/(x2-x1) * (f3[0][1][1]-f3[0][1][0]) + f3[0][1][0];
        f2[1][0] = (ix-x1)/(x2-x1) * (f3[1][0][1]-f3[1][0][0]) + f3[1][0][0];
        f2[1][1] = (ix-x1)/(x2-x1) * (f3[1][1][1]-f3[1][1][0]) + f3[1][1][0];
    }

    // Value of the funktion at the edge betwen
    // the edges between x1 and x2 f(f).
    QVector3D f1[2];
    if(y1==y2)
    {
        f1[0] = f2[0][0];
        f1[1] = f2[1][0];
    }
    else
    {
        f1[0] = (iy-y1)/(y2-y1) * (f2[0][1]-f2[0][0]) + f2[0][0];
        f1[1] = (iy-y1)/(y2-y1) * (f2[1][1]-f2[1][0]) + f2[1][0];
    }

    // Value of the funktion at the given point f(iz,iy,ix).
    QVector3D f;
    if(z1==z2)
    {
        f = f1[0];
    }
    else
    {
        f = (iz-z1)/(z2-z1) * (f1[1]-f1[0]) + f1[0];
    }

    return -f*15;
}
