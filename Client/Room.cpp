#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"

Room::Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room), client(client_) {
    this->setFocus();
    ui->setupUi(this);
    animation_scene = new AnimationView();
    ui->gridLayout->addWidget(animation_scene);
    ui->gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //animation_scene->show();

    this->resize(1280,720);
    this->setFixedSize(1280,720);
    //this->showFullScreen();
    this->setWindowTitle("FriendTube");

    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));

    local_player =  player_;

    bool close = false;
    chat_window = new ChatWindow(this, *local_player, close); // тут могут быть утечки памяти

    next_frame.clear();
    next_frame = players_;

    QTimer *update_draw_timer = new QTimer();
    connect(update_draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    update_draw_timer->start(1000 / FPS);

    connect(chat_window, SIGNAL(set_focus_room()), this, SLOT(set_focus_room()));
    // добавляем кнопки для выхода из комнаты
    push_button_exit_in_menu = new QPushButton("Leave the room", this);

    push_button_exit_in_menu->setFixedSize(120, 25);

    push_button_exit_in_menu->setStyleSheet("QPushButton { border: 1px solid #8f8f91; border-radius: 3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #f6f7fa); min-width: 80px; } QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #a0cd58, stop: 1 #bff774);}QPushButton:flat {border: none; }QPushButton:default {border-color: navy; /* делаем кнопку по умолчанию выпуклой */ }");

    push_button_exit_in_menu->setGeometry({this->width() - push_button_exit_in_menu->geometry().width() - 10,
                                   25,
                                   push_button_exit_in_menu->geometry().width(),
                                   push_button_exit_in_menu->geometry().height()});

    connect(push_button_exit_in_menu, SIGNAL(clicked()), this, SLOT(close_room()));
}

// может быть проблема в том, что не успевает запрос прийти с сервака и поэтому в массиве last_frame
// содержутся те же элементы, поэтому мы удаляем их и не можем новые создать!!!
void Room::paintEvent(QPaintEvent *event){
    draw_scene();
    if(!is_got_scene) {
        emit request_get_scene_on_the_server();
       // qDebug() << "GET SCENE";
        is_got_scene = true;
    }
}

void Room::draw_scene(){ // event сам и не нужен
   // qDebug() << "ROOM DRAW";
    update_local_player_position(); // обновляем позицию игрока
    QMutexLocker locker {&player_mutex};
    next_frame.push_back(new PlayerView(*local_player));               // добавляем в конец локального игрока
    animation_scene->add_players(last_frame, next_frame, local_player->client_id);

    local_player->chat();
}

void Room::update_local_player_position(){
    //qDebug() << local_player->direction;

    if(!is_updated_data) {
        auto data = local_player->to_json();
        emit update_state_on_the_server(data);
        is_updated_data = true;    }
}


//TODO: тут нужно поменять состояние игрока пока он пишет, чтобы он просто остановился или на конкретном кадре, типа в полёте.
void Room::keyPressEvent(QKeyEvent *apKeyEvent) {
    if(this->hasFocus()){
        if(apKeyEvent->key() == Qt::Key_Escape) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "", "Do you want to leave?",
                                  QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes){
                emit return_to_menu("");
                return;
            }
        } else {
            local_player->keyPressEvent(apKeyEvent);
        }
    }
}

void Room::keyReleaseEvent(QKeyEvent *apKeyEvent){
    if(this->hasFocus()){
     local_player->keyReleaseEvent(apKeyEvent);
    }
}

void Room::mousePressEvent(QMouseEvent *apMouseEvent){
    set_focus_room();
    if(chat_window->show_multicolor_emoji_list_widget->isVisible()){
        chat_window->show_multicolor_emoji_list_widget->hide();
    }
}

void Room::set_focus_room(){
    this->setFocus();
}

void Room::close_room() {
    emit return_to_menu("");
    return;
}

Room::~Room() {

    delete ui;
}

