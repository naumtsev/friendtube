#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "Message.h"
#include "Player.h"
class PlayerView
{
public:
    PlayerView();
    PlayerView(Player& player);
    QString player_name;
    double x;
    double y;
    QString client_id;
    Message player_message;
    QString color_square = "RED";
    void draw(QPainter& painter);
};

#endif // PLAYERVIEW_H
