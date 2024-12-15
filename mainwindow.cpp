#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);

    connect(this, &MainWindow::ChangeSnakeDirection, _ui->openGLWidget, &OpenGLWidget::ChangeDirection);
    connect(this, &MainWindow::StartGame, _ui->openGLWidget, &OpenGLWidget::StartGame);

    connect(_ui->openGLWidget, &OpenGLWidget::GameStopped, this, &MainWindow::GameStopped);
    connect(_ui->openGLWidget, &OpenGLWidget::SetScore, this, &MainWindow::SetScore);

    connect(_ui->buttonStart, &QToolButton::clicked, this, &MainWindow::On_buttonStart_Clicked);
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

void MainWindow::SetScore(int score){
    _ui->labelScore->setText("Score: " + QString::number(score));
}

void MainWindow::GameStopped(){
    _ui->buttonStart->setText("Start");
}

void MainWindow::On_buttonStart_Clicked(){
    _ui->buttonStart->setText("Stop");
    emit StartGame();
}
