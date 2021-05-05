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

    this->setScene(&scene);
   // timer_update_scene = new QTimer();
   // connect(timer_update_scene, SIGNAL(timeout()), this, SLOT(update()));
   // timer_update_scene->start(3);
}


void AnimationView::add_players(QVector<PlayerView *> &last_frame, QVector<PlayerView *> &next_frame, QString local_id){
    if(next_frame.size() > 1){
        bool draw_local_player = false;
        for(int i = 0; i < next_frame.size(); i++){
            if(next_frame[i]->client_id != local_id || draw_local_player){
                next_frame[i]->update_state();
                scene.addItem(next_frame[i]);
                scene.addItem(next_frame[i]->name);
            } else {
                draw_local_player = true;
            }
        }
        clear_vector(last_frame, local_id);
        last_frame = next_frame;
        next_frame.clear();
    }
}

void AnimationView::clear_vector(QVector<PlayerView *> &last_frame, QString local_id){
    bool clear_local_player = false;
    for(int i = 0; i < last_frame.size(); i++){
        if(last_frame[i]->client_id != local_id || clear_local_player){
            scene.removeItem(last_frame[i]);
            scene.removeItem(last_frame[i]->name);
        } else {
            clear_local_player = true;
        }
    }
}
