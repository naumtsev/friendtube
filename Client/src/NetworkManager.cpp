#include "NetworkManager.h"

NetworkManager::NetworkManager(Client *client_, const QString &ip_, int port_)
    : QObject() {
    client = client_;
    socket_mutex = new QMutex();
    ip = ip_;
    port = port_;
}

NetworkManager::~NetworkManager() {
    socket->close();
    socket->deleteLater();
    delete socket_mutex;
}

void NetworkManager::run() {
    socket = new QWebSocket();

    connect(socket, &QWebSocket::binaryMessageReceived, this,
            &NetworkManager::socketReady);
    connect(this, SIGNAL(createRoom(Player *, QVector<PlayerView *>, Video)),
            client, SLOT(createRoom(Player *, QVector<PlayerView *>, Video)));

    QString adress = "ws://" + ip + ":" + QString::number(port);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(onWebSocketError(QAbstractSocket::SocketError)));
    socket->open(QUrl(adress));
}

void NetworkManager::finish() {
    QMetaObject::invokeMethod(socket, "close", Qt::QueuedConnection);
}

void NetworkManager::socketReady(const QByteArray &data) {
    QMutexLocker locker(socket_mutex);
    QJsonParseError json_data_error;
    QJsonObject json_data =
            QJsonDocument::fromJson(data, &json_data_error).object();

    if (json_data_error.errorString().toInt() == QJsonParseError::NoError) {
        QString event_type = json_data.value("type").toString();
        if (event_type == "first_connection") {
            client_id = json_data.value("client_id").toString();
            client->menu->player->client_id = client_id;
            QJsonObject req;
            req.insert("type", "connect");
            req.insert("person_data", client->menu->player->to_json().object());
            QJsonDocument doc(req);
            sendData(doc.toJson());
            return;
        } else if (event_type == "connected") {
            QJsonObject scene = json_data.value("scene_data").toObject();
            QJsonArray json_players = scene.value("clients").toArray();
            QJsonObject video = scene.value("video").toObject();

            QVector<PlayerView *> players_;

            for (const auto &json_player : json_players) {
                players_.push_back(new PlayerView{{json_player.toObject()}});
            }

            qRegisterMetaType<Video>("Video");

            qRegisterMetaType<QVector<PlayerView *>>("QVector<PlayerView*>");
            emit createRoom(client->menu->player, players_, video);
            return;
        } else if (event_type == "updated_successfully") {
            client->room->updated_data = false;
            return;
        } else if (event_type == "scene_data") {
            client->room->got_scene = false;
            QJsonObject scene = json_data.value("data").toObject();
            QJsonArray json_players = scene.value("clients").toArray();
            QVector<PlayerView *> players_;

            for (const auto &QjsonArray : json_players) {
                players_.push_back(new PlayerView(Player(QjsonArray.toObject())));
            }

            QMutexLocker player_locker{&client->room->player_mutex};
            client->room->next_frame = std::move(players_);
            return;
        } else if (event_type == "video_event") {
            client->room->video_player->current_video =
                    json_data.value("video").toObject();
            if (json_data.value("event_type") == "set_video") {
                emit video_set_video();
            } else if (json_data.value("event_type") == "pause") {
                emit video_pause();
            } else if (json_data.value("event_type") == "stop") {
                emit video_stop();
            }
        } else if (event_type == "receiving_message") {
            client->room->local_chat->displayMessage(json_data.value("sender_name").toString(), json_data.value("send_message").toString(), json_data.value("color").toString());
        }
    }
}

void NetworkManager::update_state_on_the_server(const QJsonDocument &state) {
    QMutexLocker locker(socket_mutex);

    QJsonObject req;
    req.insert("type", "update_my_state");
    req.insert("client_id", client_id);
    req.insert("person_data", state.object());
    QJsonDocument doc(req);

    sendData(doc.toJson());
}

void NetworkManager::sendData(const QByteArray &data) {
    socket->sendBinaryMessage(data);
    socket->flush();
}

void NetworkManager::request_get_scene_on_the_server() {
    QMutexLocker locker(socket_mutex);

    QJsonObject req;
    req.insert("type", "get_scene");
    req.insert("client_id", client_id);
    QJsonDocument doc(req);

    sendData(doc.toJson());
}

void NetworkManager::onWebSocketError(QAbstractSocket::SocketError error) {
    switch (error) {
        case QAbstractSocket::SocketError::RemoteHostClosedError:
            emit disconnect("Socket error: server closed connection");
            break;
        case QAbstractSocket::SocketError::HostNotFoundError:
            emit disconnect("Socket error: host was not found");
            break;
        case QAbstractSocket::SocketError::SocketAccessError:
            emit disconnect("Socket error: no access to connect");
            break;
        case QAbstractSocket::SocketTimeoutError:
            emit disconnect("Socket error: the socket operation timed out");
            break;
        default:
            emit disconnect("Socket error: unknown error");
            break;
    }
}

void NetworkManager::video_request(QJsonObject req) {
    req.insert("client_id", client_id);
    QJsonDocument doc(req);
    sendData(doc.toJson());
}


void NetworkManager::sendMessageToAllUsers(const QString& sender_name, const QString& message, const QString& color) {
    QMutexLocker locker(socket_mutex);

    QJsonObject req;
    req.insert("type", "sending_message");
    req.insert("sender_name", sender_name);
    req.insert("send_message", message);
    req.insert("color", color);
    QJsonDocument doc(req);
    sendData(doc.toJson());
}
