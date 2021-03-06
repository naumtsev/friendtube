#ifndef VIEVANIMATION_H
#define VIEVANIMATION_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "dino.h"
class VievAnimation : public QGraphicsView
{
public:
    VievAnimation(QString color_dino);

    ~VievAnimation();
    void keyPressEvent(QKeyEvent* apKeyEvent) override;

private:
    void init_view_elements();
private:
    QGraphicsScene scene;
    Dino dinozavr;
};
#endif // VIEVANIMATION_H
