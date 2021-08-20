#ifndef MOVEMENTCHECK_H
#define MOVEMENTCHECK_H

#include <QObject>
#include "Server.h"
#include "ForbiddenZone.h"

class Server;

class MovementCheck : QObject
{
    Q_OBJECT
public:
    MovementCheck(Server *server_);
    void init_forbidden_territory();
    void check_player_position(QJsonObject &person_data);

private:
    Server *server;
    QVector<ForbiddenZone> forbidden_territory;
    QMap<QString, QPair<qreal, qreal>> current_position;
};

#endif // MOVEMENTCHECK_H
