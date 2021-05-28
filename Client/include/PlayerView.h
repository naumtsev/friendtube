#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QWidget>

#include "Message.h"
#include "Player.h"

class PlayerView : public QGraphicsPixmapItem {
public:
    PlayerView() = default;
    PlayerView(Player &player);
    PlayerView(Player &&player);
    PlayerView(const PlayerView &player_view);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    void update_state();
    void update_direction();
    void update_position_name();
    void change_direction();

    ~PlayerView();

private:
    const sprite_data &csd() const;

public:
    int saturation;
    bool owner_video = false;
    QString color_player = ":/dino/pics/sheets/m_DinoSprites - mortBIG.png";
    QString client_id;
    Message player_message;
    QString direction = "left";
    AnimateState state{AnimateState::Standing};

    int current_frame = 0;
    sprite_data spriteData;

    QVector<QPair<QPixmap, sprite_data>> Pixmaps;

    QGraphicsTextItem *name = new QGraphicsTextItem;
    QGraphicsTextItem *prefics_owner_video_name;
    QGraphicsTextItem *message = new QGraphicsTextItem;
};

#endif// PLAYERVIEW_H
