#ifndef PLAYER_H
#define PLAYER_H

#include "Message.h"
#include "HSL.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QTextOption>
#include <QJsonObject>
#include <QJsonParseError>

struct player_move{
    int x;
    int y;
};

class Player : public QObject {
    Q_OBJECT

public slots:
    void no_message();
    void move();

public:
    explicit Player(const QString& player_name_ = "YOUR_NAME_PLAYER", QObject *parent = 0); // записывание имени

    Player(QJsonObject json_player);
    QJsonDocument to_json();

    void keyPressEvent(QKeyEvent *)   ;  // зажимание клавиши
    void keyReleaseEvent(QKeyEvent *) ;  // отжимание клавиши

    void update_movement(int sign, QKeyEvent *apKeyEvent);          // обновление передвижения игрока
    void update_direction(QKeyEvent *apKeyEvent);                   // обновление направления
    ~Player();

public:

    QString player_name = "YOUR_PLAYER_NAME"; // имя игрока

    double x = 30;
    double y = 30;

    Message player_message;
    QString client_id;

    QTimer *timer_move = new QTimer(this);
    QTimer *timer_message = new QTimer(this);

    player_move movement = {0,0};
    const int move_distance = 3;
    bool direction = false; // направление: false - смотрит влево, true - смотрит вправо.
    HSL color;
};

#endif // PLAYER_H
