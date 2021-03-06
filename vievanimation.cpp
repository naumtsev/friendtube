#include "vievanimation.h"
#include "ui_vievanimation.h"

VievAnimation::VievAnimation(QString color_dino)
    :dinozavr(color_dino)
{
    setRenderHint(QPainter::Antialiasing);

    setCacheMode(QGraphicsView::CacheNone);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setWindowTitle("FriendTube");

    setFrameStyle(0);
    setSceneRect(0, 0, 800, 800);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setFixedSize(800, 800);

    scene.setBackgroundBrush(Qt::lightGray); // меняет цвет фона
    //scene.setFont(/*меняем шрифт сцены*/)

    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    // тут может хавать event-ы
    setScene(&scene);
    init_view_elements();

}

void VievAnimation::init_view_elements(){
    dinozavr.setPos(0,100);
    //scene.addText(dinozavr.name);
    scene.addItem(dinozavr.name);
    scene.addItem(&dinozavr);
}

void VievAnimation::keyPressEvent(QKeyEvent* apKeyEvent){
    switch(apKeyEvent->key())
    {
        case Qt::Key_Left:
        {
          dinozavr.set_left_direction();
          dinozavr.move();
          break;
        }
        case Qt::Key_Right:
        {
          dinozavr.set_right_direction();
          dinozavr.move();
          break;
        }
        case Qt::Key_Up:
        {
          dinozavr.set_up_direction();
          dinozavr.move();
          break;
        }
        case Qt::Key_Down:
        {
          dinozavr.set_down_direction();
          dinozavr.move();
          break;
        }
        case Qt::Key_Space:
        {
          dinozavr.stop();
          break;
        }
    }
}
VievAnimation::~VievAnimation()
{
}

