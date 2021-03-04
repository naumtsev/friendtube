#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include "Room.h"
#include "mainwindow.h"
#include "PlayerView.h"
#include <QMutex>
#include <QMutexLocker>
#include <thread>

class MainWindow;

class Client : public QObject {
    Q_OBJECT

public:
    Client(QString ip_, int port_, QWidget *parent = nullptr);
    ~Client();
    bool connect_to_server(QString ip, int port);



signals:
    void createRoom(Player*, QVector<PlayerView>);


public slots:
    void run();
    void socketReady();
    void socketDisconnect();
    void request_get_scene_on_the_server();
    void update_state_on_the_server(QJsonDocument state);
public:
     QTcpSocket* socket;
     QString client_id;
     MainWindow * main_window;
     QMutex socket_mutex;
     QString ip;
     int port;

};

#endif // CLIENT_H
