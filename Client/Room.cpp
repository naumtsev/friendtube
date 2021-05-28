#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"


Room::Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent) :
    QWidget(parent), ui(new Ui::Room), local_player(player_), next_frame(players_), client(client_) {
    ui->setupUi(this);

    init_variables();
    init_paramets();
    init_video();
    init_buttons();
    init_timers();
    init_NPC();


    connect(chat_window, SIGNAL(set_focus_room()), this, SLOT(set_focus_room()));
    connect(video_player, SIGNAL(set_focus_room()), this, SLOT(set_focus_room()));
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
    animation_scene = new AnimationView(this, this);

    animation_scene->local_player = local_player;

    chat_window = new ChatWindow(this, *local_player); // тут могут быть утечки памяти
    tool_item_right = new ToolManyItem(this, *local_player);

    food.push_back(":/pics/background_item/green_room/more_texture/apple.png");
    food.push_back(":/pics/background_item/green_room/more_texture/pig.png");
    food.push_back(":/pics/background_item/green_room/more_texture/strawberry.png");
    food.push_back(":/pics/background_item/green_room/more_texture/apple_2.png");
    food.push_back(":/pics/background_item/green_room/more_texture/chicken_leg.png");
    food.push_back(":/pics/background_item/green_room/more_texture/cookies.png");
    food.push_back(":/pics/background_item/green_room/more_texture/fish.png");
    food.push_back(":/pics/background_item/green_room/more_texture/green_pepper.png");
    food.push_back(":/pics/background_item/green_room/more_texture/kiwi.png");
    food.push_back(":/pics/background_item/green_room/more_texture/melon.png");
    food.push_back(":/pics/background_item/green_room/more_texture/potato.png");
    food.push_back(":/pics/background_item/green_room/more_texture/potato_2.png");
    food.push_back(":/pics/background_item/green_room/more_texture/red_pepper.png");
    food.push_back(":/pics/background_item/green_room/more_texture/ribs.png");
    food.push_back(":/pics/background_item/green_room/more_texture/steak.png");
    food.push_back(":/pics/background_item/green_room/more_texture/tomato.png");
}

void Room::init_video() {
    // video_widget
    video_widget = new QVideoWidget(this);
    video_widget->resize(640 , 384);
    video_widget->move(width() / 2 - video_widget->width() / 2, height() / 2 - video_widget->height( ) / 2);
    video_widget->setStyleSheet("QVideoWidget{background-color : black;}");
    video_player = new VideoPlayer(this, video_widget);
    connect(video_player, SIGNAL(video_request(QJsonObject)), client->n_manager, SLOT(video_request(QJsonObject)));


    // additional layer
    additional_layer = new QLabel(this);
    additional_layer->setFixedSize(video_widget->width(), video_widget->height());
    additional_layer->move(video_widget->x(), video_widget->y());
    QPixmap pix(":/images/empty_screen.png");
    additional_layer->setPixmap(pix);
    additional_layer->show();

    connect(video_player->m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(set_additional_layer(QMediaPlayer::State)));


    // video adver
    video_advert = new QLabel(this);
    static int width_advert = 300;

    video_advert->setFixedWidth(width_advert);
    video_advert->setAlignment(Qt::AlignCenter);
    video_advert->setWordWrap(true);
    video_advert->setVisible(false);
    video_advert->setStyleSheet("QLabel { background-color : #C20015; color : white; }");

    connect(video_player, &VideoPlayer::make_advert, [this](QString text){
        QTimer::singleShot(5000, [this](){video_advert->setVisible(false);});
        video_advert->setText(text);
        video_advert->setFixedHeight(video_advert->heightForWidth(width_advert));
        video_advert->show();
        video_advert->move(video_widget->x() + video_widget->width() - video_advert->width(), video_widget->y() - video_advert->heightForWidth(width_advert) - 3);
        this->set_focus_room();
    });



    // smile
    tool_item_right->show_multicolor_emoji_list_button->setFixedSize(video_btn_size, video_btn_size);

    // add video
    push_button_add_video = new QPushButton(this);
    push_button_add_video->setIcon(QIcon(QPixmap(":/pics/add_new_video_push_button.png")));
    push_button_add_video->setIconSize(QSize(video_btn_size, video_btn_size));
    push_button_add_video->setStyleSheet("background-color: rgba(0,0,0,0)");
    push_button_add_video->setFixedSize(video_btn_size, video_btn_size);

    push_button_add_video->setGeometry(video_widget->x(),
                                   video_widget->y() + video_widget->height() + widget_space_size,
                                   video_btn_size,
                                   video_btn_size);

    connect(push_button_add_video, &QPushButton::clicked, [&](){
        this->add_video();
        this->set_focus_room();
    });

    // pause
    push_button_pause_video = new QPushButton(this);
    push_button_pause_video->setIcon(QIcon(QPixmap(":/pics/pause_new_video_push_button.png")));
    push_button_pause_video->setIconSize(QSize(video_btn_size, video_btn_size));
    push_button_pause_video->setStyleSheet("background-color: rgba(0,0,0,0)");
    push_button_pause_video->setFixedSize(video_btn_size, video_btn_size);
    push_button_pause_video->setGeometry(push_button_add_video->x() + video_btn_size + video_btn_space_size,
                                   push_button_add_video->y(), video_btn_size, video_btn_size);

    connect(push_button_pause_video, &QPushButton::clicked,[&](){
        video_player->try_pause();
        this->set_focus_room();
    });

    // stop video
    push_button_stop_video = new QPushButton(this);
    push_button_stop_video->setIcon(QIcon(QPixmap(":/pics/delete_video_push_button.png")));
    push_button_stop_video->setIconSize(QSize(video_btn_size, video_btn_size));
    push_button_stop_video->setStyleSheet("background-color: rgba(0,0,0,0)");
    push_button_stop_video->setFixedSize(video_btn_size, video_btn_size);
    push_button_stop_video->setGeometry(push_button_pause_video->x() + video_btn_size + video_btn_space_size,
                                   push_button_pause_video->y(), video_btn_size, video_btn_size);
    connect(push_button_stop_video, &QPushButton::clicked, [&](){
        video_player->try_stop();
        this->set_focus_room();
    });


    // Slider
    volume_slider = new QSlider(Qt::Horizontal, this);
    volume_slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #ffffff; height: 18px; border-radius: 9px; }QSlider::handle:horizontal {width: 18px;}QSlider::add-page:qlineargradient {background: lightgrey;border-top-right-radius: 9px;border-bottom-right-radius: 9px;border-top-left-radius: 0px;border-bottom-left-radius: 0px;}QSlider::sub-page:qlineargradient {background: #42ff42;border-top-right-radius: 0px;border-bottom-right-radius: 0px;border-top-left-radius: 9px;border-bottom-left-radius: 9px;}");


    int slider_width = 120;
    int slider_height = 12;
    volume_slider->setFixedSize(slider_width , slider_height);
    volume_slider->move(push_button_stop_video->x() + push_button_stop_video->width() + video_btn_space_size,
                        push_button_stop_video->y() + video_btn_size / 2 - volume_slider->height() / 2);

    //volume_slider->setVisible(false);
    volume_slider->setMinimum(0);
    volume_slider->setMaximum(100);
    volume_slider->setSingleStep(1);
    connect(volume_slider, SIGNAL(valueChanged(int)), video_player, SLOT(change_volume(int))); // фокус возращаю в change_volume

    volume_slider->setValue(50);

    // эмоджи
    tool_item_right->setGeometry(volume_slider->x() + volume_slider->width() + video_btn_space_size, push_button_stop_video->y(), 200, 100); // лучше этот размер в room выставлять, чтобы ориентироваться на размер виджета room


    connect(client->n_manager, SIGNAL(video_set_video()), video_player, SLOT(set_video()));
    connect(client->n_manager, SIGNAL(video_stop()), video_player, SLOT(stop()));
    connect(client->n_manager, SIGNAL(video_pause()), video_player, SLOT(pause()));
}

void Room::init_buttons(){
    // добавляем кнопки для выхода из комнаты
    push_button_exit_in_menu = new QPushButton("Leave the room", this);

    push_button_exit_in_menu->setFixedSize(120, 25);

    push_button_exit_in_menu->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #bbb; background: white; height: 10px; border-radius: 4px; } QSlider::sub-page:horizontal { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #66e, stop: 1 #bbf); background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 #bbf, stop: 1 #55f); border: 1px solid #777; height: 10px; border-radius: 4px; } QSlider::add-page:horizontal { background: #fff; border: 1px solid #777; height: 10px; border-radius: 4px; } QSlider::handle:horizontal { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc); border: 1px solid #777; width: 13px; margin-top: -2px; margin-bottom: -2px; border-radius: 4px; } QSlider::handle:horizontal:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd); border: 1px solid #444; border-radius: 4px; } QSlider::sub-page:horizontal:disabled { background: #bbb; border-color: #999; } QSlider::add-page:horizontal:disabled { background: #eee; border-color: #999; } QSlider::handle:horizontal:disabled { background: #eee; border: 1px solid #aaa; border-radius: 4px; }");
    push_button_exit_in_menu->setGeometry({this->width() - push_button_exit_in_menu->geometry().width() - 10,
                                   25,
                                   push_button_exit_in_menu->geometry().width(),
                                   push_button_exit_in_menu->geometry().height()});
    push_button_exit_in_menu->setVisible(false);

    this->setGeometry(1280 - this->width(), 200, 50, 400); // лучше этот размер в room выставлять, чтобы ориентироваться на размер виджета room
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

void Room::init_NPC(){
    // хочу поесть!
    tablet_want_eating = new QLabel(this);
    tablet_want_eating->move(1130, 150);
    tablet_want_eating->setPixmap(QPixmap(":/pics/background_item/green_room/more_texture/tablet_want_eat.png"));
    tablet_want_eating->setVisible(false);

    // хочу поспать!
    QLabel *tablet_want_sleap = new QLabel(this);
    tablet_want_sleap->move(50, 140);
    tablet_want_sleap->setPixmap(QPixmap(":/pics/background_item/green_room/more_texture/tablet_want_sleap.png"));
    tablet_want_sleap->setVisible(false);

    // сюда нельзя!
    tablet_stop = new QLabel(this);
    tablet_stop->move(1045, 570);
    tablet_stop->setPixmap(QPixmap(":/pics/background_item/green_room/more_texture/tablet_entry_close.png"));
    tablet_stop->setVisible(false);

    // хватит есть!
    table_stop_eating = new QLabel(this);
    table_stop_eating->move(1130, 150);
    table_stop_eating->setPixmap(QPixmap(":/pics/background_item/green_room/more_texture/tablet_stor_eating.png"));
    table_stop_eating->setVisible(false);

    // первый курс жив!
    tablet_fist_course_alive = new QLabel(this);
    tablet_fist_course_alive->move(50, 600);
    tablet_fist_course_alive->setPixmap(QPixmap(":/pics/background_item/green_room/more_texture/table_first_course_alive.png"));
    tablet_fist_course_alive->setVisible(false);

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

// идея: когда человек дома, то может смайлики отсылкать, они будут над домом!!
// идея: сделать игроку пузико, если он очень много съел
void Room::keyPressEvent(QKeyEvent *apKeyEvent) {
    if (this->hasFocus() && (apKeyEvent->key() == Qt::Key_Enter || apKeyEvent->key() == 16777220)) {
        if(local_player->x() != x_very_far && local_player->y() != y_very_far){ // нельзя писать, когда в домике
            chat_window->get_focus();
        }
    } else {
        if(apKeyEvent->key() == Qt::Key_Y && animation_scene->sleap.xl <= local_player->x()&& local_player->x() <= animation_scene->sleap.xr && animation_scene->sleap.yl <= local_player->y()&& local_player->y() <= animation_scene->sleap.yr){
            local_player->setPos(x_very_far, y_very_far);
            local_player->saturation -= 100;
            local_player->state = AnimateState::Standing;
        }
        if(apKeyEvent->key() == Qt::Key_Y && animation_scene->want_or_dont_eat.xl <= local_player->x()&& local_player->x() <= animation_scene->want_or_dont_eat.xr && animation_scene->want_or_dont_eat.yl <= local_player->y()&& local_player->y() <= animation_scene->want_or_dont_eat.yr){
            local_player->saturation += 10;
            id_food %= food.size();
            GraphicsFood *food_ = new GraphicsFood();
            food_->setPixmap(food[id_food]);
            added_food.push_back(food_);
            //food_->setPos(1090, 175);
            food_->setPos(1125 + qrand()%100, 250 + qrand()%100);
            food_->setZValue(+50);
            id_food++;
            animation_scene->scene->addItem(food_);
//            QTimer::singleShot(1000, this, [&](){
//                delete_food();
//            });
        }
        if((apKeyEvent->key() == Qt::Key_End || apKeyEvent->key() == 16777216) && local_player->x() == x_very_far && local_player->y() == y_very_far){
            local_player->setPos(place_to_leave_the_house.first, place_to_leave_the_house.second);
        }
        if(local_player->x() != x_very_far && local_player->y() != y_very_far){
            local_player->keyPressEvent(apKeyEvent);
        }
    }
}

void Room::delete_food(){
    QGraphicsPixmapItem *food_ = added_food.dequeue();
    animation_scene->scene->removeItem(food_);
    delete food_;
}

void Room::keyReleaseEvent(QKeyEvent *apKeyEvent){
    if(this->hasFocus()){
        if(local_player->x() != x_very_far && local_player->y() != y_very_far){
            local_player->keyReleaseEvent(apKeyEvent);
        }
    }
}

void Room::mousePressEvent([[maybe_unused]] QMouseEvent *apMouseEvent){
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
   QInputDialog *input_form = new QInputDialog(this);

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


void Room::set_additional_layer(QMediaPlayer::State state) {

    if(state == QMediaPlayer::StoppedState) {
        QPixmap pix(":/images/empty_screen.png");
        additional_layer->setPixmap(pix);
        additional_layer->setVisible(true);
        video_player->try_stop();
    } else if(state == QMediaPlayer::PlayingState){
        additional_layer->setVisible(false);
    }
}

bool Room::is_owner_video(const QString &client_id) {
    return client_id == video_player->current_video.sender_name;
}


Room::~Room() {
    delete ui;
}

