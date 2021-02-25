#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "message.h"
#include "player.h"
class player_data
{
public:
    player_data();
    player_data(Player& player);
    double x;
    double y;
    QString client_id;
    Message player_message;
    QString color_square = "RED";
};

#endif // PLAYER_DATA_H
