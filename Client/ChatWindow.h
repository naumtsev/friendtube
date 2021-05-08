#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <unordered_map>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QGridLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QWidget>
#include <QDebug>
#include "Player.h"

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(Player &local_player, bool& close);
    ~ChatWindow();

public slots:
    QString GetText();

private:
    Player *player;
    QString *s;
    bool *close_window;
    QHBoxLayout *main_layout;
    QTextEdit *text_edit;
    QPushButton *cancel;
    QPushButton *enter_message;
    QToolButton *show_emoji_list_button;
    QListWidget *show_emoji_list_widget;

    std::unordered_map<QString, QString> map_emoji = {
        {"1", "\U0001F600"},
        {"2", "\U0001F603"}
    };

    QVBoxLayout *lyt = new QVBoxLayout;

};
#endif // CHATWINDOW_H
