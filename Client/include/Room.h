#ifndef ROOM_H
#define ROOM_H

#include <QDebug>
#include <QGraphicsView>
#include <QIcon>
#include <QInputDialog>
#include <QLabel>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QMutex>
#include <QPainter>
#include <QPushButton>
#include <QQueue>
#include <QRandomGenerator>
#include <QShortcut>
#include <QThreadPool>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "AnimationView.h"
#include "ChatWindow.h"
#include "Chat.h"
#include "Client.h"
#include "Player.h"
#include "PlayerView.h"
#include "ToolManyItem.h"
#include "VideoPlayer.h"

struct Menu;
struct Client;
struct VideoPlayer;
class AnimationView;

namespace Ui {
    struct Room;
}

enum class RoomType {
    Summer,
    Strange
};

struct Room : public QWidget {
    Q_OBJECT

public:
    Room(Client *client_, Player *player_, QVector<PlayerView *> &players_,
         RoomType r_type, QWidget *parent = nullptr);
    void draw_scene();
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *event) override;

    void init_paramets();
    void init_variables();
    void init_buttons();
    void init_timers();
    void init_video();
    void init_NPC();
    void init_chat();

    void delete_food();
    bool is_owner_video(const QString &s);
    ~Room();

public slots:
    void update_local_player_position();
    void close_room();
    void set_focus_room();
    void add_video();
    void set_additional_layer(QMediaPlayer::State state);

signals:
    void signal_close_room();
    void request_get_scene_on_the_server();
    void update_state_on_the_server(QJsonDocument);

public:
    int FPS = 60;
    int x_very_far = -1000, y_very_far = -1000;
    std::pair<int, int> place_to_leave_the_house = {110, 190};
    QVector<QString> food;
    QQueue<QGraphicsPixmapItem *> added_food;
    int id_food = 0;

    Ui::Room *ui;

    AnimationView *animation_scene;

    Player *local_player;
    bool move_player_or_no = true;
    QTimer *update_draw_timer;
    ChatWindow *chat_window = new ChatWindow;
    Chat *local_chat = new Chat;
    ToolManyItem *tool_item_right = new ToolManyItem;
    QVector<PlayerView *> last_frame;
    QVector<PlayerView *> next_frame;
    QMutex player_mutex;

    Client *client;
    bool got_scene = false;
    bool updated_data = false;
    RoomType type;

    QPushButton *push_button_exit_in_menu;

    // tablets
    QLabel *tablet_want_eating;
    QLabel *tablet_want_sleap;
    QLabel *tablet_stop;
    QLabel *table_stop_eating;
    QLabel *tablet_fist_course_alive;

    // video structs
    QVideoWidget *video_widget;
    VideoPlayer *video_player;
    qint16 video_btn_size = 20;
    qint16 video_btn_space_size = 15;
    qint16 widget_space_size = 5;
    QPushButton *push_button_add_video;
    QPushButton *push_button_stop_video;
    QPushButton *push_button_pause_video;
    QPushButton *push_button_volume_video;
    QSlider *volume_slider;
    QLabel *video_advert;
    QLabel *video_description;
    QLabel *additional_layer;
};

#endif// ROOM_H
