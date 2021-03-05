#ifndef ROOM_H
#define ROOM_H

#include "Player.h"
#include "PlayerView.h"
#include "mainwindow.h"
#include <QWidget>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
#include <QInputDialog>
#include <QThreadPool>

class MainWindow;

namespace Ui {
class Room;
}


class Room : public QWidget {
    Q_OBJECT

public:
    Room(Player *player_, QVector<PlayerView> &players_, QWidget *parent = nullptr);
    void draw_scene();
    void keyPressEvent  (QKeyEvent *)       override;  // обработка нажатий клавиш
    void keyReleaseEvent(QKeyEvent *)       override;  // обработка отжатия клавиш
    void paintEvent(QPaintEvent *event)      override;

    ~Room();

public slots:
    void update_local_player_position();

public:
    Ui::Room          *ui;
    QGraphicsScene    *scene;
    Player            *local_player;
    QTimer            *update_draw_timer;
    int                FPS = 60;
    QVector<PlayerView> players;
    MainWindow * main_window;
    bool is_got_scene = false;
    bool is_updated_data = false;

signals:
    void request_get_scene_on_the_server();
    void update_state_on_the_server(QJsonDocument);
};

#endif // ROOM_H
