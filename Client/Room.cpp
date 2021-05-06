#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"


Room::Room(Client *client_, Player *player_, QVector<PlayerView *> &players_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room), client(client_) {
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

    //players.clear();
    //players = players_;

    next_frame.clear();
    next_frame = players_;

    QTimer *update_draw_timer = new QTimer();
    connect(update_draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    update_draw_timer->start(1000 / FPS);

    // добавляем кнопки для выхода из комнаты
    push_button_exit_in_menu = new QPushButton("Return to menu", this);
    push_button_exit_in_menu->setGeometry({this->width() - push_button_exit_in_menu->geometry().width() - 15,
                                   10,
                                   push_button_exit_in_menu->geometry().width(),
                                   push_button_exit_in_menu->geometry().height()});
    connect(push_button_exit_in_menu, SIGNAL(clicked()), this, SLOT(close_room()));
}

// может быть проблема в том, что не успевает запрос прийти с сервака и поэтому в массиве last_frame
// содержутся те же элементы, поэтому мы удаляем их и не можем новые создать!!!
void Room::paintEvent(QPaintEvent *event){
    draw_scene();
    std::cout<<"is_got_scene: "<<is_got_scene<<std::endl;
    if(!is_got_scene) {
        emit request_get_scene_on_the_server();
        qDebug() << "GET SCENE";
        is_got_scene = true;
    }
}

void Room::draw_scene(){ // event сам и не нужен
    qDebug() << "ROOM DRAW";
    update_local_player_position(); // обновляем позицию игрока
    QMutexLocker locker {&player_mutex};
    next_frame.push_back(new PlayerView(*local_player));               // добавляем в конец локального игрока
    animation_scene->add_players(last_frame, next_frame, local_player->client_id);

    local_player->chat();
}

void Room::update_local_player_position(){
    //qDebug() << "test";
    //qDebug() << local_player->direction;

    if(!is_updated_data) {
        auto data = local_player->to_json();
        emit update_state_on_the_server(data);
        is_updated_data = true;
        qDebug() << "UPDATE SCENE";
    }
}


//TODO: тут нужно поменять состояние игрока пока он пишет, чтобы он просто остановился или на конкретном кадре, типа в полёте.
void Room::keyPressEvent(QKeyEvent *apKeyEvent) {
    //qDebug() << "KeyPressEvent";
    if(apKeyEvent->key() == Qt::Key_Escape) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "", "Do you want to leave?",
                              QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            //CreateMainWidget(local_player->player_name);
            //this->close();
            client->return_to_menu("");
            return;
        }
    }else if(apKeyEvent->key() == Qt::Key_Q){
        local_player->movement = {0,0};                       // при вводе сообщения игрок останавливается

        while(true){
            bool click = false;
            QString message = "Your message....";
            QString str = QInputDialog::getText(0, "Enter message!", message, QLineEdit::Normal, "", &click);
            if(!click){ // была нажата кнопка Cancel
                break;
            }
            if(str.size() == 0){ // если ничего не ввёл, то ничего непроизошло
                break;
            } else {
                if(0 < str.size() && str.size() < 17) {
                   local_player->player_message.send_message = str;
                   local_player->message->setPlainText(local_player->player_message.send_message);
                   local_player->player_message.metka_message = true;
                   local_player->player_message.metka_message_painter = false;
                    break;
                }
            }// TODO: эксперименты с размером сообщения и если нужно, то отредактировать
        }
    }else {
        local_player->keyPressEvent(apKeyEvent);
    }
}

void Room::keyReleaseEvent(QKeyEvent *apKeyEvent){
    //qDebug() << "KeyReleaseEvent";
    local_player->keyReleaseEvent(apKeyEvent);
}

void Room::close_room() {
    qDebug() << "||||||||||||close_room||||||||||||||";
    client->return_to_menu("");
    return;
}

Room::~Room()
{
    delete ui;
}

