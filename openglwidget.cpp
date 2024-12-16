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
    initializeOpenGLFunctions();

    glOrtho(_space.left(), (_space.right()+1), _space.bottom()+1, (_space.top()), 1.0, -1.0);

    InitGame();
}

void OpenGLWidget::paintGL(){

    if(_dead)
        glClearColor(0.8,0.0,0.0,1.0);
    else
        glClearColor(0.5,0.5,0.5,1.0);

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

    // Obstacles
    if(_obstacles.size()){

        glColor3f(0.1, 0.1 ,0.1);
        glBegin(GL_TRIANGLES);

        for(int i=0; i<_obstacles.size() ;i++){
            glVertex3i(_obstacles[i].left(), _obstacles[i].top(), 0);
            glVertex3i(_obstacles[i].left(), _obstacles[i].bottom()+1, 0);
            glVertex3i(_obstacles[i].right()+1, _obstacles[i].top(), 0);

            glVertex3i(_obstacles[i].left(), _obstacles[i].bottom()+1, 0);
            glVertex3i(_obstacles[i].right()+1, _obstacles[i].bottom()+1, 0);
            glVertex3i(_obstacles[i].right()+1, _obstacles[i].top(), 0);
        }

        glEnd();
    }

    // Food
    glColor3f(0.0, 0.4 ,0.0);
    glBegin(GL_TRIANGLES);

    glVertex3i(_food.left(), _food.top(), 0);
    glVertex3i(_food.left(), _food.bottom()+1, 0);
    glVertex3i(_food.right()+1, _food.top(), 0);

    glVertex3i(_food.left(), _food.bottom()+1, 0);
    glVertex3i(_food.right()+1, _food.bottom()+1, 0);
    glVertex3i(_food.right()+1, _food.top(), 0);

    glEnd();
}

void OpenGLWidget::MoveSnake(qsizetype step){

    if(!_snake.size())
        return;

    QPoint position;

    _currentmove += step;

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
    _bodyinertia[0] = _direction;
}

void OpenGLWidget::InitGame(){

    qsizetype snakefatness = 3, snakesize = 5;

    _dead = false;
    _currentmove= 0;
    _stomach.clear();

    _direction = ((Direction)QRandomGenerator::global()->bounded(0,4));
    CastSnake(snakesize, snakefatness);
    CastObjects(5, QRect(0, 0, 10, 10));
    CastFood(snakefatness);
}

bool OpenGLWidget::CollisionWithSnake(const QRect &object){

    for(int i=0; i<_snake.size() ;i++){
        if(_snake[i].intersects(object))
            return true;
    }

    return false;
}

bool OpenGLWidget::CollisionWithObjects(const QRect &object){
    for(int i=0; i<_obstacles.size() ;i++){
        if(_obstacles[i].intersects(object))
            return true;
    }

    return false;
}

bool OpenGLWidget::BitAss(){

    QRect mouth;

    switch(_direction){
    case Direction::up:
        mouth = QRect(_snake[0].topLeft(), QSize(_snake[0].width()/2, _snake[0].height()/2));
            break;
    case Direction::down:
        mouth = QRect(_snake[0].left(), _snake[0].center().y()+1, _snake[0].width(), _snake[0].height()/2);
        break;
    case Direction::left:
        mouth = QRect(_snake[0].left(), _snake[0].top(), _snake[0].width()/2, _snake[0].height());
        break;
    case Direction::right:
        mouth = QRect(_snake[0].center().x(), _snake[0].top(), _snake[0].width()/2, _snake[0].height());
        break;
    }

    for(int i=1; i<_snake.size() ;i++){
        if(_snake[i].contains(mouth))
            return true;
    }

    return false;
}

void OpenGLWidget::CastSnake(qsizetype size, qsizetype fatness){
    qsizetype posx = QRandomGenerator::global()->bounded(_space.left(), _space.right()+1);
    qsizetype posy = QRandomGenerator::global()->bounded(_space.top(), _space.bottom()+1);

    _snake.clear();
    _bodyinertia.clear();

    _snake.append(QRect(posx, posy, fatness, fatness));
    _bodyinertia.append(_direction);

    for(int i=1; i<size ;i++){

        AddPieceToBody();

        /*switch(_direction){
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

        _snake.append(QRect(posx, posy, fatness, fatness));
        _bodyinertia.append(_direction);*/
    }
}

void OpenGLWidget::CastObjects(qsizetype amount, QRect sizelimit){

    QRect newobject;
    qsizetype posx, posy, width, height;

    _obstacles.clear();

    for(qsizetype i=0; i<amount ;i++){

        do{
            posx = QRandomGenerator::global()->bounded(_space.left(), _space.right()+1);
            posy = QRandomGenerator::global()->bounded(_space.top(), _space.bottom()+1);
            width = QRandomGenerator::global()->bounded(1, sizelimit.width()+1);
            height = QRandomGenerator::global()->bounded(1, sizelimit.height()+1);

            newobject = QRect(posx, posy, width, height);
        }while(CollisionWithSnake(newobject));

        _obstacles.append(newobject);
    }
}

void OpenGLWidget::CastFood(qsizetype size){

    qsizetype posx, posy;

    do{
        posx = QRandomGenerator::global()->bounded(_space.left()+size, _space.right()+1-size);
        posy = QRandomGenerator::global()->bounded(_space.top()+size, _space.bottom()+1-size);

        _food = QRect(posx, posy, size, size);
    }while(CollisionWithSnake(_food) || CollisionWithObjects(_food));
}

void OpenGLWidget::TickTimeout(){

    if(_dead)
        return;

    MoveSnake(1);
    this->update();

    if(CollisionWithObjects(_snake[0]) || BitAss()){
        _dead = true;
        StopGame();
    }

    if(_snake[0].intersects(_food)){
        _stomach.append(QPair<qsizetype, QRect>(_currentmove+(_snake.size()*(_snake[0].width()+1)), _snake[0]));
        CastFood(_food.width());
        emit SetScore(_bodyinertia.size());
    }

    while(_stomach.size()){
        if(_stomach[0].first <= _currentmove){
            AddPieceToBody();
            _stomach.removeFirst();
        }else
            break;
    }
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
            emit GameStopped();
            return;
        }

        if(_dead)
            InitGame();

        emit SetScore(_snake.size());

        connect(_timer, &QTimer::timeout, this, &OpenGLWidget::TickTimeout);
        _timer->start(100);
    }else{
        StopGame();
    }
}

void OpenGLWidget::StopGame(){
    if(_timer){
        _timer->stop();
        delete _timer;
        _timer = nullptr;
        emit GameStopped();
    }
}

void OpenGLWidget::AddPieceToBody(){

    if(!_snake.size())
        return;

    QRect piece = _snake.last();

    qsizetype posx = piece.x(), posy = piece.y(), fatness = piece.width();

    _bodyinertia.append(_bodyinertia.last());

    switch(_bodyinertia.last()){
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

    _snake.append(QRect(posx, posy, fatness, fatness));
}
