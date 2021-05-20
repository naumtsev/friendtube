#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"


Room::Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent) :
    QWidget(parent), ui(new Ui::Room), local_player(player_), next_frame(players_), client(client_) {
    ui->setupUi(this);

    init_variables();
    init_paramets();
    init_buttons();
    init_timers();
    init_video();


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

    animation_scene->local_player = local_player;

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
    push_button_exit_in_menu->setVisible(false);
}

void Room::init_video() {
    video_widget = new QVideoWidget(this);
    video_widget->resize(960 / 1.5, 576 / 1.5);
    video_widget->move(width() / 2 - video_widget->width() / 2, height() / 2 - video_widget->height( ) / 2);
    video_player = new VideoPlayer(this, video_widget);

    connect(video_player, SIGNAL(video_request(QJsonObject)), client->n_manager, SLOT(video_request(QJsonObject)));

    video_advert = new QLabel(this);
    static int width_advert = 300;
    video_advert->setFixedWidth(width_advert);
    video_advert->setAlignment(Qt::AlignRight);
    video_advert->setWordWrap(true);
    video_advert->setVisible(false);

    connect(video_player, &VideoPlayer::make_advert, [this](QString text){
        QTimer::singleShot(5000, [this](){video_advert->setVisible(false);});
        video_advert->setText(text);
        video_advert->setFixedHeight(video_advert->heightForWidth(width_advert));
        video_advert->move(video_widget->x() + video_widget->width() - video_advert->width(), video_widget->y() - video_advert->heightForWidth(width_advert));
        video_advert->show();
    });


    push_button_add_video = new QPushButton(" + ", this);
    push_button_add_video->setFixedSize(video_btn_size, video_btn_size);

    push_button_add_video->setGeometry({video_widget->x() + video_widget->width() + space_between_video_widget,
                                   video_widget->y(),
                                   video_btn_size,
                                   video_btn_size});

   connect(push_button_add_video, SIGNAL(clicked()) , this, SLOT(add_video()));




    push_button_pause_video = new QPushButton(" = ", this);
    push_button_pause_video->setFixedSize(video_btn_size, video_btn_size);
    push_button_pause_video->move(video_widget->x() + video_widget->width() + space_between_video_widget,
                                   video_widget->y() + video_btn_size + video_btn_space_size);

    connect(push_button_pause_video, SIGNAL(clicked()), video_player, SLOT(try_pause()));


    push_button_stop_video = new QPushButton(" - ", this);
    push_button_stop_video->setFixedSize(video_btn_size, video_btn_size);
    push_button_stop_video->move(video_widget->x() + video_widget->width() + space_between_video_widget,
                                   video_widget->y() + 2 * video_btn_size + 2 * video_btn_space_size);

    connect(push_button_stop_video, SIGNAL(clicked()), video_player, SLOT(try_stop()));


    push_button_volume_video = new QPushButton(" ♪ ", this);
    push_button_volume_video->setFixedSize(video_btn_size, video_btn_size);
    push_button_volume_video->move(video_widget->x() + video_widget->width() + space_between_video_widget,
                                   video_widget->y() + 3 * video_btn_size + 3 * video_btn_space_size);

    connect(push_button_volume_video, &QPushButton::clicked, [this](){volume_slider->setVisible(volume_slider->isVisible() ^ true);});

    volume_slider = new QSlider(this);
    int slider_width = 15;
    int slider_height = 120;

    volume_slider->setFixedSize(slider_width , slider_height);
    volume_slider->move(video_widget->x() + video_widget->width() + space_between_video_widget + video_btn_size / 2 - slider_width / 2,
                        video_widget->y() + 4 * video_btn_size + 4 * video_btn_space_size);

    volume_slider->setVisible(false);
    volume_slider->setMinimum(0);
    volume_slider->setMaximum(100);
    volume_slider->setSingleStep(1);
    connect(volume_slider, SIGNAL(valueChanged(int)), video_player, SLOT(change_volume(int)));
    volume_slider->setValue(50);
    connect(client->n_manager, SIGNAL(video_set_video()), video_player, SLOT(set_video()));
    connect(client->n_manager, SIGNAL(video_stop()), video_player, SLOT(stop()));
    connect(client->n_manager, SIGNAL(video_pause()), video_player, SLOT(pause()));

}



void Room::init_timers(){
    QTimer *update_draw_timer = new QTimer();
    connect(update_draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this, SIGNAL(signal_close_room()), update_draw_timer, SLOT(stop()));


    update_draw_timer->start(1000 / FPS);

    QTimer *this_window_have_focus_timer = new QTimer();
    connect(this_window_have_focus_timer, &QTimer::timeout, [&](){
        if(!this->hasFocus()){
            local_player->movement = {0,0};
        }
    });

    connect(this, SIGNAL(signal_close_room()), this_window_have_focus_timer, SLOT(stop()));
    this_window_have_focus_timer->start(50);
}

void Room::paintEvent(QPaintEvent *event){
    draw_scene();
    if(!got_scene) {
        emit request_get_scene_on_the_server();
        got_scene = true;
    }
}

void Room::draw_scene(){
    // обновляем позицию игрока
    update_local_player_position();

    QMutexLocker locker {&player_mutex};
    // добавляем в конец локального игрока
    next_frame.push_back(new PlayerView(*local_player));
    animation_scene->add_players(last_frame, next_frame, local_player->client_id);
    local_player->chat();

}

void Room::update_local_player_position(){
    if(!updated_data) {
        auto data = local_player->to_json();
        emit update_state_on_the_server(data);
        updated_data = true;
    }
}


void Room::keyPressEvent(QKeyEvent *apKeyEvent) {
    if (this->hasFocus() && (apKeyEvent->key() == Qt::Key_Enter || apKeyEvent->key() == 16777220)) {
        chat_window->get_focus();
    } else {
        local_player->keyPressEvent(apKeyEvent);
    }
}

void Room::keyReleaseEvent(QKeyEvent *apKeyEvent){
    if(this->hasFocus()){
     local_player->keyReleaseEvent(apKeyEvent);
    }
}

void Room::mousePressEvent(QMouseEvent *apMouseEvent){
    set_focus_room();
    if(tool_item_right->show_multicolor_emoji_list_widget->isVisible()){
        tool_item_right->show_multicolor_emoji_list_widget->hide();
    }
}

void Room::set_focus_room(){
    this->setFocus();
    chat_window->lose_focus();
}

void Room::close_room() {
    // RETURN TO MENU
    emit signal_close_room();
    client->return_to_menu("");
    return;
}


void Room::add_video(){
   QInputDialog *input_form = new QInputDialog();

   input_form->setWindowTitle("FriendTube");
   input_form->setWindowIcon(QIcon(QPixmap(":/images/icon.png")));
   input_form->setFixedSize(350, 108);

   input_form->setInputMode(QInputDialog::InputMode::TextInput);
   input_form->setLabelText("Введите ссылку на видео, находящееся на yandex.disk");
   input_form->setOkButtonText("Отправить");
   input_form->setCancelButtonText("Отмена");
   input_form->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
   input_form->setModal(true);
   input_form->show();

   bool ok = input_form->exec();

   if(ok) {
     video_player->try_set_video(input_form->textValue());
   }

   delete input_form;
}



Room::~Room() {
    delete ui;
}

