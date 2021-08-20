#include "Server.h"

namespace {
QString generate_client_id(qint16 size) {
  QString client_id;
  for (int i = 0; i < size; i++) {
    client_id += (char)(QRandomGenerator::global()->bounded('9', 'Z'));
  }
  return client_id;
}
}  // namespace

Server::Server(quint16 port_, QObject *parent)
    : QObject(parent),
      web_socket_server(
          new QWebSocketServer(QStringLiteral("Server friendTube"),
                               QWebSocketServer::NonSecureMode, this)),
      port(port_),
      video_m(new VideoManager(this)), chat_m(new ChatManager(this)), m_check(new MovementCheck(this)) {}

void Server::start_server() {
  if (web_socket_server->listen(QHostAddress::Any, port)) {
    qDebug() << "Listening port " << port << "...";
    qDebug() << "My ip " << web_socket_server->serverAddress().toString();

    connect(web_socket_server, &QWebSocketServer::newConnection, this,
            &Server::incoming_connection);
  } else {
    qDebug() << "Could not start server";
  }
}

void Server::incoming_connection() {
  qDebug() << "New connection!";
  PlayerSocket *p_socket =
      new PlayerSocket(web_socket_server->nativeDescriptor(),
                       generate_client_id(client_id_size), this);
  sockets.push_back(p_socket);
}

Server::~Server() {
  delete web_socket_server;
  delete video_m;
}

void Server::send_data_to_all_users(QByteArray data) {  //
  for (auto &socket : sockets) {
    socket->sendData(data);
  }
}

void Server::socket_disconnected(PlayerSocket *socket) {
  qDebug() << "Disconnected " << socket->get_id() << "," << sockets.size() - 1
           << " clients on connection";

  for (int i = 0; i < sockets.size(); i++) {
    if (socket == sockets[i]) {
      delete sockets[i];
      sockets.removeAt(i);
      break;
    }
  }
}

QJsonObject Server::get_scene_data() {
  QJsonArray clients_data;

  for (auto &socket : sockets) {
    QJsonObject j_obj = socket->get_person_data();
    m_check->check_player_position(j_obj);
    clients_data.push_back(j_obj);
  }

  QJsonObject res;
  res.insert("clients", clients_data);
  res.insert("video", video_m->get_current_video().to_json());
  return res;
}
