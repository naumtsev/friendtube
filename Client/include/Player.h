#ifndef PLAYER_H
#define PLAYER_H

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonParseError>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <QTextOption>
#include <QTime>
#include <QTimer>

#include "HSL.h"
#include "Message.h"

struct player_move {
    qreal x;
    qreal y;
};

enum class AnimateState {
    Standing = 0,
    Moving,
    StateEnd,
};

struct sprite_data {
    int width = 0;
    int height = 0;
    int current_frame = 0;
    int frames = 0;
    int offset = 0;
    int border = 0;
};

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public slots:
    void no_message();
    void move();

public:
    explicit Player(const QString &player_name_ = "",
                    const QString &color_player_ =
                            ":/pics/sheets/m_DinoSprites - mortBIG.png",
                    QObject *parent = 0);

    Player(QJsonObject json_player);
    QJsonDocument to_json();

    sprite_data spriteData_from_json_to_sprite_data(QJsonObject json_sprite_data);
    QJsonObject spriteData_from_sprite_data_to_json();

    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;

    void update_movement(
            int sign, QKeyEvent *apKeyEvent);

    void chat();
    void stop();

    void left_direction();
    void right_direction();
    void download_pixmap();

    ~Player();

private slots:
    void next_frame();

public:
    QString direction = "left";
    AnimateState state{AnimateState::Standing};
    int saturation = 0;
    int did_you_eat = 0;
    const int player_max_saturation = 200;

    int current_frame = 0;
    QTimer timer_sprite;
    sprite_data spriteData;

    QVector<QPair<QPixmap, sprite_data>> Pixmaps;
    QString color_player = ":/pics/sheets/m_DinoSprites - mortBIG.png";

    QGraphicsTextItem *name = new QGraphicsTextItem;
    QGraphicsTextItem *message = new QGraphicsTextItem;

    Message player_message;
    QString client_id;

    QTimer *timer_move = new QTimer(this);
    QTimer *timer_message = new QTimer(this);

    player_move movement = {0, 0};

    int move_distance = 2;
    bool timer_state_character = false;
    QVector<std::pair<int, int>> position_movement_last_frame;
    QVector<std::pair<int, int>> position_name_movement_last_frame;
};

#endif// PLAYER_H
