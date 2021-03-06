#ifndef DINO_H
#define DINO_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QString>

class Dino : public QObject, public QGraphicsItem
{
    Q_OBJECT

    enum class AnimateState{
        Standing = 0,
        Moving,
        StateEnd
    };

    struct spriteData{
        int width = 0;
        int height = 0;
        int current_frame = 0;
        int frames = 0;
        int offset = 0;
        int border = 0;
    };

public:
    Dino();
    Dino(QString color_dino);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void move();
    void stop();

    void set_left_direction();
    void set_right_direction();
    void set_up_direction();
    void set_down_direction();


private:
    const spriteData& csd() const;

private slots:
    void next_frame();

private:
    qreal Dx = 0;
    qreal Dy = 0;
    QString direction = "left"; // create enum class
    AnimateState state{AnimateState::Standing};

    int current_frame = 0;
    QTimer timer;

    QVector<QPair<QPixmap, spriteData>> Pixmaps;
    QString color_dino;
public:
    QGraphicsTextItem *name = new QGraphicsTextItem;
    //QString name = "Drago";
};

#endif // DINO_H
