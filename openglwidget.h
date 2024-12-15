#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    unsigned int test;

public:
    OpenGLWidget(QWidget *parent);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();
};

#endif // OPENGLWIDGET_H
