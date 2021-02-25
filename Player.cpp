#include "Player.h"


Player::Player(const QString& player_name_, const QString& color_, QObject *parent) :
    QObject(parent), QGraphicsItem(), player_name(player_name_), color_square(color_)
{
    // задать ориентацию (смотрит влево или вправо)
    connect(timer_move, &QTimer::timeout, this, &Player::move);
}

Player::Player(QJsonObject json_player){
    player_name = json_player["name"].toString();
    client_id =json_player["id"].toString();
    x = json_player["x"].toDouble();
    y = json_player["y"].toDouble();
    player_message = from_json_to_message(json_player["message"].toObject()); // Создать для message
}


Player::~Player()
{

}
QRectF Player::boundingRect() const {
    //empty
}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //empty
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
    x += movement.x; // перемещение игрока в зависимости от нажатых кнопок
    y += movement.y;

    // Проверка на выход за границы поля
    if(this->x - 10 < -350){
        x += move_distance;
    }
    if(this->x + 10 > 350){
        x -= move_distance;
    }
    if(this->y - 10 < -350){
        y += move_distance;
    }
    if(this->y + 10 > 350){
        y -= move_distance;
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
    player_message.metka_message = false;
    player_message.metka_message_painter = false;
    player_message.send_message = "";
    //update(); // это важная штучка!!!!!(наверно)
}

QJsonDocument Player::player_to_json(){
    QJsonObject json_player;
    json_player.insert("name", QJsonValue::fromVariant(player_name)); // почему-то первым message идёт - TODO: изменить
    json_player.insert("id", QJsonValue::fromVariant(client_id));
    json_player.insert("x", QJsonValue::fromVariant(this->x));
    json_player.insert("y", QJsonValue::fromVariant(this->y));
    json_player.insert("message", player_message.from_message_to_json());

    qDebug() << json_player;
    QJsonDocument doc(json_player);
    QString json_string = doc.toJson(QJsonDocument::Indented);
    QTextStream out(stdout);
    out << json_string;
    return doc;
}
