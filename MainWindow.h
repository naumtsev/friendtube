#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui>
#include <QMainWindow>
#include <QInputDialog>
#include <QErrorMessage>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QPaintEvent>

#include "room.h"
#include "Player.h"

struct MainWindow : QWidget
{
    MainWindow();
    MainWindow(QString& name_player, QString& color);// конструктор, чтобы сохранять имя игрока

public slots:
    void enter_to_room();         // вход в комнату
    void change_player_name();    // замена имени пользователя

private:
    void keyReleaseEvent(QKeyEvent *)    override;  // отжимание клавиши
    void paintEvent(QPaintEvent *event)  override;

public:
    int mSelectRoomIndex;
    Player player;
    QTimer *timer;
};

void CreateMainWidget(QString player_name, QString color); // создание главного меню
#endif // MAINWINDOW_H
