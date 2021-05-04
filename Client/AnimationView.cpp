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

    /* Немного поднастроим отображение виджета и его содержимого */
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
        this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

        /* Также зададим минимальные размеры виджета
         * */
//        this->setMinimumHeight(100);
//        this->setMinimumWidth(100);
//        this->setMaximumHeight(720);
//        this->setMaximumHeight(1280);

        //scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
        this->setScene(&scene);          // Устанавливаем сцену в виджет
        scene.setItemIndexMethod(QGraphicsScene::NoIndex);
        timer_update_scene = new QTimer();
        connect(timer_update_scene, SIGNAL(timeout()), &scene, SLOT(update()));
        timer_update_scene->start(20);
}


void AnimationView::add_players(QVector<PlayerView *> &players_, QString local_id){
    qDebug() << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
    scene.clear();
    bool draw_local_player = false;
    QBrush whiteBrush(Qt::white);
    QPen blackPen(Qt::black);
    std::cout<<players_.size()<<std::endl;
    for(int i = 0; i < players_.size(); i++){
        if(players_[i]->client_id != local_id || draw_local_player){
            std::cout<<"11111111111111111111111111111111111111111111111111111111111111111111"<<std::endl;
            players_[i]->update_state();
            scene.addItem(players_[i]);
            scene.addItem(players_[i]->name);
        } else {
            draw_local_player = true;
        }
    }

}
