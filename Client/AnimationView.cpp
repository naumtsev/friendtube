#include "AnimationView.h"
#include <typeinfo>

GraphicsBush::GraphicsBush(QObject *parent)
  :  QObject(parent) {
  //this->setBrush(QColor(0, 255, 0));
}

QPainterPath GraphicsBush::shape() const {
    QPainterPath path;
    path.addEllipse(10,10,10,10);
    return path;
}

GraphicsTree::GraphicsTree(QObject *parent)
  :  QObject(parent) {
  //this->setBrush(QColor(0, 255, 0));
}

QPainterPath GraphicsTree::shape() const {
    QPainterPath path;
    path.addEllipse(30,80,20,30);
    return path;
}

GraphicsPingvin::GraphicsPingvin(QObject *parent)
  :  QObject(parent) {
  //this->setBrush(QColor(0, 255, 0));
}

QPainterPath GraphicsPingvin::shape() const {
    QPainterPath path;
    path.addEllipse(6,6,12,12);
    return path;
}

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

    init_background_item();
}

void AnimationView::init_background_item(){
    if(number_room == 0){
        QBrush *ibrush = new QBrush;
        ibrush->setTextureImage(QImage(":/images/background1.png"));
        scene->setBackgroundBrush(*ibrush);
        // по бокам ограда
        for(int i = 0; i < 26; i++){ // сверху
            QPixmap emoji(":/pics/background_item/green_room/fence_12gorizontal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48 * i + 3, 3);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 26; i++){ // снизу
            QPixmap emoji(":/pics/background_item/green_room/fence_12gorizontal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48*i + 3, 720 - 48 + 3);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 14; i++){ // слева
            QPixmap emoji(":/pics/background_item/green_room/fence_11vertikal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(0, 48 * i);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 14; i++){ // справа
            QPixmap emoji(":/pics/background_item/green_room/fence_11vertikal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(1280 - 24 - 8, 48 * i);
            scene->addItem(fence_vertical);
        }
        // ограда плеера
        for(int i = 6; i < 20; i++){ // сверху
            QPixmap emoji(":/pics/background_item/green_room/fence_12gorizontal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48 * i + 3, 48 * 3);
            scene->addItem(fence_vertical);
        }
        for(int i = 6; i < 20; i++){ // снизу
            QPixmap emoji(":/pics/background_item/green_room/fence_12gorizontal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48*i + 3, 720 - 48 * 4);
            scene->addItem(fence_vertical);
        }
        for(int i = 3; i < 11; i++){ // слева
            QPixmap emoji(":/pics/background_item/green_room/fence_11vertikal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48*6, 48 * i - 1);
            scene->addItem(fence_vertical);
        }
        for(int i = 3; i < 11; i++){ // справа
            QPixmap emoji(":/pics/background_item/green_room/fence_11vertikal.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(1280 - 24 - 8 - 48*6, 48 * i - 1);
            scene->addItem(fence_vertical);
        }
        // дополнительные item
        for(int i = 0; i < 10; i++){
            QPixmap emoji(":/pics/background_item/green_room/GrassObjects_32_04.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsBush *fence_vertical = new GraphicsBush;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+3);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }

        for(int i = 0; i < 10; i++){
            QPixmap emoji(":/pics/background_item/green_room/Tree.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsTree *fence_vertical = new GraphicsTree;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+4);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }
    } else {
        QBrush *ibrush = new QBrush;
        ibrush->setTextureImage(QImage(":/images/background2.png"));
        scene->setBackgroundBrush(*ibrush);
        QString path_pixmap = ":/pics/background_item/blue_room/snow_tree3.png";
        // по бокам ограда
        for(int i = 0; i < 27; i++){ // сверху
            QPixmap emoji(":/pics/background_item/blue_room/snow_tree1.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48 * i + 3, 3);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 27; i++){ // сверху
            if(qrand()%3 == 0){
                QPixmap emoji(":/pics/background_item/blue_room/snow_tree2.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
                QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
                fence_vertical->setPos(48 * i + 3, qrand()% 10 + 3);
                scene->addItem(fence_vertical);
            }
        }
        for(int i = 0; i < 27; i++){ // снизу
            QPixmap emoji(":/pics/background_item/blue_room/snow_tree1.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48*i + 3, 720 - 48 + 3);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 14; i++){ // слева
            QPixmap emoji(":/pics/background_item/blue_room/snow_tree1.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(0, 48 * i);
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < 14; i++){ // справа
            QPixmap emoji(":/pics/background_item/blue_room/snow_tree1.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(1280 - 24 - 8, 48 * i);
            scene->addItem(fence_vertical);
        }
        // ограда плеера
        for(int i = 12; i < 40; i++){ // сверху
            QPixmap emoji(path_pixmap); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(24 * i + 3, 48 * 3 - 5);
            scene->addItem(fence_vertical);
        }
        for(int i = 12; i < 40; i++){ // снизу
            QPixmap emoji(path_pixmap); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(24*i + 3, 720 - 24 * 7);
            scene->addItem(fence_vertical);
        }
        for(int i = 6; i < 24; i++){ // слева
            QPixmap emoji(path_pixmap); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(48*6 + 2, 24 * i);
            scene->addItem(fence_vertical);
        }
        for(int i = 6; i < 24; i++){ // справа
            QPixmap emoji(path_pixmap); // подкорректировать расположение изображения, чтобы прям над персонажем
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(emoji);
            fence_vertical->setPos(1280 - 24 - 8 - 48*6, 24 * i - 1);
            scene->addItem(fence_vertical);
        }

        for(int i = 0; i < qrand()%10; i++){ // 1 тип пингвина
            QPixmap emoji(":/pics/background_item/blue_room/pingvin1.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsPingvin *fence_vertical = new GraphicsPingvin;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+4);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < qrand()%10; i++){ // 2 тип пингвина
            QPixmap emoji(":/pics/background_item/blue_room/pingvin2.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsPingvin *fence_vertical = new GraphicsPingvin;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+4);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < qrand()%10; i++){ // 3 тип пингвина
            QPixmap emoji(":/pics/background_item/blue_room/pingvin3.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsPingvin *fence_vertical = new GraphicsPingvin;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+4);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }
        for(int i = 0; i < qrand()%10; i++){ // 4 тип пингвина
            QPixmap emoji(":/pics/background_item/blue_room/pingvin4.png"); // подкорректировать расположение изображения, чтобы прям над персонажем
            GraphicsPingvin *fence_vertical = new GraphicsPingvin;
            fence_vertical->setPixmap(emoji);
            fence_vertical->setZValue(+4);
            while(!scene->collidingItems(fence_vertical).isEmpty()){
                fence_vertical->setPos(qrand() % 1280, qrand() % 720);
            }
            scene->addItem(fence_vertical);
        }
    }
}

void AnimationView::add_players(QVector<PlayerView *> &last_frame, QVector<PlayerView *> &next_frame, QString local_id){
    if(next_frame.size() > 1){

        bool draw_local_player = false;
        int id = -1;

        for(int i = 0; i < next_frame.size(); i++){
            if(next_frame[i]->client_id != local_id || draw_local_player){
                next_frame[i]->update_state();
                scene->addItem(next_frame[i]);
                    if(i != next_frame.size() - 1){
                        scene->addItem(next_frame[i]->name);
                        display_message(next_frame[i]);
                    }
            } else {
                id = i;
                draw_local_player = true;
            }
        }

        clear_vector(last_frame, local_id);

        int i = 0;

        while(colliding_with_player(next_frame) > 0){
            next_frame[next_frame.size() - 1]->setPos(local_player->position_movement_last_frame[i].first,
                                                      local_player->position_movement_last_frame[i].second);
            next_frame[next_frame.size() - 1]->name->setPos(local_player->position_name_movement_last_frame[i].first,
                                                            local_player->position_name_movement_last_frame[i].second);

            local_player->setPos(local_player->position_movement_last_frame[i].first,
                                 local_player->position_movement_last_frame[i].second);

            local_player->name->setPos(local_player->position_name_movement_last_frame[i].first,
                                       local_player->position_name_movement_last_frame[i].second);
            i++;

        }
        next_frame[i]->update_state();
        scene->addItem(next_frame[next_frame.size() - 1]->name);
        display_message(next_frame[next_frame.size() - 1]);

        local_player->position_movement_last_frame.clear();
        local_player->position_name_movement_last_frame.clear();

        last_frame = next_frame;

    }
    next_frame.clear();
}

int AnimationView::colliding_with_player(QVector<PlayerView *> &next_frame){
    int id = next_frame.size() - 1;
    int count = 0;
    QList<QGraphicsItem *> colliding = scene->collidingItems(next_frame[id]);
    for(QGraphicsItem* item: colliding){
        int count_i = 1;
        for(int i = 0; i <= id; i++){
            if(item == next_frame[i]){
                count_i = 0;
            }
            if(item == next_frame[i]->name){
                count_i = 0;
            }
            if(next_frame[i]->player_message.type == "emoji" && item == next_frame[i]->player_message.emoji){
                count_i = 0;
            }
            if(count_i == 0){
                break;
            }
        }
        count += count_i;
    }
    return count;
}

void AnimationView::display_message(PlayerView *player){
    if(player->player_message.send_message != ""){
        if(player->player_message.type == "text"){
            player->message->setPos(player->pos().x() + 24 - player->message->boundingRect().width()/2, player->pos().y() - 30);     // чтобы не было глюков при замене direction нужно,
            scene->addItem(player->message);
        } else {
            QPixmap emoji(player->player_message.send_message); // подкорректировать расположение изображения, чтобы прям над персонажем
            emoji = emoji.scaled(36,36,Qt::KeepAspectRatio);
            player->player_message.emoji = new QGraphicsPixmapItem(emoji);
            player->player_message.emoji->setPos(player->pos().x() + 6, player->pos().y() - 48);
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
                    delete last_frame[i]->message;
                } else {
                    scene->removeItem(last_frame[i]->player_message.emoji); // тут скорее всего нужно удалять элемент
                    delete last_frame[i]->player_message.emoji;
                }
            }
        } else {
            clear_local_player = true;
        }
        delete last_frame[i];
    }
}



