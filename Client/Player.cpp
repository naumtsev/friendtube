#include "Player.h"


Player::Player(const QString& player_name_, QObject *parent) :
    QObject(parent),  player_name(player_name_) {
    // задать ориентацию (смотрит влево или вправо)
    connect(timer_move, &QTimer::timeout, this, &Player::move);
}

Player::Player(QJsonObject json_player){
    player_name = json_player["name"].toString();
    client_id =json_player["id"].toString();
    x = json_player["x"].toDouble();
    y = json_player["y"].toDouble();
    player_message = from_json_to_message(json_player["message"].toObject()); // передаём сообщение
    color = from_json_to_hsl(json_player["HSL"].toObject()); // передаём цвет
}


Player::~Player() {

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
    to_json();
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
    int size_picture = 25;
    if(this->x < size_picture){
        x += move_distance;
    }
    if(this->x > 1280 - size_picture){
        x -= move_distance;
    }
    if(this->y < size_picture){
        y += move_distance;
    }
    if(this->y > 720 - size_picture){
        y -= move_distance;
    }
}


void Player::update_movement(int sign, QKeyEvent *apKeyEvent){
    int apKey = apKeyEvent->key();

    if(apKey == Qt::Key_W) {                                               //вверх
        movement.y -=  sign*move_distance;
    } else if (apKey == Qt::Key_S) {                                       //вниз
        movement.y +=  sign*move_distance;
    } else if (apKey == Qt::Key_A) {                                       //влево
        movement.x -=  sign*move_distance;
    } else if (apKey == Qt::Key_D) {                                       //вправо
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
}

QJsonDocument Player::to_json(){
    QJsonObject json_player;
    json_player.insert("name", QJsonValue::fromVariant(player_name)); // почему-то первым message идёт - TODO: изменить
    json_player.insert("id", QJsonValue::fromVariant(client_id));
    json_player.insert("x", QJsonValue::fromVariant(this->x));
    json_player.insert("y", QJsonValue::fromVariant(this->y));
    json_player.insert("message", player_message.from_message_to_json());
    json_player.insert("HSL", color.from_hsl_to_json());

    QJsonDocument doc(json_player);
    QString json_string = doc.toJson(QJsonDocument::Indented);
    return doc;
}
