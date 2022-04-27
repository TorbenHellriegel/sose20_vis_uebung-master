#include "horizontalslicerenderer.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLTexture>

HorizontalSliceRenderer::HorizontalSliceRenderer()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    Z = 0;
    initOpenGLShaders();
    initBorderGeometry();
}

HorizontalSliceRenderer::~HorizontalSliceRenderer()
{
    vertexBuffer.destroy();
}

void HorizontalSliceRenderer::setMapper(HorizontalSliceMapper *imgMapper)
{
    mapper = imgMapper;
}

void HorizontalSliceRenderer::drawImage(QMatrix4x4 mvpMatrix)
{
    // Set the image to the image returned by the mapper method.
    image = mapper->mapSliceToImage(Z);

    // Create a texture from the image.
    QOpenGLTexture imgtexture(QOpenGLTexture::Target2D);
    imgtexture.create();
    imgtexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    imgtexture.setData(image);

    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Set the image texture matrix as a uniform value.
    const unsigned int textureUnit = 0;
    imgtexture.bind(textureUnit);
    shaderProgram.setUniformValue("imgtexture", textureUnit);

    // Issue OpenGL draw commands.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();
    imgtexture.destroy();
}

void HorizontalSliceRenderer::moveSlice(float steps)
{
    // Calculate the new position of the Slice after the steps.
    Z += steps;
    float z15 = Z/15;

    // Set the new texture coordinates.
    const unsigned int numVertices = 5;
    float texcoords[numVertices][3] = {
        {0, 0, z15}, {1, 0, z15}, {1, 1, z15}, {0, 1, z15},{0, 0, z15}};

    // Upload texture coordinates to the vertex buffer.
    vertexBuffer.bind();
    vertexBuffer.allocate(texcoords, numVertices * 3 * sizeof(float));
    vertexBuffer.release();
}

void HorizontalSliceRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "texture_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "texture_fshader.glsl"))
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

void HorizontalSliceRenderer::initBorderGeometry()
{
    // Vertices of a unit cube that represents the bounding box.
    const unsigned int numVertices = 5;
    float texcoords[numVertices][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},{0, 0, 0}};

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create(); // make sure to destroy in destructor!
    vertexBuffer.bind();
    vertexBuffer.allocate(texcoords, numVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("texcoords", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("texcoords");
        vertexBuffer.release();
    }
}
