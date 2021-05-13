#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"

Room::Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room), client(client_), local_player(player_), next_frame(players_) {
    ui->setupUi(this); // всегда в начале нужно это делать

    init_variables();
    init_paramets();
    init_buttons();
    init_timers();

    connect(chat_window, SIGNAL(set_focus_room()), this, SLOT(set_focus_room()));
    connect(tool_item_right, SIGNAL(set_focus_room()), this, SLOT(set_focus_room()));
    connect(push_button_exit_in_menu, SIGNAL(clicked()), this, SLOT(close_room()));
}

void Room::init_paramets(){
    ui->gridLayout->addWidget(animation_scene);
    ui->gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->resize(1280,720);
    this->setFixedSize(1280,720);
    //this->showFullScreen();
    this->setWindowTitle("FriendTube");
    this->setWindowIcon(QIcon(QPixmap(":/images/icon.png")));

    this->setFocus();
}

void Room::init_variables(){
    animation_scene = new AnimationView(this);
    chat_window = new ChatWindow(this, *local_player); // тут могут быть утечки памяти
    tool_item_right = new ToolManyItem(this, *local_player);
}

void Room::init_buttons(){
    // добавляем кнопки для выхода из комнаты
    push_button_exit_in_menu = new QPushButton("Leave the room", this);

    push_button_exit_in_menu->setFixedSize(120, 25);

    push_button_exit_in_menu->setStyleSheet("QPushButton { border: 1px solid #8f8f91; border-radius: 3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #f6f7fa); min-width: 80px; } QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #a0cd58, stop: 1 #bff774);}QPushButton:flat {border: none; }QPushButton:default {border-color: navy; /* делаем кнопку по умолчанию выпуклой */ }");

    push_button_exit_in_menu->setGeometry({this->width() - push_button_exit_in_menu->geometry().width() - 10,
                                   25,
                                   push_button_exit_in_menu->geometry().width(),
                                   push_button_exit_in_menu->geometry().height()});
}

void Room::init_timers(){
    QTimer *update_draw_timer = new QTimer();
    connect(update_draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    update_draw_timer->start(1000 / FPS);

    QTimer *this_window_have_focus_timer = new QTimer();
    connect(this_window_have_focus_timer, &QTimer::timeout, [&](){
        if(!this->hasFocus()){
            local_player->movement = {0,0};
        }
    });
    this_window_have_focus_timer->start(50);
}

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


void Room::keyPressEvent(QKeyEvent *apKeyEvent) {
    //std::cout<< apKeyEvent->key() <<std::endl;
    if(this->hasFocus()){
        if(apKeyEvent->key() == Qt::Key_Escape) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "", "Do you want to leave?",
                                  QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes){
                emit return_to_menu("");
                return;
            }
        } else if(apKeyEvent->key() == Qt::Key_Enter || apKeyEvent->key() == 16777220){ // походу у меня enter-a нет (
            chat_window->get_focus();
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
    //if(chat_window->show_multicolor_emoji_list_widget->isVisible()){
    //    chat_window->show_multicolor_emoji_list_widget->hide();
    //}
}

void Room::set_focus_room(){
    this->setFocus();
    chat_window->lose_focus();
}

void Room::close_room() {
    emit return_to_menu("");
    return;
}

Room::~Room() {

    delete ui;
}

