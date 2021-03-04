#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include "Player.h"
#include "Client.h"
#include "HSL.h"
#include "Room.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class Room;
class Client;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event) override;
public slots:
    void on_h_slider_sliderMoved(int len);
    void on_s_slider_sliderMoved(int len);
    void on_l_slider_sliderMoved(int len);
    void on_connectButton_clicked();
    void createRoom(Player *player, QVector<PlayerView> players_);

public:
    Ui::MainWindow *ui;
    QColor circle_color = Qt::red;
    Room *room;
    Client *client;
    Player *player;
    QThread *client_thread;
};
#endif // MAINWINDOW_H
