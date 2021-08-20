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
    ui->textEdit->append("<font color=" + color + ">" + sender_name + "</font>" + ": " + message);
}
