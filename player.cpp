#include "player.h"


Player::Player(const QString& player_name_, const QString& color_, QObject *parent) :
    QObject(parent), QGraphicsItem(), player_name(player_name_), color_square(color_)
{
    // задать ориентацию (смотрит влево или вправо)
    connect(timer_move, &QTimer::timeout, this, &Player::move);
}


Player::~Player()
{

}

QRectF Player::boundingRect() const   // сделать отдельную структурку message
                                      // которая будет рисовать сообщение, чтобы не наслаивалось,
                                      // когда много игроков
{
    return QRectF(-55,-45,110,110);   // Ограничиваем область, в которой лежит квадрат
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        QPolygon polygon({QPoint(-25, -25), QPoint(25, -25), QPoint( 25, 25), QPoint(-25, 25)}); //рисуем квадрат
        painter->setBrush(Qt::red);                                     //задаём цвет квадрата
        painter->drawPolygon(polygon);                                  //рисуем персонажа TODO: Будем рисовать текстуры
        painter->drawText(-55,25,110,20,Qt::AlignCenter,player_name);   //отображение имени под персонажем + выравнивание посередине
        //хочу canvas.drawtext сюда впихнуть!!! чтобы можно было норм текст рисовать.
        //http://developer.alexanderklimov.ru/android/catshop/android.graphics.canvas.php#drawtext
        if(metka_message && !metka_message_painter){
            metka_message_painter = true;
            timer_message->stop();
            timer_message->start(5000);
            connect(timer_message, SIGNAL(timeout()), this, SLOT(no_message()));
        }
        if(metka_message_painter){
            painter->drawText(-55,-40,110,20,Qt::AlignRight,message);
        }
        // когда много сообщений подряд, то очень плохо - т.к. таймер не очень работает
        // TODO: сделать так, чтобы можно было подрят сообщения писать и таймер перезапускался.
        Q_UNUSED(option);
        Q_UNUSED(widget);
}

void Player::keyPressEvent(QKeyEvent *apKeyEvent){
    qDebug() << "keyPressEventPLAYE";

    const int FPS = 100;

    update_movement(1, apKeyEvent); // меняем скорость передвижения. передаем 1 т.к. произошло нажатие
    update_direction(apKeyEvent);   // меняем направления игрока
    if(!timer_move->isActive()){
        qDebug() <<"TIMER";
        timer_move->start(1000/FPS); // проблема, что 2 раза вызывается, для первого клика + второго!!!
    }
}

void Player::keyReleaseEvent(QKeyEvent *apKeyEvent)
{
   qDebug() << "keyReleaseEventPLAYER";

   update_movement(-1, apKeyEvent); // передаем -1 т.к. произошло отжатие
   if (timer_move->isActive()) {
       qDebug() <<"TIMER-STOP";
       timer_move->stop();
   }
}

void Player::move(){

    setPos(mapToParent(movement.x, movement.y)); // перемещение игрока в зависимости от нажатых кнопок

    // Проверка на выход за границы поля
    if(this->x() - 10 < -350){
        setPos(mapToParent( move_distance, 0));
    }
    if(this->x() + 10 > 350){
        setPos(mapToParent(-move_distance, 0));
    }
    if(this->y() - 10 < -350){
        setPos(mapToParent( 0, move_distance));
    }
    if(this->y() + 10 > 350){
        setPos(mapToParent( 0,-move_distance));
    }
}


void Player::update_movement(int sign, QKeyEvent *apKeyEvent){
    int apKey = apKeyEvent->key();

    if(apKey == Qt::Key_W) {                                               //вверх
        qDebug() << "W!!!";
        movement.y -=  sign*move_distance;
    } else if (apKey == Qt::Key_S) {                                       //вниз
        qDebug() << "S!!!";
        movement.y +=  sign*move_distance;
    } else if (apKey == Qt::Key_A) {                                       //влево
        qDebug() << "A!!!";
        movement.x -=  sign*move_distance;
    } else if (apKey == Qt::Key_D) {                                       //вправо
        qDebug() << "D!!!";
        movement.x +=  sign*move_distance;
    }
}

void Player::update_direction(QKeyEvent *apKeyEvent){
    if(apKeyEvent->key() == Qt::Key_A){
        direction = false;                  // пока не понятно интуитивно (TODO: переделать)
    }
    if(apKeyEvent->key() == Qt::Key_D){
        direction = true;                  // пока не понятно интуитивно (TODO: переделать)
    }
}

void Player::no_message(){
    metka_message = false;
    metka_message_painter = false;
    message = "";
    update();
}
