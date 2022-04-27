#ifndef HORIZONTALSLICEMAPPER_H
#define HORIZONTALSLICEMAPPER_H

#include <QImage>
#include <QOpenGLTexture>
#include "flowdatasource.h"

class HorizontalSliceMapper
{
public:
    HorizontalSliceMapper();
    ~HorizontalSliceMapper();
    void setDataSource(FlowDataSource *flowDataSource);
    QImage mapSliceToImage(int iz);

private:
    FlowDataSource *dataSource;
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
