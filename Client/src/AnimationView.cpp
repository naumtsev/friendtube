#include "AnimationView.h"

#include <typeinfo>
#include <QFile>

// bush
GraphicsBush::GraphicsBush(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsBush::shape() const {
    QPainterPath path;
    path.addEllipse(15, 25, 0, 0);
    return path;
}
// bush

// bush_three
GraphicsThreeBush::GraphicsThreeBush(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsThreeBush::shape() const {
    QPainterPath path;
    path.addEllipse(20, 30, 2, 1);
    return path;
}
// bush_three

// tree
GraphicsTree::GraphicsTree(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsTree::shape() const {
    QPainterPath path;
    path.addEllipse(32, 100, 3, 3);
    return path;
}
// tree

// tree_two
GraphicsTreeTwo::GraphicsTreeTwo(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsTreeTwo::shape() const {
    QPainterPath path;
    path.addEllipse(53, 128, 20, 4);
    return path;
}
// tree_two

// stone
GraphicsStone::GraphicsStone(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsStone::shape() const {
    QPainterPath path;
    path.addEllipse(14, 23, 8, 1);
    return path;
}
// stone

// house
GraphicsHouse::GraphicsHouse(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsHouse::shape() const {
    QPainterPath path;
    path.addEllipse(0, 100, 166, 70);
    return path;
}
// house

// cafe
GraphicsCafe::GraphicsCafe(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsCafe::shape() const {
    QPainterPath path;
    path.addEllipse(0, 175, 166, 1);
    return path;
}
// cafe

// taverna
GraphicsTaverna::GraphicsTaverna(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsTaverna::shape() const {
    QPainterPath path;
    path.addEllipse(0, 100, 166, 70);
    return path;
}
// taverna

// house_man
GraphicsHouseMan::GraphicsHouseMan(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsHouseMan::shape() const {
    QPainterPath path;
    path.addEllipse(30, 30, 35, 50);
    return path;
}
// house_man

// cook
GraphicsCook::GraphicsCook(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsCook::shape() const {
    QPainterPath path;
    path.addEllipse(30, 30, 35, 50);
    return path;
}
// cook

// security
GraphicsSecurity::GraphicsSecurity(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsSecurity::shape() const {
    QPainterPath path;
    path.addEllipse(30, 40, 35, 35);
    return path;
}
// security

// food
GraphicsFood::GraphicsFood(QObject *parent) : QObject(parent) {}

QPainterPath GraphicsFood::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, 16, 16);
    return path;
}
// food

AnimationView::AnimationView(Room *room_, QWidget *parent)
    : QGraphicsView(parent) {
    room = room_;
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

    this->setHorizontalScrollBarPolicy(
            Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(
            Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding);

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);

    item_prefics_owner_name = new QGraphicsTextItem("[LIVE]");
    item_prefics_owner_name->setVisible(false);
    item_prefics_owner_name->setDefaultTextColor(Qt::red);
    scene->addItem(item_prefics_owner_name);

    this->setScene(scene);
    timer_update_scene = new QTimer();

    connect(timer_update_scene, SIGNAL(timeout()), this, SLOT(update()));
    timer_update_scene->start(3);

    init_background_item();
}

void AnimationView::init_background_item() {
    QBrush *ibrush = new QBrush;
    if (room->type == RoomType::Summer) {
        ibrush->setTextureImage(
                    QImage(":/images/background_summer.png"));
    }
    else if (room->type == RoomType::Strange) {
        ibrush->setTextureImage(
                    QImage(":/images/background_strange.png"));
    }
    scene->setBackgroundBrush(*ibrush);

    // houses

    // house
    QPixmap texture_house(
            ":/pics/background_item/green_room/more_texture/buildings/"
            "building2_1.png");
    GraphicsHouse *house = new GraphicsHouse;
    house->setPixmap(texture_house);
    house->setPos(15, 10);
    house->setZValue(+7);
    scene->addItem(house);

    GraphicsHouseMan *house_man = new GraphicsHouseMan;
    house_man->setPixmap(
            QPixmap(":/pics/background_item/green_room/house_man.png"));
    house_man->setPos(20, 150);
    house_man->setZValue(+8);
    scene->addItem(house_man);
    // house

    // taverna
    QPixmap texture_taverna(
            ":/pics/background_item/green_room/more_texture/buildings/"
            "building3_1.png");
    GraphicsTaverna *taverna = new GraphicsTaverna;
    taverna->setPixmap(texture_taverna);
    taverna->setPos(1080, 500);
    taverna->setZValue(+4);
    scene->addItem(taverna);

    QGraphicsPixmapItem *signboard = new QGraphicsPixmapItem(
            QPixmap(":/pics/background_item/green_room/more_texture/decor/sign.png"));
    signboard->setPos(taverna->x() + 63, taverna->y() + 63);
    signboard->setZValue(+10);
    scene->addItem(signboard);

    GraphicsSecurity *security = new GraphicsSecurity;
    security->setPixmap(
            QPixmap(":/pics/background_item/green_room/security.png"));
    security->setPos(taverna->x() - 60, taverna->y() + 75);
    security->setZValue(+4);
    scene->addItem(security);
    // taverna

    // cafe
    QPixmap texture_cafe(
            ":/pics/background_item/green_room/more_texture/buildings/"
            "building1_1.png");
    GraphicsCafe *cafe = new GraphicsCafe;
    cafe->setPixmap(texture_cafe);
    cafe->setPos(1080, 10);
    cafe->setZValue(+9);
    scene->addItem(cafe);

    GraphicsCook *cook = new GraphicsCook;
    cook->setPixmap(QPixmap(":/pics/background_item/green_room/cook.png"));
    cook->setPos(1100, 160);
    cook->setZValue(+9);
    scene->addItem(cook);
    // cafe

    // houses

    // по бокам ограда
    for (int i = 0; i < 26; i++) {// сверху
        QPixmap texture(":/pics/background_item/green_room/fence_12gorizontal.png");
        QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
        fence_vertical->setPos(48 * i + 3, 3);
        fence_vertical->setZValue(+5);
        scene->addItem(fence_vertical);
    }
    for (int i = 0; i < 26; i++) {// снизу
        QPixmap texture(":/pics/background_item/green_room/fence_12gorizontal.png");
        QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
        fence_vertical->setPos(48 * i + 3, 720 - 48 + 3);
        fence_vertical->setZValue(+5);
        scene->addItem(fence_vertical);
    }
    for (int i = 0; i < 14; i++) {// слева
        QPixmap texture(":/pics/background_item/green_room/fence_11vertikal.png");
        QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
        fence_vertical->setPos(0, 48 * i);
        fence_vertical->setZValue(+5);
        scene->addItem(fence_vertical);
    }
    for (int i = 0; i < 14; i++) {// справа
        QPixmap texture(":/pics/background_item/green_room/fence_11vertikal.png");
        QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
        fence_vertical->setPos(1280 - 24 - 8, 48 * i);
        fence_vertical->setZValue(+5);
        scene->addItem(fence_vertical);
    }
    // по бокам ограда

    // ограда плеера
    QString path_to_texture =
            ":/pics/background_item/green_room/tilestonebricks32.png";
    for (int i_1 = 0; i_1 < 13; i_1++) {
        for (int i = 9; i < 31; i++) {
            QPixmap texture(path_to_texture);
            QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
            fence_vertical->setPos(32 * i, 48 * 3 + 32 * i_1);
            scene->addItem(fence_vertical);
            if (i_1 == 0) {
                QGraphicsPixmapItem *grass = new QGraphicsPixmapItem(
                        QPixmap(":/pics/background_item/green_room/more_texture/land/"
                                "land_4_32px.png"));
                grass->setPos(fence_vertical->x(), fence_vertical->y());
                scene->addItem(grass);
            }
            if (i == 9) {
                QGraphicsPixmapItem *grass_vertical_left = new QGraphicsPixmapItem(
                        QPixmap(":/pics/background_item/green_room/more_texture/land/"
                                "land_5_32px.png"));
                grass_vertical_left->setPos(fence_vertical->x(), fence_vertical->y());
                scene->addItem(grass_vertical_left);
            }
            if (i == 30) {
                QGraphicsPixmapItem *grass_vertical_right = new QGraphicsPixmapItem(
                        QPixmap(":/pics/background_item/green_room/more_texture/land/"
                                "land_6_32px.png"));
                grass_vertical_right->setPos(fence_vertical->x(), fence_vertical->y());
                scene->addItem(grass_vertical_right);
            }
        }
    }

    for (int i = 9; i < 31; i++) {
        QPixmap texture(path_to_texture);
        QGraphicsPixmapItem *fence_vertical = new QGraphicsPixmapItem(texture);
        fence_vertical->setPos(32 * i, 720 - 48 * 3 - 24);
        scene->addItem(fence_vertical);

        QGraphicsPixmapItem *grass = new QGraphicsPixmapItem(QPixmap(
                ":/pics/background_item/green_room/more_texture/land/land_3_32px.png"));
        grass->setPos(32 * i, 720 - 48 * 3 - 24);
        scene->addItem(grass);

        if (i == 9) {
            QGraphicsPixmapItem *grass_vertical_left =
                    new QGraphicsPixmapItem(QPixmap(":/pics/background_item/green_room/"
                                                    "more_texture/land/land_5_32px.png"));
            grass_vertical_left->setPos(32 * i, 720 - 48 * 3 - 24);
            scene->addItem(grass_vertical_left);
        }

        if (i == 30) {
            QGraphicsPixmapItem *grass_vertical_right =
                    new QGraphicsPixmapItem(QPixmap(":/pics/background_item/green_room/"
                                                    "more_texture/land/land_6_32px.png"));
            grass_vertical_right->setPos(32 * i, 720 - 48 * 3 - 24);
            scene->addItem(grass_vertical_right);
        }
    }
    // ограда плеера


    QString json_string;
    QFile file;
    if (room->type == RoomType::Summer) {
        file.setFileName(":/json_pictures/all_summer.json");
    }
    else if (room->type == RoomType::Strange) {
        file.setFileName(":/json_pictures/all_strange.json");
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    json_string = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json_string.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray trees_array = json["trees"].toArray();
    QJsonArray bushes_array = json["bushes"].toArray();
    QJsonArray threebushes_array = json["threebushes"].toArray();
    QJsonArray stones_array = json["stones"].toArray();

    foreach (const QJsonValue& tree_value, trees_array) {
        if (tree_value.isObject()) {
            QJsonObject tree_obj = tree_value.toObject();
            GraphicsTreeTwo *megatree = new GraphicsTreeTwo;
            megatree->setPixmap(QPixmap(tree_obj["pixmap"].toString()));
            megatree->setZValue(+tree_obj["z"].toInt());
            megatree->setPos(tree_obj["x"].toInt(), tree_obj["y"].toInt());
            scene->addItem(megatree);
        }
    }

    foreach (const QJsonValue& bush_value, bushes_array) {
        if (bush_value.isObject()) {
            QJsonObject bush_obj = bush_value.toObject();
            GraphicsBush *bush = new GraphicsBush;
            bush->setPixmap(QPixmap(bush_obj["pixmap"].toString()));
            bush->setZValue(+bush_obj["z"].toInt());
            bush->setPos(bush_obj["x"].toInt(), bush_obj["y"].toInt());//180,200
            scene->addItem(bush);
        }
    }

    foreach (const QJsonValue& threebush_value, threebushes_array) {
        if (threebush_value.isObject()) {
            QJsonObject threebush_obj = threebush_value.toObject();
            GraphicsThreeBush *threebush = new GraphicsThreeBush;
            threebush->setPixmap(QPixmap(threebush_obj["pixmap"].toString()));
            threebush->setZValue(+threebush_obj["z"].toInt());
            threebush->setPos(threebush_obj["x"].toInt(), threebush_obj["y"].toInt());//140,200
            scene->addItem(threebush);
        }
    }

    foreach (const QJsonValue& stone_value, stones_array) {
        if (stone_value.isObject()) {
            QJsonObject stone_obj = stone_value.toObject();
            GraphicsStone *stone = new GraphicsStone;
            stone->setPixmap(QPixmap(stone_obj["pixmap"].toString()));
            stone->setZValue(+stone_obj["z"].toInt());
            stone->setPos(stone_obj["x"].toInt(), stone_obj["y"].toInt());
            scene->addItem(stone);
        }
    }
}

void AnimationView::add_players(QVector<PlayerView *> &last_frame,
                                QVector<PlayerView *> &next_frame,
                                QString local_id) {
    if (next_frame.size() > 1) {
        bool draw_local_player = false;
        int end_player = next_frame.size() - 1;

        for (int i = 0; i < next_frame.size(); i++) {
            if (next_frame[i]->client_id != local_id || draw_local_player) {
                next_frame[i]->update_state();
                scene->addItem(next_frame[i]);
                if (i != next_frame.size() - 1) {
                    next_frame[i]->name->setDefaultTextColor(Qt::white);
                    scene->addItem(next_frame[i]->name);
                    if (room->is_owner_video(next_frame[i]->client_id)) {
                        item_prefics_owner_name->setVisible(true);
                        item_prefics_owner_name->setPos(next_frame[i]->name->x() - 40,
                                                        next_frame[i]->name->y());
                    }
                    display_message(next_frame[i]);
                }
            } else {
                draw_local_player = true;
            }
        }

        clear_vector(last_frame, local_id);

        int i = 0;

        while (colliding_with_player(next_frame) > 0) {
            next_frame[end_player]->setPos(
                    local_player->position_movement_last_frame[i].first,
                    local_player->position_movement_last_frame[i].second);
            next_frame[end_player]->name->setPos(
                    local_player->position_name_movement_last_frame[i].first,
                    local_player->position_name_movement_last_frame[i].second);

            local_player->setPos(
                    local_player->position_movement_last_frame[i].first,
                    local_player->position_movement_last_frame[i].second);

            local_player->name->setPos(
                    local_player->position_name_movement_last_frame[i].first,
                    local_player->position_name_movement_last_frame[i].second);
            i++;
        }

        next_frame[end_player]->update_state();
        add_tables(next_frame[end_player]);
        next_frame[end_player]->name->setDefaultTextColor(Qt::white);
        scene->addItem(next_frame[end_player]->name);
        if (room->is_owner_video(next_frame[end_player]->client_id)) {
            item_prefics_owner_name->setVisible(true);
            item_prefics_owner_name->setPos(next_frame[end_player]->name->x() - 40,
                                            next_frame[end_player]->name->y());
        }
        display_message(next_frame[end_player]);

        local_player->position_movement_last_frame.clear();
        local_player->position_name_movement_last_frame.clear();

        last_frame = std::move(next_frame);
    }
    next_frame.clear();
}

void AnimationView::add_tables(PlayerView *player) {
    if (sleep.xl <= player->x() && player->x() <= sleep.xr &&
        sleep.yl <= player->y() && player->y() <= sleep.yr) {
        tablet_want_sleap =
                new QGraphicsPixmapItem(QPixmap(":/pics/background_item/green_room/"
                                                "more_texture/tablet_want_sleep.png"));
        tablet_want_sleap->setPos(50, 140);
        tablet_want_sleap->setZValue(+15);
        scene->addItem(tablet_want_sleap);
    } else if (want_or_dont_eat.xl <= player->x() &&
               player->x() <= want_or_dont_eat.xr &&
               want_or_dont_eat.yl <= player->y() &&
               player->y() <= want_or_dont_eat.yr) {
        if (local_player->saturation >= 100) {
            table_stop_eating = new QGraphicsPixmapItem();
            table_stop_eating->setPos(1130, 150);
            table_stop_eating->setPixmap(
                    QPixmap(":/pics/background_item/green_room/more_texture/"
                            "tablet_stor_eating.png"));
            table_stop_eating->setZValue(+15);
            scene->addItem(table_stop_eating);
        } else {
            tablet_want_eating = new QGraphicsPixmapItem();
            tablet_want_eating->setPos(1130, 150);
            tablet_want_eating->setPixmap(
                    QPixmap(":/pics/background_item/green_room/more_texture/"
                            "tablet_want_eat.png"));
            tablet_want_eating->setZValue(+15);
            scene->addItem(tablet_want_eating);
        }
    } else if (enter_close_to_taverna.xl <= player->x() &&
               player->x() <= enter_close_to_taverna.xr &&
               enter_close_to_taverna.yl <= player->y() &&
               player->y() <= enter_close_to_taverna.yr) {
        tablet_stop = new QGraphicsPixmapItem();
        tablet_stop->setPos(1045, 570);
        tablet_stop->setPixmap(
                QPixmap(":/pics/background_item/green_room/more_texture/"
                        "tablet_entry_close.png"));
        tablet_stop->setZValue(+15);
        scene->addItem(tablet_stop);
    } else if (first_course_alive.xl <= player->x() &&
               player->x() <= first_course_alive.xr &&
               first_course_alive.yl <= player->y() &&
               player->y() <= first_course_alive.yr) {
        tablet_fist_course_alive = new QGraphicsPixmapItem();
        tablet_fist_course_alive->setPos(50, 600);
        tablet_fist_course_alive->setPixmap(
                QPixmap(":/pics/background_item/green_room/more_texture/"
                        "table_first_course_alive.png"));
        tablet_fist_course_alive->setZValue(+15);
        scene->addItem(tablet_fist_course_alive);
    }
    if (ay.xl <= player->x() && player->x() <= ay.xr && ay.yl <= player->y() &&
        player->y() <= ay.yr) {
        tablet_ay = new QGraphicsPixmapItem();
        tablet_ay->setPos(80, 620);
        tablet_ay->setPixmap(
                QPixmap(":/pics/background_item/green_room/more_texture/table_ay.png"));
        tablet_ay->setZValue(+15);
        scene->addItem(tablet_ay);
    }
}

void AnimationView::delete_tables(PlayerView *player) {
    if (sleep.xl <= player->x() && player->x() <= sleep.xr &&
        sleep.yl <= player->y() && player->y() <= sleep.yr) {
        scene->removeItem(tablet_want_sleap);
        delete tablet_want_sleap;
    } else if (want_or_dont_eat.xl <= player->x() &&
               player->x() <= want_or_dont_eat.xr &&
               want_or_dont_eat.yl <= player->y() &&
               player->y() <= want_or_dont_eat.yr) {
        if (player->saturation >= 100) {
            scene->removeItem(table_stop_eating);
            delete table_stop_eating;
        } else {
            scene->removeItem(tablet_want_eating);
            delete tablet_want_eating;
        }
    } else if (enter_close_to_taverna.xl <= player->x() &&
               player->x() <= enter_close_to_taverna.xr &&
               enter_close_to_taverna.yl <= player->y() &&
               player->y() <= enter_close_to_taverna.yr) {
        scene->removeItem(tablet_stop);
        delete tablet_stop;
    } else if (first_course_alive.xl <= player->x() &&
               player->x() <= first_course_alive.xr &&
               first_course_alive.yl <= player->y() &&
               player->y() <= first_course_alive.yr) {
        scene->removeItem(tablet_fist_course_alive);
        delete tablet_fist_course_alive;
    }
    if (ay.xl <= player->x() && player->x() <= ay.xr && ay.yl <= player->y() &&
        player->y() <= ay.yr) {
        scene->removeItem(tablet_ay);
        delete tablet_ay;
    }
}

int AnimationView::colliding_with_player(QVector<PlayerView *> &next_frame) {
    int id = next_frame.size() - 1;
    int count = 0;
    QList<QGraphicsItem *> colliding = scene->collidingItems(next_frame[id]);
    for (QGraphicsItem *item : colliding) {
        int count_i = 1;
        if (dynamic_cast<GraphicsFood *>(item)) {
            count_i = 0;
            delete item;
        }
        if (dynamic_cast<QGraphicsTextItem *>(item)) {
            count_i = 0;
        }
        for (int i = 0; i <= id; i++) {
            if (item == next_frame[i]) {
                count_i = 0;
            }
            if (item == next_frame[i]->name) {
                count_i = 0;
            }
            if (next_frame[i]->player_message.type == "emoji" &&
                item == next_frame[i]->player_message.emoji) {
                count_i = 0;
            }
            if (count_i == 0) {
                break;
            }
        }
        count += count_i;
    }
    return count;
}

void AnimationView::display_message(PlayerView *player) {
    if (player->player_message.send_message != "") {
        if (player->player_message.type == "text") {
            player->message->setPos(
                    player->pos().x() + 24 - player->message->boundingRect().width() / 2,
                    player->pos().y() -
                            30);
            player->message->setDefaultTextColor(Qt::white);
            scene->addItem(player->message);
        } else {
            QPixmap emoji(player->player_message.send_message);
            emoji = emoji.scaled(36, 36, Qt::KeepAspectRatio);
            player->player_message.emoji = new QGraphicsPixmapItem(emoji);
            player->player_message.emoji->setPos(player->pos().x() + 6,
                                                 player->pos().y() - 48);
            scene->addItem(player->player_message.emoji);
        }
    }
}

void AnimationView ::clear_vector(QVector<PlayerView *> &last_frame,
                                  QString local_id) {
    bool clear_local_player = false;
    bool watch_video = false;
    for (int i = 0; i < last_frame.size(); i++) {
        if (last_frame[i]->client_id != local_id || clear_local_player) {
            scene->removeItem(last_frame[i]);
            scene->removeItem(last_frame[i]->name);
            if (room->is_owner_video(last_frame[i]->client_id)) {
                watch_video = true;
            }
            if (last_frame[i]->player_message.send_message != "") {
                if (last_frame[i]->player_message.type == "text") {
                    scene->removeItem(last_frame[i]->message);
                } else {
                    scene->removeItem(last_frame[i]->player_message.emoji);
                    delete last_frame[i]->player_message.emoji;
                }
            }
        } else {
            delete_tables(last_frame[last_frame.size() - 1]);
            clear_local_player = true;
        }
        delete last_frame[i];
    }
    if (!watch_video) {
        item_prefics_owner_name->setVisible(false);
    }
}
