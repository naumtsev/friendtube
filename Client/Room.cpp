#include "Room.h"
#include "PlayerView.h"
#include "ui_Room.h"


Room::Room(Client *client_, Player *player_, QVector<PlayerView> &players_, QWidget *parent):
    QWidget(parent),
    ui(new Ui::Room), client(client_) {
    ui->setupUi(this);
    this->resize(1280,720);
    this->setFixedSize(1280,720);
    //this->showFullScreen();
    this->setWindowTitle("FriendTube");
    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));


    scene = new QGraphicsScene();
    local_player =  player_;        // создаём персонажа

    players.clear();
    players = players_;
    QTimer *update_draw_timer = new QTimer();
    connect(update_draw_timer, SIGNAL(timeout()), this, SLOT(update()));
    update_draw_timer->start(1000/FPS);

}

void Room::paintEvent(QPaintEvent *event){

    draw_scene();
    if(!is_got_scene) {
        emit request_get_scene_on_the_server();
        //qDebug() << "GET SCENE";
        is_got_scene = true;
    }
}

void Room::draw_scene(){ // event сам и не нужен
    QPainter painter(this);

   // qDebug() << "ROOM DRAW";
    update_local_player_position(); // обновляем позицию игрока

    for(std::size_t i = 0; i < players.size(); i++){
        if(players[i].client_id != local_player->client_id){
            players[i].draw(painter);
        }
    }
    local_player->draw(painter);
}




void Room::update_local_player_position(){
    //qDebug() << "test";
    if(!is_updated_data) {
         auto data = local_player->to_json();
        emit update_state_on_the_server(data);
        is_updated_data = true;
        //qDebug() << "UPDATE SCENE";
    }
}

void Room::keyPressEvent(QKeyEvent *apKeyEvent) {

    //qDebug() << "KeyPressEvent";
    if(apKeyEvent->key() == Qt::Key_Escape) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "", "Do you want to leave?",
                              QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            //CreateMainWidget(local_player->player_name);
            this->close();
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
            }else if(0 < str.size() && str.size() < 17) {
                    local_player->player_message.send_message = str;
                    local_player->player_message.metka_message = true;
                    local_player->player_message.metka_message_painter = false;
                    break;
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


Room::~Room()
{
    delete ui;
}
