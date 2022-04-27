#include "opengldisplaywidget.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <iostream>
#include "math.h"
#include <QTimer>


OpenGLDisplayWidget::OpenGLDisplayWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      distanceToCamera(-8.0)
{
    setFocusPolicy(Qt::StrongFocus);
}


OpenGLDisplayWidget::~OpenGLDisplayWidget()
{
    // Clean up visualization pipeline.
    delete bboxRenderer;
    delete dataSource;
    delete hzMapper;
    delete hzRenderer;
    delete clMapper;
    delete clRenderer;
    delete slMapper;
    delete slRenderer;
    // ....
}


QString OpenGLDisplayWidget::openGLString()
{
    QString profileStr;
    switch (format().profile())
    {
    case QSurfaceFormat::NoProfile:
        profileStr = "no profile";
        break;
    case QSurfaceFormat::CompatibilityProfile:
        profileStr = "compatibility profile";
        break;
    case QSurfaceFormat::CoreProfile:
        profileStr = "core profile";
        break;
    }

    return QString("%1.%2 (%3)").arg(format().majorVersion())
            .arg(format().minorVersion()).arg(profileStr);
}

void OpenGLDisplayWidget::timeStep()
{
    // Increase the time variable to generate new tornado data.
    dataSource->timeStep(1);
    slMapper->recalculateStreamLines();
    update();
    QTimer::singleShot(10, this, SLOT(timeStep()));
}


void OpenGLDisplayWidget::initializeGL()
{
    // Query and display some information about the used OpenGL context.
    std::cout << "Initializing OpenGLDisplayWidget with OpenGL version "
              <<  openGLString().toStdString() << ".\n" << std::flush;

    // Set the backgound color of the OpenGL display enable the depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0, 0, 0, 1);
    f->glEnable(GL_DEPTH_TEST);

    // Our own initialization of the visualization pipeline.
    initVisualizationPipeline();
}


void OpenGLDisplayWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio of the current viewport.
    float aspectRatio = float(w) / std::max(1, h);

    // Reset projection and set new perspective projection.
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0, aspectRatio, 0.05, 25.0);

    // Update model-view-projection matrix with new projection.
    updateMVPMatrix();
}


void OpenGLDisplayWidget::paintGL()
{
    // Clear color and depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call renderer modules.
    bboxRenderer->drawBoundingBox(mvpMatrix);
    hzRenderer->drawImage(mvpMatrix);
    clRenderer->drawLines(mvpMatrix);
    slRenderer->drawLines(mvpMatrix);
    // ....
}


void OpenGLDisplayWidget::mousePressEvent(QMouseEvent *e)
{
    // Save the current position of the mouse pointer for subsequent use
    // in mouseMoveEvent().
    lastMousePosition = QVector2D(e->localPos());
}


void OpenGLDisplayWidget::mouseMoveEvent(QMouseEvent *e)
{
    // If the user holds the left mouse button while moving the mouse, update
    // the rotation angles that specify from which side the grid visualization
    // is viewed.
    if (e->buttons() & Qt::LeftButton)
    {
        // Vector that points from the last stored position of the mouse
        // pointer to the current position.
        QVector2D mousePosDifference = QVector2D(e->localPos()) - lastMousePosition;

        // Update rotation angles in x and y direction. The factor "10" is an
        // arbitrary scaling constant that controls the sensitivity of the
        // mouse.
        rotationAngles.setX(
                    fmod(rotationAngles.x() + mousePosDifference.x()/10.,
                         360.));
        rotationAngles.setY(
                    fmod(rotationAngles.y() + mousePosDifference.y()/10.,
                         360.));

        // Store current position of mouse pointer for next call to this method.
        lastMousePosition = QVector2D(e->localPos());

        // Update model-view-projection matrix with new rotation angles.
        updateMVPMatrix();

        // Redraw OpenGL.
        update();
    }
}


void OpenGLDisplayWidget::wheelEvent(QWheelEvent *e)
{
    // Update distance of the camera to the rendered visualization. The factor
    // "500" is arbitrary and controls that sensitivity of the mouse.
    distanceToCamera += e->delta() / 500.;

    // Update model-view-projection matrix with new distance to camera.
    updateMVPMatrix();

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::keyPressEvent(QKeyEvent *e)
{
    // Check which key was pressed.
    if (e->key() == Qt::Key_Up)
    {
        // Check if Z would go out of bounds then either move the Slice up
        // or give out an Error message and abort.
        if(hzRenderer->Z<15)
        {
            hzRenderer->moveSlice(1);
            clRenderer->moveSlice(1);
        }
        else
        {
            std::cout << "Hoher geht nicht" << "\n";
            return;
        }
    }
    else if (e->key() == Qt::Key_Down)
    {
        // Check if Z would go out of bounds then either move the Slice down
        // or give out an Error message and abort.
        if(hzRenderer->Z>0)
        {
            hzRenderer->moveSlice(-1);
            clRenderer->moveSlice(-1);
        }
        else
        {
            std::cout << "Tiefer geht nicht" << "\n";
            return;
        }
    }
    else if (e->key() == Qt::Key_Left)
    {
        // Check if isoline value would go out of bounds then either
        // decrease the value or give out an Error message and abort.
        if(clRenderer->isoline>-0.3)
        {
            clRenderer->moveLines(-0.01);
        }
        else
        {
            std::cout << "Weiter geht die Isolinie nicht" << "\n";
            return;
        }
    }
    else if (e->key() == Qt::Key_Right)
    {
        // Check if isoline value would go out of bounds then either
        // increase the value or give out an Error message and abort.
        if(clRenderer->isoline<0.3)
        {
            clRenderer->moveLines(0.01);
        }
        else
        {
            std::cout << "Weiter geht die Isolinie nicht" << "\n";
            return;
        }
    }
    else if (e->key() == Qt::Key_Space)
    {
        // Start the animation loop.
        QTimer::singleShot(0, this, SLOT(timeStep()));
    }
    else
    {
        return;
    }

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::updateMVPMatrix()
{
    // Calculate a simple model view transformation from rotation angles
    // and distance to camera.
    // NOTE: Read from bottom to top.

    QMatrix4x4 mvMatrix;
    mvMatrix.translate(0.0, 0.0, distanceToCamera);
    mvMatrix.rotate(rotationAngles.y(), QVector3D(1.0, 0.0, 0.0));
    mvMatrix.rotate(rotationAngles.x(), QVector3D(0.0, 1.0, 0.0));
    mvMatrix.translate(-1.0, -1.0, -1.0);
    mvMatrix.scale(2.0);

    mvpMatrix = projectionMatrix * mvMatrix;
}


void OpenGLDisplayWidget::initVisualizationPipeline()
{
    // Initialize the visualization pipeline:

    // Initialize data source(s).
    dataSource = new FlowDataSource();
    dataSource->createData();
    // ....

    // Initialize mapper modules.
    hzMapper = new HorizontalSliceMapper;
    hzMapper->setDataSource(dataSource);
    clMapper = new HorizontalContourLinesMapper;
    clMapper->setDataSource(dataSource);
    slMapper = new StreamLineMapper;
    slMapper->setDataSource(dataSource);
    // ....

    // Initialize rendering modules.
    bboxRenderer = new DataVolumeBoundingBoxRenderer();
    hzRenderer = new HorizontalSliceRenderer;
    hzRenderer->setMapper(hzMapper);
    clRenderer = new HorizontalContourLinesRenderer;
    clRenderer->setMapper(clMapper);
    slRenderer = new StreamLineRenderer;
    slRenderer->setMapper(slMapper);
    // ....
}
