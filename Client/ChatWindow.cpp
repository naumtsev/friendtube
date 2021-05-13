#include "ChatWindow.h"

ChatWindow::ChatWindow(QWidget *parent): QWidget(parent){

}
ChatWindow::ChatWindow(QWidget *parent, Player &local_player)
    : QWidget(parent)
{
    player = &local_player;

    init_parameters();
    init_text_edit();
    init_buttons();
}

void ChatWindow::keyPressEvent(QKeyEvent *apKeyEvent){
    if(apKeyEvent->key() == Qt::Key_Enter || apKeyEvent->key() == 16777220){ // походу у меня enter нет (
        enter_message->clicked();
    }
}

void ChatWindow::init_parameters(){
    //setWindowTitle("Chat");
    setWindowFlags(Qt::FramelessWindowHint); // окно без рамки
    this->setFixedSize(400, 230);
    this->setGeometry(440, 0, 400, 230); // лучше этот размер в room выставлять, чтобы ориентироваться на размер виджета room
    setWindowOpacity( 0.8 ); // делаем полупрозрачным
    this->hide();
}

void ChatWindow::get_focus(){
    this->show();
    text_edit->setFocus();
}

void ChatWindow::lose_focus(){
    this->hide();
}

void ChatWindow::init_buttons(){
    init_text_edit();
    init_close_window_buttons();

}

void ChatWindow::init_text_edit(){
    text_edit = new QLineEdit(this);
    text_edit->setFixedSize(310,30);
    text_edit->setGeometry(0,0,310,30);

}
void ChatWindow::init_close_window_buttons(){

    enter_message = new QPushButton(this);
    enter_message->setCursor(Qt::PointingHandCursor);
    enter_message->setToolTip("Text");
    enter_message->setIcon(QIcon(":/pics/emoji/sending_message/send.png"));
    enter_message->setIconSize(QSize(30, 30));
    enter_message->setFixedSize(enter_message->iconSize());
    enter_message->setGeometry(text_edit->pos().x() + text_edit->width(), 0, enter_message->width(), enter_message->height());
    connect(enter_message, &QPushButton::clicked, [=](){
        player->player_message.type = "text";
        player->player_message.send_message = text_edit->text();
        player->message->setPlainText(player->player_message.send_message);
        player->player_message.metka_message = true;
        player->player_message.metka_message_painter = false;
        qDebug() << s;
        player->movement = {0,0};
        text_edit->clear();
        // тут нужно фокус на Room делать
        emit set_focus_room();
    });
}

ChatWindow::~ChatWindow()
{
}

