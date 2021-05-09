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
#include <QDirIterator>
#include "Player.h"

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(QWidget* parent = 0);
    ChatWindow(QWidget* parent, Player &local_player, bool& close);

    void init_parameters();

    void init_buttons();
    void init_close_window_buttons();
    void init_black_white_emoji_buttons();
    void init_multicolor_emoji_buttons();

    ~ChatWindow();

public slots:
    QString GetText();

signals:
    void set_focus_room();

private:
    Player *player;
    QString *s;
    bool *close_window;
    QHBoxLayout *main_layout;
    QTextEdit *text_edit;
    QPushButton *cancel;
    QPushButton *enter_message;
    QToolButton *show_emoji_list_button;
    QToolButton *show_multicolor_emoji_list_button;
    QListWidget *show_emoji_list_widget;
    QListWidget *show_multicolor_emoji_list_widget;

    std::unordered_map<QString, QString> map_emoji = {
        {"1", "\U0001F600"},
        {"2", "\U0001F603"}
    };

    QVBoxLayout *lyt = new QVBoxLayout;

};
#endif // CHATWINDOW_H
