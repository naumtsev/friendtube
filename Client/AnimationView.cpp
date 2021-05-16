#include "AnimationView.h"

AnimationView::AnimationView(QWidget *parent) :
    QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);

    setCacheMode(QGraphicsView::CacheNone);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setWindowTitle("FriendTube");
    setFrameStyle(0);
    setSceneRect(0, 0, 1280, 720);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setFixedSize(1280, 720);

    setFrameStyle(0);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету


    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1280,720);
    this->setScene(scene);
    timer_update_scene = new QTimer();







    connect(timer_update_scene, SIGNAL(timeout()), this, SLOT(update()));
    timer_update_scene->start(3);
}


void AnimationView::add_players(QVector<PlayerView *> &last_frame, QVector<PlayerView *> &next_frame, QString local_id){
    if(next_frame.size() > 1){
        bool draw_local_player = false;
        for(int i = 0; i < next_frame.size(); i++){
            if(next_frame[i]->client_id != local_id || draw_local_player){
                next_frame[i]->update_state();
                scene->addItem(next_frame[i]);
                scene->addItem(next_frame[i]->name);
                display_message(next_frame[i]);
            } else {
                draw_local_player = true;
            }
        }
        clear_vector(last_frame, local_id);
        last_frame = next_frame;
    }
    next_frame.clear();
}

void AnimationView::display_message(PlayerView *player){
    if(player->player_message.send_message != ""){
        if(player->player_message.type == "text"){
            scene->addItem(player->message);
        } else {
            QPixmap emoji(player->player_message.send_message); // подкорректировать расположение изображения, чтобы прям над персонажем
            emoji = emoji.scaled(25,25,Qt::KeepAspectRatio);
            player->player_message.emoji = new QGraphicsPixmapItem(emoji);
            player->player_message.emoji->setPos(player->message->pos());
            scene->addItem(player->player_message.emoji);
        }
    }
}

void AnimationView::clear_vector(QVector<PlayerView *> &last_frame, QString local_id){
    bool clear_local_player = false;
    for(int i = 0; i < last_frame.size(); i++){
        if(last_frame[i]->client_id != local_id || clear_local_player){
            scene->removeItem(last_frame[i]); // тут скорее всего нужно удалять элемент
            scene->removeItem(last_frame[i]->name); // тут скорее всего нужно удалять элемент
            if(last_frame[i]->player_message.send_message != ""){
                if(last_frame[i]->player_message.type == "text"){
                    scene->removeItem(last_frame[i]->message); // тут скорее всего нужно удалять элемент
                    //delete last_frame[i]->message;
                } else {
                    scene->removeItem(last_frame[i]->player_message.emoji); // тут скорее всего нужно удалять элемент
                    //delete last_frame[i]->player_message.emoji;
                }
            }
            delete last_frame[i];
        } else {
            clear_local_player = true;
        }
    }
}



