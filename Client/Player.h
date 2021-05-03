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
#include <iostream>

struct player_move{
    qreal x;
    qreal y;
};

enum class AnimateState{
    Standing = 0,
    Moving,
    StateEnd
};

struct sprite_data{
    int width = 0;
    int height = 0;
    int current_frame = 0;
    int frames = 0;
    int offset = 0;
    int border = 0;
};

class Player : public QObject,  public QGraphicsPixmapItem {
    Q_OBJECT

public slots:
    void no_message();
    void move();

public:
    explicit Player(QString color_player);
    explicit Player(const QString& player_name_ = "YOUR_NAME_PLAYER", const QString& color_player = "", QObject *parent = 0); // записывание имени

    Player(QJsonObject json_player);
    QJsonDocument to_json();

    sprite_data spriteData_from_json_to_sprite_data(QJsonObject json_sprite_data);
    QJsonObject spriteData_from_sprite_data_to_json();

    void keyPressEvent(QKeyEvent *)   override;  // зажимание клавиши
    void keyReleaseEvent(QKeyEvent *) override;  // отжимание клавиши

    void update_movement(QKeyEvent *apKeyEvent);          // обновление передвижения игрока
    void update_direction(QKeyEvent *apKeyEvent);                   // обновление направления

    void chat();
    //void move();
    void stop();

    void set_left_direction();
    void set_right_direction();
    void set_up_direction();
    void set_down_direction();
    void change_direction();
    void download_pixmap();

    ~Player();


private slots:
    void next_frame();

public:
    QString direction = "left"; // create enum class
    AnimateState state{AnimateState::Standing};

    int current_frame = 0;
    QTimer timer_sprite;
    sprite_data spriteData;

    QVector<QPair<QPixmap, sprite_data>> Pixmaps;
    QString color_player = ":/pics/sheets/m_DinoSprites - douxBIG.png";

    QGraphicsTextItem *name = new QGraphicsTextItem;
    //QString player_name = "YOUR_PLAYER_NAME"; // имя игрока

    //double x = 30;
    //double y = 30;

    Message player_message;
    QString client_id;

    QTimer *timer_move = new QTimer(this);
    QTimer *timer_message = new QTimer(this);

    player_move movement = {0,0};
};

#endif // PLAYER_H
