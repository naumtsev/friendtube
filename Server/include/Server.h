#ifndef SERVER_H
#define SERVER_H

#include <QJsonArray>
#include <QMutex>
#include <QMutexLocker>
#include <QRandomGenerator>
#include <QTcpServer>

#include "JsonHandler.h"
#include "PlayerSocket.h"
#include "QtWebSockets/QWebSocket"
#include "QtWebSockets/QWebSocketServer"
#include "VideoManager.h"
#include "ChatManager.h"
#include "MovementCheck.h"

class PlayerSocket;
class VideoManager;
class ChatManager;
class MovementCheck;

class Server : public QObject {
  Q_OBJECT
 public:
  explicit Server(quint16 port, QObject *parent = nullptr);
  ~Server();

  void start_server();
  void send_data_to_all_users(QByteArray data);
  void socket_disconnected(PlayerSocket *socket);

  QJsonObject get_scene_data();
 signals:

 public slots:
  void incoming_connection();

 public:
  QVector<PlayerSocket *> sockets;
  QWebSocketServer *web_socket_server;
  qint16 port;
  const qint16 client_id_size = 10;
  VideoManager *video_m;
  ChatManager *chat_m;
  MovementCheck *m_check;
};

#endif  // SERVER_H
