#include "Player.h"


Player::Player(const QString& player_name_, const QString& color_player_, QObject *parent) :
    QObject(parent) {
    connect(timer_move, &QTimer::timeout, this, &Player::move);

    setPos(30, 30);

    download_pixmap();

    color_player = color_player_;
    name->setPlainText(player_name_);
    connect(&timer_sprite, &QTimer::timeout, this, &Player::next_frame);
    timer_sprite.start(50);
}

Player::Player(QJsonObject json_player){
    name->setPlainText(json_player["name"].toString());
    client_id =json_player["id"].toString();
    setPos(json_player["x"].toDouble(), json_player["y"].toDouble());
    player_message = from_json_to_message(json_player["message"].toObject());// передаём сообщение
    color_player = json_player["color_player"].toString();
    current_frame = json_player["current_frame"].toInt();
    direction = json_player["direction"].toString();
    spriteData = spriteData_from_json_to_sprite_data(json_player["spriteData"].toObject());          // передаём спрайт
    if(json_player["AnimateState"] == "Standing") {
        state = AnimateState::Standing;
    } else if (json_player["AnimateState"] == "Moving") {
        state = AnimateState::Moving;
    } else if (json_player["AnimateState"] == "StateEnd") {
        state = AnimateState::StateEnd;
    }
    download_pixmap();
}


// нужно менять цвет в tmp!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void Player::download_pixmap(){
    Pixmaps.clear();
    Pixmaps.reserve((int)AnimateState::StateEnd);
    QPair<QPixmap, sprite_data> tmp;

    tmp.first.load(color_player);                // standing
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 2;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);

    tmp.first.load(color_player);                // move
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 8;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);
}

void Player::keyPressEvent(QKeyEvent *apKeyEvent){
    const int FPS = 100;

    update_movement(apKeyEvent);
    if(!timer_move->isActive()){
        timer_move->start(1000/FPS); // проблема, что 2 раза вызывается, для первого клика + второго!!!
    }
    to_json();
}

void Player::keyReleaseEvent(QKeyEvent *apKeyEvent)
{
   //qDebug() << "KeyPress";
   update_movement(apKeyEvent);
   if (timer_move->isActive()) {
       timer_move->stop();
   }
}

void Player::move(){
    setPos(pos().x() + movement.x, pos().y() + movement.y);
    state = AnimateState::Moving;
    //qDebug() << "-----------!!!!!!!!!!!!!!!!------------------------------------!!!!!!!!!";
    //x += movement.x; // перемещение игрока в зависимости от нажатых кнопок
    //y += movement.y;

    // Проверка на выход за границы поля
    if(this->pos().x() < spriteData.width){
        setPos(pos().x() + movement.x, pos().y());
    }
    if(this->pos().x() > 1280 - spriteData.width){
        setPos(pos().x() - movement.x, pos().y());
    }
    if(this->pos().y() < spriteData.height){
        setPos(pos().x(), pos().y() + movement.y);
    }
    if(this->pos().y() > 720 - spriteData.height){
        setPos(pos().x(), pos().y() - movement.y);
    }
}


void Player::update_movement(QKeyEvent *apKeyEvent){
    //int apKey = apKeyEvent->key();
    switch(apKeyEvent->key())
    {
        case Qt::Key_Left:
        case Qt::Key_A:
        {
          set_left_direction();
          move();
          break;
        }
        case Qt::Key_Right:
        case Qt::Key_D:
        {
          set_right_direction();
          move();
          break;
        }
        case Qt::Key_Up:
        case Qt::Key_W:
        {
          set_up_direction();
          move();
          break;
        }
        case Qt::Key_Down:
        case Qt::Key_S:
        {
          set_down_direction();
          move();
          break;
        }
        case Qt::Key_Space:
        {
          stop();
          break;
        }
     }
    /*
    if(apKey == Qt::Key_W) {                                               //вверх
        movement.y -=  sign*move_distance;
    } else if (apKey == Qt::Key_S) {                                       //вниз
        movement.y +=  sign*move_distance;
    } else if (apKey == Qt::Key_A) {                                       //влево
        movement.x -=  sign*move_distance;
    } else if (apKey == Qt::Key_D) {                                       //вправо
        movement.x +=  sign*move_distance;
    }
    */
}


// скорее всего нужно будет сделать это в PlayerView и вызывать при добавлении игрока на поле!!!
void Player::set_left_direction(){
    if(direction == "left"){
        this->change_direction(); // скорее всего не понадобиться
    }
    movement.x = -2;
    direction = "right";
}

void Player::set_right_direction(){
    if(direction == "right"){
        this->change_direction(); // скорее всего не понадобиться
    }
    movement.x = 2;
    direction = "left";
}

void Player::set_up_direction(){
    movement.y = -2;
}

void Player::set_down_direction(){
    movement.y =  2;
}

void Player::stop(){
    state = AnimateState::Standing;
    current_frame = 0;
    movement = {0,0};
}

void Player::change_direction(){
    // Get the current position
    QRectF itemRectOld = this->sceneBoundingRect();

    // Get the current transform
    QTransform transform(this->transform());
    qreal m11 = transform.m11(); // Horizontal scaling
    qreal m12 = transform.m12(); // Vertical shearing
    qreal m13 = transform.m13(); // Horizontal Projection
    qreal m21 = transform.m21(); // Horizontal shearing
    qreal m22 = transform.m22(); // vertical scaling
    qreal m23 = transform.m23(); // Vertical Projection
    qreal m31 = transform.m31(); // Horizontal Position (DX)
    qreal m32 = transform.m32(); // Vertical Position (DY)
    qreal m33 = transform.m33(); // Addtional Projection Factor

    // Horizontal flip
    m11 = -m11;

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

    // Set the items transformation
    setTransform(transform);

    // Re-position back to origin
    QRectF itemRectNew = this->sceneBoundingRect();
    qreal dx = itemRectOld.center().x()-itemRectNew.center().x();
    qreal dy = itemRectOld.center().y()-itemRectNew.center().y();
    this->moveBy(dx, dy);

}

void Player::chat(){
    if(player_message.metka_message && !player_message.metka_message_painter){
        player_message.metka_message_painter = true;
        timer_message->stop();
        timer_message->start(5000);
        connect(timer_message, SIGNAL(timeout()), this, SLOT(no_message()));
    }
}

void Player::no_message(){
    player_message.metka_message = false;
    player_message.metka_message_painter = false;
    player_message.send_message = "";
}

QJsonDocument Player::to_json(){
    QJsonObject json_player;

    json_player.insert("name", QJsonValue::fromVariant(name->toPlainText()));
    json_player.insert("id", QJsonValue::fromVariant(client_id));
    json_player.insert("x", QJsonValue::fromVariant(this->pos().x()));
    json_player.insert("y", QJsonValue::fromVariant(this->pos().y()));
    json_player.insert("message", player_message.from_message_to_json());
    json_player.insert("current_frame", QJsonValue::fromVariant(current_frame));
    json_player.insert("direction", QJsonValue::fromVariant(direction));

    if(state == AnimateState::Standing){
        json_player.insert("AnimateState", QJsonValue::fromVariant("Standing"));
    } else if (state == AnimateState::Moving){
        json_player.insert("AnimateState", QJsonValue::fromVariant("Moving"));
    } else if (state == AnimateState::StateEnd){
        json_player.insert("AnimateState", QJsonValue::fromVariant("StateEnd"));
    }

    json_player.insert("color_player", QJsonValue::fromVariant(color_player));
    json_player.insert("spriteData", spriteData_from_sprite_data_to_json());
    QJsonDocument doc(json_player);

    //QString json_string = doc.toJson(QJsonDocument::Indented);
    //qDebug() << json_string;

    return doc;
}

QJsonObject Player::spriteData_from_sprite_data_to_json(){
    QJsonObject json_sprite_data;
    int i;
    if(state == AnimateState::Standing){
        i = 0;
    } else {
        i = 1;
    }
    json_sprite_data.insert("width", QJsonValue::fromVariant(Pixmaps[i].second.width));
    json_sprite_data.insert("height", QJsonValue::fromVariant(Pixmaps[i].second.height));
    json_sprite_data.insert("current_frame", QJsonValue::fromVariant(Pixmaps[i].second.current_frame));
    json_sprite_data.insert("frames", QJsonValue::fromVariant(Pixmaps[i].second.frames));
    json_sprite_data.insert("offset", QJsonValue::fromVariant(Pixmaps[i].second.offset));
    json_sprite_data.insert("border", QJsonValue::fromVariant(Pixmaps[i].second.border));
    return json_sprite_data;
}

sprite_data Player::spriteData_from_json_to_sprite_data(QJsonObject json_sprite_data){
    sprite_data spritedata;

    spritedata.width =  json_sprite_data["width"].toInt();
    spritedata.height = json_sprite_data["height"].toInt();
    spritedata.current_frame = json_sprite_data["current_frame"].toInt();
    spritedata.frames = json_sprite_data["frames"].toInt();
    spritedata.offset = json_sprite_data["offset"].toInt();
    spritedata.border = json_sprite_data["border"].toInt();

    return spritedata;
}

void Player::next_frame(){
    qreal Dy = 0;
    if(state == AnimateState::Standing){
        if(current_frame % 2 == 0) {
            Dy = 0.5;
        }else{
            Dy =-0.5;
        }
    }
    if(direction == "left"){
        int left_x = 24;                        // не знаю костыль ли это или нормальное решение.
        name->setPos(pos().x() + movement.x + left_x  - name->boundingRect().width()/2, pos().y() + movement.y + Dy - 15);     // чтобы не было глюков при замене direction нужно,
        //name->setPos(pos().x() + Dx + left_x, pos().y() + Dy - 10);
    }else{                                                              // чтобы left_x + right_x = 48 (размеру перснонажа)
        int right_x = 24;
        name->setPos(pos().x() + movement.y - right_x - name->boundingRect().width()/2, pos().y() + movement.y + Dy - 15); // -  boundingRect().height()/2
        //name->setPos(pos().x() + Dx - right_x, pos().y() + Dy - 10);
    }
    switch(state){
        case AnimateState::Moving : {
            move();
            current_frame++;
            if(current_frame >= Pixmaps[static_cast<int>(state)].second.frames){
                current_frame = 4;
            }
            break;
        }
        case AnimateState::Standing :
        case AnimateState::StateEnd : {
            current_frame++;
            if(current_frame >= Pixmaps[static_cast<int>(state)].second.frames){
                current_frame = 0;
            }
            break;
        }
    }
}


Player::~Player() {

}
