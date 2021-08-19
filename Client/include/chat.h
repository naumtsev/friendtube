#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include "PlayerView.h"
#include "Message.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();

    void showMessage(const QString &message);
    void displayMessage(const QString &sender_name, const QString &message, const QString &color);
    void set_focus();
    void showAllNewMessages(QVector<PlayerView *>& players);
signals:
    void sendMessageToAllUsers(const QString& sender_name, const QString& message, const QString& color);
    void open_button_clicked();
private:
    Ui::Chat *ui;
};

#endif // CHAT_H
