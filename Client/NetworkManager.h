#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Room.h"
#include "Menu.h"
#include "PlayerView.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMutex>
#include <QMutexLocker>
#include <thread>
#include <QWebSocket>

struct Menu;
struct Client;

struct NetworkManager : public QObject {
    Q_OBJECT

public:
    NetworkManager(Client *client_, const QString &ip_, int port_);
    ~NetworkManager();
    bool connect_to_server(QString ip, int port);


signals:
    void createRoom(Player*, QVector<PlayerView *>);
    void disconnect(const QString &reason);

public slots:
    void run();
    void socketReady(const QByteArray &data);
    void socketDisconnect();
    void request_get_scene_on_the_server();
    void update_state_on_the_server(const QJsonDocument &state);
    void onConnected();
    void onWebSocketError(QAbstractSocket::SocketError);

public:
    void sendData(const QByteArray &data);
    QWebSocket *socket;
    QString client_id;
    Client *client;
    QMutex *socket_mutex;
    QString ip;
    int port;

};

#endif // NETWORKMANAGER_H
