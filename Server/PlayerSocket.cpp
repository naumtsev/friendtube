#include "PlayerSocket.h"

PlayerSocket::PlayerSocket(qintptr ID, QString client_id_, Server *server_,
                           QObject *parent)
    : QObject(parent),
      client_id(client_id_),
      server(server_),
      socketDescriptor(ID) {
  qDebug() << client_id + " client connected";

  socket = server->web_socket_server->nextPendingConnection();

  connect(socket, SIGNAL(binaryMessageReceived(const QByteArray &)), this,
          SLOT(read_data(const QByteArray &)));
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

  QJsonObject jsonResponse;
  jsonResponse.insert("type", "first_connection");
  jsonResponse.insert("client_id", client_id);
  QJsonDocument doc(jsonResponse);
  sendData(doc.toJson());
}

PlayerSocket::~PlayerSocket() { socket->deleteLater(); }

QString PlayerSocket::get_id() { return client_id; }

QJsonObject PlayerSocket::get_person_data() { return person_data; }

void PlayerSocket::read_data(const QByteArray &data) {
  QJsonParseError json_data_error;
  QJsonDocument json_data = QJsonDocument::fromJson(data, &json_data_error);

  if (json_data_error.errorString().toInt() == QJsonParseError::NoError) {
    QString event_type = json_data.object().value("type").toString();

    if (!event_type.size()) {
      sendData(json_handler::generate_error("missing request type").toJson());
      return;
    }

    if (event_type == "connect") {
      QJsonObject json = json_data.object();
      person_data = json.value("person_data").toObject();

      QJsonObject jsonResponse;
      jsonResponse.insert("type", "connected");
      jsonResponse.insert("scene_data", server->get_scene_data());

      QJsonDocument doc(jsonResponse);
      sendData(doc.toJson());
      return;

    } else if (event_type == "update_my_state") {
      QJsonObject json = json_data.object();
      person_data = json.value("person_data").toObject();

      QJsonObject jsonResponse;
      jsonResponse.insert("type", "updated_successfully");
      jsonResponse.insert("client_id", client_id);

      QJsonDocument doc(jsonResponse);

      sendData(doc.toJson());
      return;
    } else if (event_type == "return_to_menu") {
      socket->close();

      return;
    } else if (event_type == "get_scene") {
      QJsonObject jsonResponse;

      jsonResponse.insert("type", "scene_data");
      jsonResponse.insert("data", server->get_scene_data());
      jsonResponse.insert("client_id", client_id);

      QJsonDocument doc(jsonResponse);
      sendData(doc.toJson());
      return;

    } else if (event_type == "video_event") {
      if (json_data.object().value("event_type") == "stop") {
        server->video_m->stop_video();
      } else if (json_data.object().value("event_type") == "pause") {
        server->video_m->pause_video();
      } else if (json_data.object().value("event_type") == "set_new_video") {
        server->video_m->set_new_video(
            json_data.object().value("video").toObject());
      }
      return;
    }

  } else {
    // get invalid json
    sendData(json_handler::generate_error("Invalid json").toJson());
  }
}

void PlayerSocket::disconnected() { server->socket_disconnected(this); }

void PlayerSocket::sendData(QString data) {
  socket->sendBinaryMessage(data.toUtf8());
  socket->flush();
}
