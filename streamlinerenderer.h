#ifndef STRAMLINERENDERER_H
#define STRAMLINERENDERER_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "streamlinemapper.h"

class StreamLineRenderer
{
public:
    StreamLineRenderer();
    ~StreamLineRenderer();
    void setMapper(StreamLineMapper *streamLineMapper);
    void drawLines(QMatrix4x4 mvpMatrix);

private:
    void initOpenGLShaders();
    QVector<QVector3D> vector;

    StreamLineMapper *mapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
};

#endif // STRAMLINERENDERER_H
