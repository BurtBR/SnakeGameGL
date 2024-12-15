#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *_ui;

    void keyPressEvent(QKeyEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SetScore(int score);
    void GameStopped();

    void On_buttonStart_Clicked();

signals:
    void ChangeSnakeDirection(OpenGLWidget::Direction);
    void StartGame();
    void StopGame();
};
#endif // MAINWINDOW_H
