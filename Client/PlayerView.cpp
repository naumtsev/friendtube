#include "PlayerView.h"

PlayerView::PlayerView() {
}

PlayerView::PlayerView(Player& player){
    player_name = player.player_name;
    x = player.x;
    y = player.y;
    client_id = player.client_id;
    player_message = player.player_message;
    color = QColor(player.color.h, player.color.s, player.color.l);
}

PlayerView::PlayerView(Player&& player){
    player_name = std::move(player.player_name);
    x = std::move(player.x);
    y = std::move(player.y);
    client_id = std::move(player.client_id);
    player_message = std::move(player.player_message);
    color = QColor(player.color.h, player.color.s, player.color.l);
}

void PlayerView::draw(QPainter& painter){
    QPolygon polygon({QPoint(-25 + x, -25 +  y), QPoint(25 + x, -25 + y), QPoint( 25 +  x, 25 + y), QPoint(-25 + x, 25 + y)}); //рисуем квадрат
    painter.setBrush(color);                                     //задаём цвет квадрата
    painter.drawPolygon(polygon);                                //рисуем персонажа TODO: Будем рисовать текстуры
    painter.drawText(-55 + x,25 + y,110,20,Qt::AlignCenter,player_name);   //отображение имени под персонажем + выравнивание посередине
    //http://developer.alexanderklimov.ru/android/catshop/android.graphics.canvas.php#drawtext
}
