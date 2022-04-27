#ifndef HORIZONTALCONTOURLINESMAPPER_H
#define HORIZONTALCONTOURLINESMAPPER_H

#include <QVector>
#include <QVector3D>
#include "flowdatasource.h"

class HorizontalContourLinesMapper
{
public:
    HorizontalContourLinesMapper();
    ~HorizontalContourLinesMapper();
    void setDataSource(FlowDataSource *flowDataSource);
    QVector<QVector3D> mapSliceToContourLineSegments(int iz, float iso);

private:
    float isoline;
    QVector3D isoCrossingBetweenTwoVertices(int x1, int y1, int x2, int y2, float z);

    QVector<QVector3D> vector;
    FlowDataSource *dataSource;
};

#endif // HORIZONTALSLICETOCONTOURLINEMAPPER_H
