#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMainWindow>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpSocket>
#include <QWebSocket>
#include <thread>

#include "Menu.h"
#include "PlayerView.h"
#include "Room.h"
#include "Video.h"

struct Menu;
struct Client;

struct NetworkManager : public QObject {
  Q_OBJECT

 public:
  NetworkManager(Client *client_, const QString &ip_, int port_);
  ~NetworkManager();
  bool connect_to_server(QString ip, int port);

 signals:
  void createRoom(Player *, QVector<PlayerView *>, Video);
  void disconnect(const QString &reason);

  void video_set_video();
  void video_stop();
  void video_pause();

 public slots:
  void run();
  void socketReady(const QByteArray &data);
  void video_request(QJsonObject);
  void request_get_scene_on_the_server();
  void update_state_on_the_server(const QJsonDocument &state);
  void onWebSocketError(QAbstractSocket::SocketError);
  void finish();

 public:
  void sendData(const QByteArray &data);
  QWebSocket *socket;
  QString client_id;
  Client *client;
  QMutex *socket_mutex;
  QString ip;
  int port;
};

#endif  // NETWORKMANAGER_H
