#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "Message.h"
#include "Player.h"
struct PlayerView {
public:
    PlayerView();
    PlayerView(Player& player);
    PlayerView(Player&& player);
    double x;
    double y;
    QColor color;
    QString player_name;
    QString client_id;
    Message player_message;
    void draw(QPainter& painter);
};

#endif // PLAYERVIEW_H
