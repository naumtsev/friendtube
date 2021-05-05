#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H

#include <QGraphicsView>
#include "PlayerView.h"
#include <QTimer>
#include <QTime>
class AnimationView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit AnimationView(QWidget *parent = 0);
    void add_players(QVector<PlayerView *> &last_frame, QVector<PlayerView *> &next_frame, QString local_id);

public slots:
    void clear_vector(QVector<PlayerView *> &last_frame, QString local_id);

private:
     QGraphicsScene scene;
     int x = 1, y = 1;
     int distance = 1;
     QTimer *timer_update_scene;
};

#endif // ANIMATIONVIEW_H
