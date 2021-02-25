#ifndef PLAYER_H
#define PLAYER_H

#include "Message.h"
#include "Hsl.h"
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
//
struct player_move{
    int x;
    int y;
};

class Player : public QObject, public QGraphicsItem
{
    Q_OBJECT

public slots:
    void no_message();
    void move();

public:
    explicit Player(const QString& player_name_ = "YOUR_NAME_PLAYER", QObject *parent = 0); // записывание имени

    Player(QJsonObject json_player);
    QJsonDocument player_to_json();

    void keyPressEvent(QKeyEvent *)      override;  // зажимание клавиши
    void keyReleaseEvent(QKeyEvent *)    override;  // отжимание клавиши

    void update_movement(int sign, QKeyEvent *apKeyEvent);          // обновление передвижения игрока
    void update_direction(QKeyEvent *apKeyEvent);                   // обновление направления
    ~Player();

protected:
    QRectF boundingRect() const override; // чекнуть про override
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

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
