#include <iostream>
#include <algorithm>
#include "flowdatasource.h"
extern "C" {
  #include "tornadoSrc.h"
}

FlowDataSource::FlowDataSource()
{
    time = 0;
    cartesianDataGrid = new float[Z*Y*X*C];
}

FlowDataSource::~FlowDataSource()
{
    delete[] cartesianDataGrid;
}

void FlowDataSource::createData()
{
    // Generate the tornado data.
    gen_tornado(X,Y,Z,time,cartesianDataGrid);
}

void FlowDataSource::timeStep(int steps)
{
    // Generate new tornado data for the new time variable.
    time += steps;
    gen_tornado(X,Y,Z,time,cartesianDataGrid);
}

float FlowDataSource::getDataValue(int iz, int iy, int ix, int ic)
{
    // Return the data value at the specified point in a 3D space.
    return cartesianDataGrid[iz*Y*X*C+iy*X*C+ix*C+ic];
}

QVector3D FlowDataSource::getDataVector(int iz, int iy, int ix)
{
    // Return the data vector at the specified point in a 3D space.
    QVector3D vec;
    vec.setX(getDataValue(iz,iy,ix,0));
    vec.setY(getDataValue(iz,iy,ix,1));
    vec.setZ(getDataValue(iz,iy,ix,2));
    return vec;
}
