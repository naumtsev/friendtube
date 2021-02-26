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

class MainWindow;

class Client : QObject {
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();
    bool connect_to_server(QString ip, int port);
    void update_state_on_the_server(QJsonDocument state);
    void request_get_scene_on_the_server();

public slots:
    void socketReady();
    void socketDisconnect();
public:
     QTcpSocket* socket;
     QString client_id;
     MainWindow * main_window;
};

#endif // CLIENT_H
