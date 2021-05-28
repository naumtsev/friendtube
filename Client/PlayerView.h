#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "Message.h"
#include "Player.h"
#include <QWidget>

class PlayerView : public QGraphicsPixmapItem{


public:
    PlayerView() = default;
    PlayerView(Player& player);
    PlayerView(Player&& player);
    PlayerView(const PlayerView& player_view);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    void update_state();
    void update_direction();
    void update_position_name();
    void change_direction();


    ~PlayerView();

private:
    const sprite_data& csd() const;

public:
    //double x;
    //double y;
    int saturation;
    QString color_player = ":/dino/pics/sheets/m_DinoSprites - mortBIG.png";
    QString client_id;
    Message player_message;
    QString direction = "left"; // create enum class
    AnimateState state{AnimateState::Standing};

    int current_frame = 0;
    sprite_data spriteData;

    QVector<QPair<QPixmap, sprite_data>> Pixmaps;

    QGraphicsTextItem *name = new QGraphicsTextItem;
    QGraphicsTextItem *prefics_owner_video_name;
    QGraphicsTextItem *message = new QGraphicsTextItem;
};

#endif // PLAYERVIEW_H
