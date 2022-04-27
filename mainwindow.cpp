#include "mainwindow.h"
#include "opengldisplaywidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // The main window contains only a single widget, the OpenGL viewport.
    // For the purpose of this exercise, we implement the entire visualization
    // pipeline in this OpenGL widget. This may not be the best system
    // architecture, but it makes things sufficently simple for us.

    OpenGLDisplayWidget *openGLDisplay = new OpenGLDisplayWidget(this);

    // Request an OpenGL context format compatible with version 3.3.
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    openGLDisplay->setFormat(format);

    setCentralWidget(openGLDisplay);
    resize(1100, 800);
}


MainWindow::~MainWindow()
{
}
