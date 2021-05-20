#ifndef ROOM_H
#define ROOM_H

#include "Player.h"
#include "PlayerView.h"
#include "AnimationView.h"
#include "ChatWindow.h"
#include "Client.h"
#include "ToolManyItem.h"
#include "VideoPlayer.h"

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
#include <QLabel>

struct Menu;
struct Client;
struct VideoPlayer;

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
    void mousePressEvent(QMouseEvent *)       override;
    void paintEvent(QPaintEvent *event)      override;


    void init_paramets();
    void init_variables();
    void init_buttons();
    void init_timers();
    void init_video();

    ~Room();

public slots:
    void update_local_player_position();
    void close_room();
    void set_focus_room();
    void add_video();

signals:
    void signal_close_room();
    void request_get_scene_on_the_server();
    void update_state_on_the_server(QJsonDocument);

public:
    int                FPS = 60;
    Ui::Room          *ui;
    AnimationView     *animation_scene;
    Player            *local_player;
    QTimer            *update_draw_timer;
    ChatWindow        *chat_window = new ChatWindow;
    ToolManyItem      *tool_item_right = new ToolManyItem;
    QVector<PlayerView *> last_frame;
    QVector<PlayerView *> next_frame;
    QMutex player_mutex;


    Client *client;
    bool got_scene = false;
    bool updated_data = false;

    QPushButton *push_button_exit_in_menu;

    // video structs

    QVideoWidget *video_widget;
    VideoPlayer *video_player;
    qint16 video_btn_size = 30;
    qint16 video_btn_space_size = 15;
    qint16 space_between_video_widget = 10;
    QPushButton *push_button_add_video;
    QPushButton *push_button_stop_video;
    QPushButton *push_button_pause_video;
    QPushButton *push_button_volume_video;
    QSlider *volume_slider;
    QLabel *video_advert;
    QLabel *video_description;


};

#endif // ROOM_H
