#include "openglwidget.h"

#include <QRandomGenerator>


OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), QOpenGLFunctions(){
}

OpenGLWidget::~OpenGLWidget(){

    if(_timer){
        delete _timer;
        _timer = nullptr;
    }

}

void OpenGLWidget::initializeGL(){

    qsizetype snakefatness = 3, snakesize = 5;

    initializeOpenGLFunctions();

    glOrtho(_space.left(), (_space.right()+1), _space.bottom()+1, (_space.top()), 1.0, -1.0);
    glClearColor(0.5,0.5,0.5,1.0);

    _direction = ((Direction)QRandomGenerator::global()->bounded(0,4));
    _snake = CastSnake(snakesize, snakefatness, _direction, _space);

    if(!_timer){
        try{
            _timer = new QTimer;
        }catch(...){
            return;
        }

        connect(_timer, &QTimer::timeout, this, &OpenGLWidget::TickTimeout);
        _timer->start(100);
    }
}

void OpenGLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);

    if(_snake.size()){
        // Head
        glColor3f(1.0, 0.0 ,0.0);

        glBegin(GL_TRIANGLES);
        switch(_direction){
        case Direction::right:
            if((_snake[0].x() + _snake[0].y()) % 2){
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);

                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
            }else{
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].right()+1, _snake[0].center().y()+1, 0);
            }
            break;

        case Direction::left:
            if((_snake[0].x() + _snake[0].y()) % 2){
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);

                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
            }else{
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].center().y()+1, 0);
            }
            break;

        case Direction::down:
            if((_snake[0].x() + _snake[0].y()) % 2){
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);

                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
            }else{
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].center().x()+1 , _snake[0].bottom()+1, 0);
            }
            break;

        case Direction::up:
            if((_snake[0].x() + _snake[0].y()) % 2){
                glVertex3i(_snake[0].left(), _snake[0].top(), 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);

                glVertex3i(_snake[0].right()+1, _snake[0].top(), 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
            }else{
                glVertex3i(_snake[0].left(), _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].right()+1, _snake[0].bottom()+1, 0);
                glVertex3i(_snake[0].center().x()+1 , _snake[0].top(), 0);
            }
            break;

        default:
            break;
        }

        // Body
        for(int i=1; i<_snake.size() ;i++){
            glVertex3i(_snake[i].left(), _snake[i].top(), 0);
            glVertex3i(_snake[i].left(), _snake[i].bottom()+1, 0);
            glVertex3i(_snake[i].right()+1, _snake[i].top(), 0);

            glVertex3i(_snake[i].left(), _snake[i].bottom()+1, 0);
            glVertex3i(_snake[i].right()+1, _snake[i].bottom()+1, 0);
            glVertex3i(_snake[i].right()+1, _snake[i].top(), 0);
        }
        glEnd();
    }
}

void OpenGLWidget::MoveSnake(qsizetype step){

    if(!_snake.size())
        return;

    Direction currdir = _direction;
    QPoint position;

    for(int i=0; i<_snake.size() ;i++){
        position = _snake[i].center();

        switch(currdir){
        case Direction::up:
            position.ry() -= step;
            if(position.ry() <= _space.top())
                position.ry() += _space.height();
            break;
        case Direction::down:
            position.ry() += step;
            if(position.ry() >= (_space.bottom()+1))
                position.ry() -= _space.height();
            break;
        case Direction::left:
            position.rx() -= step;
            if(position.rx() <= _space.left())
                position.rx() += _space.width();
            break;
        case Direction::right:
            position.rx() += step;
            if(position.rx() >= (_space.right()+1))
                position.rx() -= _space.width();
            break;
        default:
            break;
        }

        _snake[i].moveCenter(position);
    }
}

QVector<QRect> OpenGLWidget::CastSnake(qsizetype size, qsizetype fatness, Direction direction, QRect space){
    QVector<QRect> snake;

    qsizetype posx = QRandomGenerator::global()->bounded(space.left(), space.right()+1);
    qsizetype posy = QRandomGenerator::global()->bounded(space.top(), space.bottom()+1);

    snake.append(QRect(posx, posy, fatness, fatness));

    for(int i=1; i<size ;i++){

        switch(direction){
        case Direction::down:
            posy -= (fatness+1);
            if(posy <= _space.top())
                posy += _space.width();
            break;
        case Direction::right:
            posx -= (fatness+1);
            if(posx <= _space.left())
                posx += _space.width();
            break;
        case Direction::up:
            posy += (fatness+1);
            if(posy >= _space.bottom())
                posy -= _space.height();
            break;
        case Direction::left:
            posx += (fatness+1);
            if(posx >= (_space.right()+1))
                posx -= _space.width();
            break;
        default:
            break;
        }

        snake.append(QRect(posx, posy, fatness, fatness));
    }

    return snake;
}

void OpenGLWidget::TickTimeout(){

    MoveSnake(1);

    this->update();
}

void OpenGLWidget::ChangeDirection(Direction dir){
    _direction = dir;
}

