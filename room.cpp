#include "room.h"
#include "ui_room.h"

Room::Room(const QString& player_name,const QString& color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    this->resize(800,800);
    this->setFixedSize(800,800);
    //this->showFullScreen();

    scene = new QGraphicsScene();
    player = new Player{player_name, color};

    ui->graphicsView->setScene(scene); // забирала
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus); // TODO: чекнуть почему заработало?

    scene->setSceneRect(0, 0, 0, 0);

    scene->addLine(-300,0,300,0,QPen(Qt::black));
    scene->addLine(0,-300,0,300,QPen(Qt::black));

    scene->addLine(-350,-350, 350,-350, QPen(Qt::black));
    scene->addLine(-350, 350, 350, 350, QPen(Qt::black));
    scene->addLine(-350,-350,-350, 350, QPen(Qt::black));
    scene->addLine( 350,-350, 350, 350, QPen(Qt::black));


    scene->addItem(player);
    player->setPos(0,0);

}



void Room::keyPressEvent(QKeyEvent *apKeyEvent)
{
    qDebug() << "KeyPressEvent";
    if(apKeyEvent->key() == Qt::Key_Escape) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "", "Do you want to leave?",
                              QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            CreateMainWidget(player->player_name, player->color_square);
            this->close();
        }
    }else if(apKeyEvent->key() == Qt::Key_Q){
        player->movement = {0,0};                       // при вводе сообщения игрок останавливается

        while(true){
            bool click = false;
            QString message = "Your message....";
            QString str = QInputDialog::getText(0, "Enter message!", message, QLineEdit::Normal, "", &click);
            if(!click){ // была нажата кнопка Cancel
                break;
            }
            if(str.size() == 0){ // если ничего не ввёл, то ничего непроизошло
                break;
            }else if(0 < str.size() && str.size() < 17 && str!="") {
                    player->message = str;
                    player->metka_message = true;
                    player->metka_message_painter = false;
                    player->update(); // нужно для перерисовки TODO: тесты!!!, может и не нужна строчка
                    break;
            }// TODO: эксперименты с размером сообщения и если нужно, то отредактировать
        }
    }else {
        player->keyPressEvent(apKeyEvent);
    }
}

void Room::keyReleaseEvent(QKeyEvent *apKeyEvent){
    qDebug() << "KeyReleaseEvent";

    player->keyReleaseEvent(apKeyEvent);
}


Room::~Room()
{
    delete ui;
}

