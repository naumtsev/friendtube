#ifndef ROOM_H
#define ROOM_H

#include "main_widget.h"
#include "player.h"
#include <QWidget>
#include <QGLWidget>
#include <QGraphicsScene>
#include <QShortcut>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(const QString& player_name = "YOUR_NAME_ROOM", const QString& color = "COLOR_RED", QWidget *parent = 0);

    void keyPressEvent  (QKeyEvent *)       override;  // обработка нажатий клавиш
    void keyReleaseEvent(QKeyEvent *)       override;  // обработка отжатия клавиш

    ~Room();

private:
    Ui::Room          *ui;
    QGraphicsScene    *scene;
    Player            *player;
    QTimer            *timer;
};

#endif // ROOM_H
