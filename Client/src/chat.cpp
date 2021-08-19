#include "chat.h"
#include "ui_chat.h"
#include <QDebug>

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::showMessage(const QString &message) {
    ui->textEdit->append(message);
}

void Chat::set_focus() {
    this->show();
}

void Chat::showAllNewMessages(QVector<PlayerView *> &players) {
    for (auto & player : players) {
        if (player->player_message.metka_message && !player->player_message.metka_message_painter) {

            showMessage(player->name->toPlainText() + ": " + player->player_message.send_message);
            qDebug() << "metka false\n";
//            player->player_message.is_new = false;
        }
    }
}

void Chat::displayMessage(const QString &sender_name, const QString &message, const QString &color) {
    if (color == "red") ui->textEdit->append("<font color=red>" + sender_name + "</font>" + ": " + message);
    if (color == "blue") ui->textEdit->append("<font color=blue>" + sender_name + "</font>" + ": " + message);
    if (color == "green") ui->textEdit->append("<font color=green>" + sender_name + "</font>" + ": " + message);
    if (color == "yellow") ui->textEdit->append("<font color=yellow>" + sender_name + "</font>" + ": " + message);
    qDebug() << "message_shown";
}
