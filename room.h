#ifndef ROOM_H
#define ROOM_H

#include "main_widget.h"
#include "player.h"
#include <QWidget>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
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
    void paintEvent(QPaintEvent *event)     override;
    void keyPressEvent  (QKeyEvent *)       override;  // обработка нажатий клавиш
    void keyReleaseEvent(QKeyEvent *)       override;  // обработка отжатия клавиш
    void update_position_local_player();
    ~Room();

private:
    Ui::Room          *ui;
    QGraphicsScene    *scene;
    Player            *player;
    QTimer            *timer;
};

#endif // ROOM_H
