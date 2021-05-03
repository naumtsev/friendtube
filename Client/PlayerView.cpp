#include "PlayerView.h"

PlayerView::PlayerView(Player& player){
    name->setPlainText(player.name->toPlainText());
    setPos(player.pos().x(), player.pos().y());
    color_player = player.color_player;
    client_id = player.client_id;
    player_message = player.player_message;
    direction = player.direction;
    state = player.state;
    current_frame = player.current_frame;
    spriteData = player.spriteData;
    Pixmaps = player.Pixmaps;
}

PlayerView::PlayerView(Player&& player){
    name->setPlainText(player.name->toPlainText());
    setPos(player.pos().x(), player.pos().y());
    color_player = std::move(player.color_player);
    client_id = std::move(player.client_id);
    player_message = std::move(player.player_message);
    direction = std::move(player.direction);
    state = std::move(player.state);
    current_frame = std::move(player.current_frame);
    spriteData = std::move(player.spriteData);
    Pixmaps = std::move(player.Pixmaps);
}

PlayerView::PlayerView(const PlayerView& player_view){
    name = player_view.name;
    setPos(player_view.pos().x(), player_view.pos().y());
    color_player = player_view.color_player;
    client_id = player_view.client_id;
    player_message = player_view.player_message;
    direction = player_view.direction; // create enum class
    state = player_view.state;
    current_frame = player_view.current_frame;
    spriteData = player_view.spriteData;
    Pixmaps = player_view.Pixmaps;
}

void PlayerView::update_player(){
    qDebug() << "update_player3333333333333333333333333333333333333333333333333333333333333333333";
}

QRectF PlayerView::boundingRect() const{
    return QRectF(0,0,48,48);
}

void PlayerView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){ // почему-то не работает ж(
    qDebug() << "PlayerView::paint2222222222222222222222222222222222222222222222222222222222" << "\n";
    painter->drawPixmap(0,0,Pixmaps[static_cast<int>(state)].first,(csd().offset*current_frame + csd().border),0,csd().width,csd().height);
}

const sprite_data& PlayerView::csd() const {
    return Pixmaps[static_cast<int>(state)].second;
}

/*
void PlayerView::draw(QPainter& painter){
    QPolygon polygon({QPoint(-25 + x, -25 +  y), QPoint(25 + x, -25 + y), QPoint( 25 +  x, 25 + y), QPoint(-25 + x, 25 + y)}); //рисуем квадрат
    painter.setBrush(color);                                     //задаём цвет квадрата
    painter.drawPolygon(polygon);                                //рисуем персонажа TODO: Будем рисовать текстуры
    painter.drawText(-55 + x,25 + y,110,20,Qt::AlignCenter,player_name);   //отображение имени под персонажем + выравнивание посередине
    painter.drawText(-55 + x,-40 + y,110,20,Qt::AlignRight,player_message.send_message); // чтобы отображалось сообщение
    //http://developer.alexanderklimov.ru/android/catshop/android.graphics.canvas.php#drawtext
}*/

PlayerView::~PlayerView(){

}
