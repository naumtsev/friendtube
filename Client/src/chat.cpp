#include "Chat.h"
#include "ui_chat.h"
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

void Chat::set_focus() {
    this->show();
}

void Chat::displayMessage(const QString &sender_name, const QString &message, const QString &color) {
    if (color == "red") ui->textEdit->append("<font color=red>" + sender_name + "</font>" + ": " + message);
    if (color == "blue") ui->textEdit->append("<font color=blue>" + sender_name + "</font>" + ": " + message);
    if (color == "green") ui->textEdit->append("<font color=green>" + sender_name + "</font>" + ": " + message);
    if (color == "yellow") ui->textEdit->append("<font color=yellow>" + sender_name + "</font>" + ": " + message);
}
