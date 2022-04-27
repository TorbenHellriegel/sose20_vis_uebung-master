#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "horizontalslicemapper.h"
#include "flowdatasource.h"

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    ~HorizontalSliceRenderer();
    void setMapper(HorizontalSliceMapper *imgMapper);
    void drawImage(QMatrix4x4 mvpMatrix);
    void moveSlice(float steps);

    float Z;

private:
    void initOpenGLShaders();
    void initBorderGeometry();

    QImage image;
    HorizontalSliceMapper *mapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
};

#endif // HORIZONTALSLICERENDERER_H
