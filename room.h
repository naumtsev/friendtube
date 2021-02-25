#ifndef ROOM_H
#define ROOM_H

#include "MainWindow.h"
#include "Player.h"
#include "PlayerView.h"
#include <QWidget>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(const QString& player_name = "YOUR_NAME_ROOM", const QString& color = "COLOR_RED", QWidget *parent = 0);
    void draw_scene();
    void keyPressEvent  (QKeyEvent *)       override;  // обработка нажатий клавиш
    void keyReleaseEvent(QKeyEvent *)       override;  // обработка отжатия клавиш
    void drawPlayer(PlayerView& player);
    void paintEvent(QPaintEvent *event)      override;

    ~Room();

public slots:
    void update_local_player_position();

private:
    Ui::Room          *ui;
    QGraphicsScene    *scene;
    Player            *local_player;
    QTimer            *update_draw_timer;
    int                FPS = 60;
};

#endif // ROOM_H
