#ifndef SERVER_H
#define SERVER_H

#include "PlayerSocket.h"
#include "JsonHandler.h"
#include "VideoManager.h"
#include <QTcpServer>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QMutex>
#include <QMutexLocker>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"

class PlayerSocket;
class VideoManager;

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);
    ~Server();

    void start_server();
    void send_data_to_all_users(QByteArray data);
    void socket_disconnected(PlayerSocket* socket);

    QJsonObject get_scene_data();
signals:

public slots:
  void incoming_connection();

public:
    QVector<PlayerSocket*> sockets;
    QWebSocketServer *web_socket_server;
    qint16 port;
    const qint16 client_id_size = 10;
    VideoManager *video_m;
};

#endif // SERVER_H
