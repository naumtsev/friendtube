#ifndef SERVER_H
#define SERVER_H

#include "SocketThread.h"
#include "JsonHandler.h"
#include <QTcpServer>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QMutex>
#include <QMutexLocker>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"

class SocketThread;

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);
    void startServer();
    void send_data_to_all_users(QString data);
    ~Server();
    void socket_disconnected(SocketThread* socket);
    QJsonObject get_scene_data();
signals:

public slots:
  void incomingConnection();

public:
    QList<SocketThread*> sockets;
    QWebSocketServer *web_socket_server;
    qint16 port;
    const qint16 client_id_size = 10;
    QMutex data_mutex;
};

#endif // SERVER_H
