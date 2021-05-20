#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H

#include <QGraphicsView>
#include "Player.h"
#include "PlayerView.h"
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QVideoWidget>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QRect>
#include <QDateTime>


class GraphicsBush : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
public:
  explicit GraphicsBush(QObject *parent = 0);
  QPainterPath shape() const override;
};

class GraphicsTree : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
public:
  explicit GraphicsTree(QObject *parent = 0);
  QPainterPath shape() const override;
};

class GraphicsPingvin : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
public:
  explicit GraphicsPingvin(QObject *parent = 0);
  QPainterPath shape() const override;
};

class AnimationView : public QGraphicsView {
    Q_OBJECT

public:
    explicit AnimationView(QWidget *parent = 0);
    void add_players(QVector<PlayerView *> &last_frame, QVector<PlayerView *> &next_frame, QString local_id);
    void display_message(PlayerView *player);
    int colliding_with_player(QVector<PlayerView *> &next_frame);
    void init_background_item();


public slots:
    void clear_vector(QVector<PlayerView *> &last_frame, QString local_id);


private:
     QGraphicsScene *scene;
     int x = 1, y = 1;
     int distance = 1;
     QTimer *timer_update_scene;



     // Video
     QVideoWidget *video_widget;

public:
     bool *move_player_or_no;
     Player   *local_player;
     int number_room = 1;//qrand()%2;
};

#endif // ANIMATIONVIEW_H
