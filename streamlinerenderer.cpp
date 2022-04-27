#include "streamlinerenderer.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

StreamLineRenderer::StreamLineRenderer()
{
    initOpenGLShaders();
}

StreamLineRenderer::~StreamLineRenderer()
{
    vertexBuffer.destroy();
    vector.clear();
}

void StreamLineRenderer::setMapper(StreamLineMapper *streamLineMapper)
{
    mapper = streamLineMapper;
}

void StreamLineRenderer::drawLines(QMatrix4x4 mvpMatrix)
{
    // Set the vector to the vector returned by the mapper method.
    vector = mapper->getRKVector();

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vector.data(), vector.size() * 3 * sizeof(float));
    vertexBuffer.release();

    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Issue OpenGL draw commands.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(5);
    f->glDrawArrays(GL_LINES, 0, vector.size());

    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }
}

void StreamLineRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "streamline_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "streamline_fshader.glsl"))
    {
        std::cout << "Fragment shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}
