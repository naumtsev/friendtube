#ifndef ROOM_H
#define ROOM_H

#include "Player.h"
#include "PlayerView.h"
#include "AnimationView.h"
#include "ChatWindow.h"
#include "Client.h"
#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
#include <QInputDialog>
#include <QThreadPool>
#include <QGraphicsView>
#include <QMutex>
#include <QPushButton>

struct Menu;
struct Client;

namespace Ui {
struct Room;
}

struct Room : public QWidget {
    Q_OBJECT

public:
    Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent = nullptr);
    void draw_scene();
    void keyPressEvent  (QKeyEvent *)       override;  // обработка нажатий клавиш
    void keyReleaseEvent(QKeyEvent *)       override;  // обработка отжатия клавиш
    void paintEvent(QPaintEvent *event)      override;

    ~Room();

public slots:
    void update_local_player_position();
    void close_room();

signals:
    void request_get_scene_on_the_server();
    void update_state_on_the_server(QJsonDocument);
    void return_to_menu(const QString &reason);

public:
    Ui::Room          *ui;
    AnimationView     *animation_scene;
    Player            *local_player;
    QTimer            *update_draw_timer;
    ChatWindow        *chat_window;
    int                FPS = 60;
    QMutex player_mutex;
    QVector<PlayerView *> last_frame;
    QVector<PlayerView *> next_frame;
    Client *client;
    bool is_got_scene = false;
    bool is_updated_data = false;
    QPushButton *push_button_exit_in_menu;
};

#endif // ROOM_H
