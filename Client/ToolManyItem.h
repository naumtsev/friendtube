
#ifndef TOOLMANYITEM_H
#define TOOLMANYITEM_H

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

class ToolManyItem : public QWidget
{
    Q_OBJECT

public:
    ToolManyItem(QWidget* parent = 0);
    ToolManyItem(QWidget* parent, Player &local_player);

    void init_parameters();

    void init_buttons();
    void init_multicolor_emoji_buttons();

    Player *player;

    QToolButton *show_multicolor_emoji_list_button;
    QListWidget *show_multicolor_emoji_list_widget;

signals:
    void set_focus_room();
};

#endif // TOOLMANYITEM_H
