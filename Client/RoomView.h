#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QGraphicsView>
#include "PlayerView.h"
#include <QTimer>
#include <QTime>
#include <QVideoWidget>

struct RoomView : public QGraphicsView {
    Q_OBJECT

public:
    explicit RoomView(QWidget *parent = 0);
    void update_players(QVector<PlayerView *> &players_, QString local_id);
    ~RoomView();

public:
     QGraphicsScene *scene;
     QVideoWidget *video_widget;
     QTimer *timer_update_scene;

};

#endif // ROOMVIEW_H
