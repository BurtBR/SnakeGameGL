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
}

void OpenGLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);

    if(_snake.size()){
        // Head
        glColor3f(0.5, 0.0 ,0.0);

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

    QPoint position;

    // Body
    for(int i=(_snake.size()-1); i>0 ;i--){

        position = _snake[i].center();

        switch(_bodyinertia[i]){
        case Direction::up:
            position.ry() -= step;
            if(position.ry() <= _space.top())
                position.ry() += _space.height();
            _snake[i].moveCenter(position);

            if((_snake[i].center().y() - _snake[i-1].center().y()) == 0){
                if((_snake[i].center().x() - _snake[i-1].center().x()) > 0)
                    _bodyinertia[i] = Direction::left;
                else
                    _bodyinertia[i] = Direction::right;
            }
            break;
        case Direction::down:
            position.ry() += step;
            if(position.ry() >= (_space.bottom()+1))
                position.ry() -= _space.height();
            _snake[i].moveCenter(position);

            if((_snake[i].center().y() - _snake[i-1].center().y()) == 0){
                if((_snake[i].center().x() - _snake[i-1].center().x()) > 0)
                    _bodyinertia[i] = Direction::left;
                else
                    _bodyinertia[i] = Direction::right;
            }
            break;
        case Direction::left:
            position.rx() -= step;
            if(position.rx() <= _space.left())
                position.rx() += _space.width();
            _snake[i].moveCenter(position);

            if((_snake[i].center().x() - _snake[i-1].center().x()) == 0){
                if((_snake[i].center().y() - _snake[i-1].center().y()) > 0)
                    _bodyinertia[i] = Direction::up;
                else
                    _bodyinertia[i] = Direction::down;
            }
            break;
        case Direction::right:
            position.rx() += step;
            if(position.rx() >= (_space.right()+1))
                position.rx() -= _space.width();
            _snake[i].moveCenter(position);

            if((_snake[i].center().x() - _snake[i-1].center().x()) == 0){
                if((_snake[i].center().y() - _snake[i-1].center().y()) > 0)
                    _bodyinertia[i] = Direction::up;
                else
                    _bodyinertia[i] = Direction::down;
            }
            break;
        default:
            break;
        }
    }

    // Head
    position = _snake[0].center();
    switch(_direction){
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
    _snake[0].moveCenter(position);
}

QVector<QRect> OpenGLWidget::CastSnake(qsizetype size, qsizetype fatness, Direction direction, QRect space){
    QVector<QRect> snake;
    qsizetype posx = QRandomGenerator::global()->bounded(space.left(), space.right()+1);
    qsizetype posy = QRandomGenerator::global()->bounded(space.top(), space.bottom()+1);

    _bodyinertia.clear();

    snake.append(QRect(posx, posy, fatness, fatness));
    _bodyinertia.append(direction);

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
        _bodyinertia.append(direction);
    }

    return snake;
}

void OpenGLWidget::TickTimeout(){

    MoveSnake(1);

    this->update();
}

void OpenGLWidget::ChangeDirection(Direction dir){

    if(_snake.size() < 2)
        return;

    switch(dir){
    case Direction::up:
    case Direction::down:
        if(abs(_snake[0].center().x()-_snake[1].center().x()) >= _snake[0].width())
            _direction = dir;
        break;
    case Direction::left:
    case Direction::right:
        if(abs(_snake[0].center().y()-_snake[1].center().y()) >= _snake[0].height())
            _direction = dir;
        break;
    }
}

void OpenGLWidget::StartGame(){
    if(!_timer){
        try{
            _timer = new QTimer;
        }catch(...){
            return;
        }

        emit SetScore(_snake.size());

        connect(_timer, &QTimer::timeout, this, &OpenGLWidget::TickTimeout);
        _timer->start(100);
    }else{
        _timer->stop();
        delete _timer;
        _timer = nullptr;
        emit GameStopped();
    }
}

