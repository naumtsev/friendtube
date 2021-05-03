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
    void update_player();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ~PlayerView();

private:
    const sprite_data& csd() const;

public:
    //double x;
    //double y;
    QString color_player = ":/dino/pics/sheets/m_DinoSprites - mortBIG.png";
    QString client_id;
    Message player_message;
    //void draw(QPainter& painter);
    QString direction = "left"; // create enum class
    AnimateState state{AnimateState::Standing};

    int current_frame = 0;
    sprite_data spriteData;

    QVector<QPair<QPixmap, sprite_data>> Pixmaps;

    QGraphicsTextItem *name = new QGraphicsTextItem;
};

#endif // PLAYERVIEW_H
