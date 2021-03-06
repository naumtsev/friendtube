#include "dino.h"

Dino::Dino()
{
    /*
    Pixmaps.reserve((int)AnimateState::StateEnd);
    QPair<QPixmap, spriteData> tmp;

    tmp.first.load(color_dino);               // standing (в этом конструкторе не определен color_dino, поэтому не работает)
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 2;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);

    tmp.first.load(color_dino);                // move
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 8;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);



    connect(&timer, &QTimer::timeout, this, &Dino::next_frame);
    timer.start(100);
    */
}

Dino::Dino(QString color_dino){

    Pixmaps.reserve((int)AnimateState::StateEnd);
    QPair<QPixmap, spriteData> tmp;

    tmp.first.load(color_dino);                // standing
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 2;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);

    tmp.first.load(color_dino);                // move
    tmp.second.width = 48;
    tmp.second.height = 48;
    tmp.second.current_frame = 0;
    tmp.second.frames = 8;
    tmp.second.offset = 48;
    tmp.second.border = 0;
    Pixmaps.push_back(tmp);



    connect(&timer, &QTimer::timeout, this, &Dino::next_frame);
    timer.start(100);
}


QRectF Dino::boundingRect() const{
    return QRectF(0,0,48,48);
}

void Dino::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(0,0,Pixmaps[static_cast<int>(state)].first,(csd().offset*current_frame + csd().border),0,csd().width,csd().height);
}

const Dino::spriteData& Dino::csd() const {
    return Pixmaps[static_cast<int>(state)].second;
}

void Dino::next_frame(){
    switch(state){
        case Dino::AnimateState::Moving:
        {
            move();
            current_frame++;
            if(current_frame >= Pixmaps[static_cast<int>(state)].second.frames){
                current_frame = 4;
            }
            break;
        }
        case Dino::AnimateState::Standing :
        case Dino::AnimateState::StateEnd : {
            current_frame++;
            if(current_frame >= Pixmaps[static_cast<int>(state)].second.frames){
                current_frame = 0;
            }
            break;
        }
    }


    QGraphicsItem::update();
}
void Dino::stop(){
    state = AnimateState::Standing;
    current_frame = 0;
    Dx = 0;
    Dy = 0;
}
void Dino::move(){
    setPos(pos().x() + Dx, pos().y() + Dy);
    state = AnimateState::Moving;
}
void Dino::set_left_direction(){
    //setTransform(QTransform::fromScale(-1, 1));
    //Dx = -2;
    if(direction == "left")
      {
        // Get the current position
        QRectF itemRectOld = this->sceneBoundingRect();

        // Get the current transform
        QTransform transform(this->transform());
        qreal m11 = transform.m11(); // Horizontal scaling
        qreal m12 = transform.m12(); // Vertical shearing
        qreal m13 = transform.m13(); // Horizontal Projection
        qreal m21 = transform.m21(); // Horizontal shearing
        qreal m22 = transform.m22(); // vertical scaling
        qreal m23 = transform.m23(); // Vertical Projection
        qreal m31 = transform.m31(); // Horizontal Position (DX)
        qreal m32 = transform.m32(); // Vertical Position (DY)
        qreal m33 = transform.m33(); // Addtional Projection Factor

        // Horizontal flip
        m11 = -m11;

        // Write back to the matrix
        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

        // Set the items transformation
        setTransform(transform);

        // Re-position back to origin
        QRectF itemRectNew = this->sceneBoundingRect();
        qreal dx = itemRectOld.center().x()-itemRectNew.center().x();
        qreal dy = itemRectOld.center().y()-itemRectNew.center().y();
        this->moveBy(dx, dy);

      }
    Dx = -2;
    direction = "right";
}
void Dino::set_right_direction(){
    //setTransform(QTransform::fromScale(1, 1));
    //Dx =  2;
    if(direction == "right")
      {
        //setTransform(QTransform::fromScale(1, 1));
        // Get the current position
        QRectF itemRectOld = this->sceneBoundingRect();

        // Get the current transform
        QTransform transform(this->transform());
        qreal m11 = transform.m11(); // Horizontal scaling
        qreal m12 = transform.m12(); // Vertical shearing
        qreal m13 = transform.m13(); // Horizontal Projection
        qreal m21 = transform.m21(); // Horizontal shearing
        qreal m22 = transform.m22(); // vertical scaling
        qreal m23 = transform.m23(); // Vertical Projection
        qreal m31 = transform.m31(); // Horizontal Position (DX)
        qreal m32 = transform.m32(); // Vertical Position (DY)
        qreal m33 = transform.m33(); // Addtional Projection Factor

        // Horizontal flip
        m11 = -m11;

        // Write back to the matrix
        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

        // Set the items transformation
        setTransform(transform);

        // Re-position back to origin
        QRectF itemRectNew = this->sceneBoundingRect();
        qreal dx = itemRectOld.center().x()-itemRectNew.center().x();
        qreal dy = itemRectOld.center().y()-itemRectNew.center().y();
        this->moveBy(dx, dy);

      }
    Dx = 2;
    direction = "left";
}
void Dino::set_up_direction(){
    Dy = -2;
}
void Dino::set_down_direction(){
    Dy =  2;
}

