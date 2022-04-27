#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include <QVector>
#include <QVector3D>

class FlowDataSource
{
public:
    FlowDataSource();
    ~FlowDataSource();
    void createData();
    void timeStep(int steps);
    float getDataValue(int iz, int iy, int ix, int ic);
    QVector3D getDataVector(int iz, int iy, int ix);
    void printValuesOfHorizontalSlice(int iz);
    static const int X = 16;
    static const int Y = 16;
    static const int Z = 16;
    static const int C = 3;

private:
    int time;
    float * cartesianDataGrid;
};

#endif // FLOWDATASOURCE_H
