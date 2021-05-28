#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H

#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QPushButton>
#include <QRect>
#include <QTime>
#include <QTimer>
#include <QVideoWidget>

#include "Player.h"
#include "PlayerView.h"
#include "Room.h"

class GraphicsBush : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsBush(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsThreeBush : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsThreeBush(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsTree : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsTree(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsTreeTwo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsTreeTwo(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsStone : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsStone(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsHouse : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsHouse(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsCafe : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsCafe(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsTaverna : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsTaverna(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsHouseMan : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsHouseMan(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsCook : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsCook(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsSecurity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsSecurity(QObject *parent = 0);
    QPainterPath shape() const override;
};

class GraphicsFood : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsFood(QObject *parent = 0);
    QPainterPath shape() const override;
};

struct rectangle {
    int xl, yl;// левый ВЕРХНИЙ край
    int xr, yr;// правый НИЖНИЙ край
};

struct Room;

class AnimationView : public QGraphicsView {
    Q_OBJECT

public:
    explicit AnimationView(Room *room_, QWidget *parent = nullptr);
    void add_players(QVector<PlayerView *> &last_frame,
                     QVector<PlayerView *> &next_frame, QString local_id);
    void display_message(PlayerView *player);
    int colliding_with_player(QVector<PlayerView *> &next_frame);
    void init_background_item();
    void add_tables(PlayerView *player);
    void delete_tables(PlayerView *player);

public slots:
    void clear_vector(QVector<PlayerView *> &last_frame, QString local_id);

private:
    int x = 1, y = 1;
    int distance = 1;
    QGraphicsTextItem *item_prefics_owner_name;
    QTimer *timer_update_scene;
    // table
    QGraphicsPixmapItem *tablet_want_eating;
    QGraphicsPixmapItem *tablet_want_sleap;
    QGraphicsPixmapItem *tablet_stop;
    QGraphicsPixmapItem *table_stop_eating;
    QGraphicsPixmapItem *tablet_fist_course_alive;
    QGraphicsPixmapItem *tablet_ay;

    // Video
    QVideoWidget *video_widget;

    // Room
    Room *room;

public:
    QGraphicsScene *scene;
    bool *move_player_or_no;
    Player *local_player;
    rectangle sleep = {20, 180, 100, 230};
    rectangle want_or_dont_eat = {1060, 180, 1190, 250};
    rectangle enter_close_to_taverna = {980, 580, 1020, 630};
    rectangle first_course_alive = {40, 580, 70, 640};
    rectangle ay = {60, 570, 90, 660};
};

#endif// ANIMATIONVIEW_H
