#include "openglwidget.h"


OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), QOpenGLFunctions(){
}

OpenGLWidget::~OpenGLWidget(){

}

void OpenGLWidget::initializeGL(){

    float aspect = this->width()/this->height();

    initializeOpenGLFunctions();

    glOrtho(0, 100 * aspect, 0, 100 * aspect, 1.0, -1.0);
    glClearColor(0.5,0.5,0.5,1.0);
    glPointSize(10);
}

void OpenGLWidget::paintGL(){

    test++;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0 ,0.0);
    glBegin(GL_POINTS);
    glVertex3f(test%100 , test%100, 0);
    glVertex3f(test%100, 20+test%100, 0);
    glVertex3f( 10+test%100, 10+test%100, 0);
    glEnd();


}

