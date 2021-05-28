
#ifndef TOOLMANYITEM_H
#define TOOLMANYITEM_H

#include <QDebug>
#include <QDirIterator>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>

#include "Player.h"

class ToolManyItem : public QWidget {
    Q_OBJECT

public:
    ToolManyItem(QWidget *parent = 0);
    ToolManyItem(QWidget *parent, Player &local_player);

    void init_parameters();

    void init_buttons();
    void init_multicolor_emoji_buttons();

    Player *player;

    QToolButton *show_multicolor_emoji_list_button;
    QListWidget *show_multicolor_emoji_list_widget;

signals:
    void set_focus_room();
};

#endif// TOOLMANYITEM_H
