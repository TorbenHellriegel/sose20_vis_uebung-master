#ifndef HORIZONTALCONTOURLINESRENDERER_H
#define HORIZONTALCONTOURLINESRENDERER_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "horizontalcontourlinesmapper.h"

class HorizontalContourLinesRenderer
{
public:
    HorizontalContourLinesRenderer();
    ~HorizontalContourLinesRenderer();
    void setMapper(HorizontalContourLinesMapper *lineMapper);
    void drawLines(QMatrix4x4 mvpMatrix);
    void moveSlice(float steps);
    void moveLines(float steps);

    float Z;
    float isoline;

private:
    void initOpenGLShaders();
    QVector<QVector3D> vector;

    HorizontalContourLinesMapper *mapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
};

#endif // HORIZONTALCONTOURLINESRENDERER_H
