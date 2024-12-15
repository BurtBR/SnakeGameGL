#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    connect(this, &MainWindow::ChangeSnakeDirection, _ui->openGLWidget, &OpenGLWidget::ChangeDirection);
}

MainWindow::~MainWindow(){
    delete _ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_W:
    case Qt::Key_Up:
        emit ChangeSnakeDirection(OpenGLWidget::Direction::up);
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        emit ChangeSnakeDirection(OpenGLWidget::Direction::left);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        emit ChangeSnakeDirection(OpenGLWidget::Direction::down);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        emit ChangeSnakeDirection(OpenGLWidget::Direction::right);
        break;
    default:
        break;
    }
}
