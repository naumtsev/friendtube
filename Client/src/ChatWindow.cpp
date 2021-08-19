#include "ChatWindow.h"

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent) {}
ChatWindow::ChatWindow(QWidget *parent, Player &local_player)
    : QWidget(parent) {
    player = &local_player;

    init_parameters();
    init_text_edit();
    init_buttons();
}

void ChatWindow::keyPressEvent(QKeyEvent *apKeyEvent) {
    if (apKeyEvent->key() == Qt::Key_Enter ||
        apKeyEvent->key() == 16777220) {
        emit enter_message->clicked();
    }
}

void ChatWindow::init_parameters() {
    setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(400, 50);
    this->setGeometry(440, 0, 400,
                      30);
    setWindowOpacity(0.8);
    this->hide();
}

void ChatWindow::get_focus() {
    this->show();
    text_edit->setFocus();
}

void ChatWindow::lose_focus() { this->hide(); }

void ChatWindow::init_buttons() {
    init_text_edit();
    init_close_window_buttons();
}

void ChatWindow::init_text_edit() {
    text_edit = new QLineEdit(this);
    text_edit->setFixedSize(370, 30);
    text_edit->setGeometry(0, 0, 370, 30);
}
void ChatWindow::init_close_window_buttons() {
    enter_message = new QPushButton(this);
    enter_message->setCursor(Qt::PointingHandCursor);
    enter_message->setToolTip("Text");
    enter_message->setIcon(QIcon(":/pics/emoji/sending_message/send.png"));
    enter_message->setIconSize(QSize(30, 30));
    enter_message->setFixedSize(enter_message->iconSize());
    enter_message->setGeometry(text_edit->pos().x() + text_edit->width(), 0,
                               enter_message->width(), enter_message->height());
    connect(enter_message, &QPushButton::clicked, [=]() {
        player->player_message.type = "text";
        player->player_message.send_message = text_edit->text();
        player->player_message.sender_name = player->name->toPlainText();
        player->message->setPlainText(player->player_message.send_message);
//        qDebug() << "metka_true\n";
//        player->player_message.is_new = true;
        player->player_message.metka_message = true;
        player->player_message.metka_message_painter = false;
        player->movement = {0, 0};
        qDebug() << "message_sent";
        if (player->color_player == ":/pics/sheets/m_DinoSprites - mortBIG.png") emit chat->sendMessageToAllUsers(player->player_message.sender_name, player->player_message.send_message, "red");
        if (player->color_player == ":/pics/sheets/m_DinoSprites - douxBIG.png") emit chat->sendMessageToAllUsers(player->player_message.sender_name, player->player_message.send_message, "blue");
        if (player->color_player == ":/pics/sheets/m_DinoSprites - vitaBIG.png") emit chat->sendMessageToAllUsers(player->player_message.sender_name, player->player_message.send_message, "green");
        if (player->color_player == ":/pics/sheets/m_DinoSprites - tardBIG.png") emit chat->sendMessageToAllUsers(player->player_message.sender_name, player->player_message.send_message, "yellow");
        text_edit->clear();
        emit set_focus_room();
    });
}

ChatWindow::~ChatWindow() {}


//Added

ChatWindow::ChatWindow(QWidget *parent, Player &local_player, Chat &local_chat)
    : QWidget(parent) {
    player = &local_player;
    chat = &local_chat;

    init_parameters();
    init_text_edit();
    init_buttons();
}
