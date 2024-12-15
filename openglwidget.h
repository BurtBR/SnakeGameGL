#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
    Q_OBJECT
public:
    enum class Direction{
        up,
        down,
        left,
        right
    };

private:
    const QRect _space = QRect(0, 0, 100, 100);
    QVector<QRect> _snake;
    QTimer *_timer = nullptr;
    OpenGLWidget::Direction _direction;

    void MoveSnake(qsizetype step);

public:
    OpenGLWidget(QWidget *parent);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();
    QVector<QRect> CastSnake(qsizetype size, qsizetype fatness, Direction direction, QRect space);

private slots:
    void TickTimeout();
};

#endif // OPENGLWIDGET_H
