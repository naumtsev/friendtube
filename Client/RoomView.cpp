#include "RoomView.h"

RoomView::RoomView(QWidget *parent) :
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

        scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
        this->setScene(scene);          // Устанавливаем сцену в виджет
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);

        video_widget = new QVideoWidget(this);
        video_widget->resize(960 / 1.5, 576 / 1.5);
        video_widget->move(width() / 2 - video_widget->width() / 2, height() / 2 - video_widget->height( ) / 2);

        qDebug() << "Video widget was created";

        timer_update_scene = new QTimer();
        connect(timer_update_scene, SIGNAL(timeout()), scene, SLOT(update()));
        timer_update_scene->start(20);
}


void RoomView::update_players(QVector<PlayerView *> &players_, QString local_id){
   // scene->clear();
    bool draw_local_player = false;
    QBrush whiteBrush(Qt::white);
    QPen blackPen(Qt::black);
    for(int i = 0; i < players_.size(); i++){
        if(players_[i]->client_id != local_id || draw_local_player){
            players_[i]->update_state();
            scene->addItem(players_[i]);
            scene->addItem(players_[i]->name);
        } else {
            draw_local_player = true;
        }
    }
}



RoomView::~RoomView() {
    scene->~QGraphicsScene();
    timer_update_scene->stop();
    timer_update_scene->~QTimer();
}
