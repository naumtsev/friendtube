#ifndef SERVER_H
#define SERVER_H

#include "SocketThread.h"
#include "EventManager.h"
#include "JsonHandler.h"
#include <QTcpServer>
#include <QJsonArray>
#include <QRandomGenerator>

class SocketThread;

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(qint16 port);
    void startServer();
    void send_data_to_all_users(QString data);
    ~Server();
    void socket_disconnected(SocketThread* socket);
    QJsonObject get_scene_data();
signals:

public slots:
  void incomingConnection(qintptr socketDescriptor);

public:
    QList<SocketThread*> sockets;
    qint16 port;
    const qint16 client_id_size = 6;
};

#endif // SERVER_H
