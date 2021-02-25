#include "room.h"
#include "player_data.h"
#include "ui_room.h"

QVector<player_data> player_from_the_room;

Room::Room(const QString& player_name,const QString& color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    this->resize(800,800);
    this->setFixedSize(800,800);
    //this->showFullScreen();

    scene = new QGraphicsScene();
    player = new Player{player_name, color};        // создаём персонажа
    player_data player_data_new(*player);             // создаём урезанную копию для вектора
    player_from_the_room.push_back(player_data_new);
    /*)
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
*/ // чекнуть, как можно сделать, чтоыб

}
void Room::paintEvent(QPaintEvent *event){ // event сам и не нужен
    qDebug() << "paintEvent";
    QPainter painter(this);
    update_position_local_player(); // обновляем позицию игрока
    for(std::size_t i = 0; i < player_from_the_room.size(); i++){
        if(player_from_the_room[i].client_id == player->client_id){
            if(player->player_message.metka_message && !player->player_message.metka_message_painter){
                player->player_message.metka_message_painter = true;
                player->timer_message->stop();
                player->timer_message->start(5000);
                connect(player->timer_message, SIGNAL(timeout()), this, SLOT(no_message()));
            }
            if(player->player_message.metka_message_painter){
                painter.drawText(-55 + player->x,-40 + player->y,110,20,Qt::AlignRight,player->player_message.send_message);
            }
        }
        QPolygon polygon({QPoint(-25 + player->x, -25 +  player->y), QPoint(25 +  player->x, -25 +  player->y), QPoint( 25 +  player->x, 25 +  player->y), QPoint(-25 +  player->x, 25 +  player->y)}); //рисуем квадрат
        painter.setBrush(Qt::red);                                     //задаём цвет квадрата
        painter.drawPolygon(polygon);                                  //рисуем персонажа TODO: Будем рисовать текстуры
        painter.drawText(-55 + player->x,25 + player->y,110,20,Qt::AlignCenter,player->player_name);   //отображение имени под персонажем + выравнивание посередине
        //хочу canvas.drawtext сюда впихнуть!!! чтобы можно было норм текст рисовать.
        //http://developer.alexanderklimov.ru/android/catshop/android.graphics.canvas.php#drawtext
    }
}

void Room::update_position_local_player(){
    for(int i = 0; i < player_from_the_room.size(); i++){
        if(player_from_the_room[i].client_id == player->client_id){
            player_from_the_room[i].x = player->x;
            player_from_the_room[i].y = player->y;
            player_from_the_room[i].player_message = player->player_message;
            break;
        }
    }
}

void Room::keyPressEvent(QKeyEvent *apKeyEvent)
{
    qDebug() << "KeyPressEvent";
    update();
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
                    player->player_message.send_message = str;
                    player->player_message.metka_message = true;
                    player->player_message.metka_message_painter = false;
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

