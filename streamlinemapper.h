#ifndef STREAMLINEMAPPER_H
#define STREAMLINEMAPPER_H

#include <QVector>
#include <QVector3D>
#include "flowdatasource.h"

class StreamLineMapper
{
public:
    StreamLineMapper();
    ~StreamLineMapper();
    void setDataSource(FlowDataSource *flowDataSource);
    void makeStreamLinesEuler();
    void makeStreamLinesRungeKutta();
    void recalculateStreamLines();
    QVector<QVector3D> getEVector();
    QVector<QVector3D> getRKVector();

private:
    void generateSeeds();
    QVector3D getDataVector(float iz, float iy, float ix);

    // Important variables for generating the streamlines.
    float delta = 0.2;
    float slSegments = 100;
    // Important variables for generating the seeds.
    float firstSeedz = 0.1;
    float firstSeedy = 0.1;
    float firstSeedx = 0.1;
    float nextSeedz = 0.2;
    float nextSeedy = 0.4;
    float nextSeedx = 0.4;

    QVector<QVector3D> seeds;
    QVector<QVector3D> Evector;
    QVector<QVector3D> RKvector;
    FlowDataSource *dataSource;
};

#endif // STREAMLINEMAPPER_H
