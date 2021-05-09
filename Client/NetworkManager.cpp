#include "NetworkManager.h"

NetworkManager::NetworkManager(Client *client_, const QString &ip_, int port_) : QObject() {
    client = client_;
    socket_mutex = new QMutex();
    ip = ip_;
    port = port_;
}

NetworkManager::~NetworkManager(){
    qDebug() << "~NetworkManager";
    socket->close();
    socket->~QWebSocket();
    socket_mutex->~QMutex();
}


void NetworkManager::run() {
    socket = new QWebSocket();

   connect(socket, &QWebSocket::connected, this, &NetworkManager::onConnected);
   //connect(socket, &QWebSocket::disconnected, this, &NetworkManager::socketDisconnect);
   connect(socket, &QWebSocket::binaryMessageReceived, this, &NetworkManager::socketReady);
   connect(this, SIGNAL(createRoom(Player*, QVector<PlayerView *>)), client, SLOT(createRoom(Player*, QVector<PlayerView *>)));


    QString adress = "ws://" + ip + ":" + QString::number(port);
    qDebug() << "Try connect to " + adress;
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onWebSocketError(QAbstractSocket::SocketError)));
    socket->open(QUrl(adress));
}


void NetworkManager::onConnected() {
    qDebug() << "Connection is successful";
}


void NetworkManager::socketDisconnect() {

   qDebug() << "Disconnect";
  // socket->close();
  // client->return_to_menu("");
   //emit disconnect("Socket error: server closed connection");
}


void NetworkManager::socketReady(const QByteArray &data) {
    QMutexLocker locker(socket_mutex);
    // qDebug() << "Client Thread" << QThread::currentThreadId();



    QJsonParseError json_data_error;
    QJsonObject json_data = QJsonDocument::fromJson(data, &json_data_error).object();
    qDebug() << "_______________\n" << json_data << "\n______________\n";

    if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
        QString event_type = json_data.value("type").toString();
        //qDebug() << event_type;

        if(event_type == "first_connection") {
           client_id = json_data.value("client_id").toString();
           client->menu->player->client_id = client_id;
           qDebug() << "OK";
           QJsonObject req;
           req.insert("type", "connect");
           req.insert("person_data", client->menu->player->to_json().object());
           QJsonDocument doc(req);

           sendData(doc.toJson());

           qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
           return;
        } else if(event_type == "connected") {
            QJsonObject scene = json_data.value("scene_data").toObject();
            QJsonArray json_players = scene.value("clients").toArray();

            QVector<PlayerView *> players_;
            for(auto json_player: json_players) {
                players_.push_back(new PlayerView{{json_player.toObject()}});
            }
            qRegisterMetaType<QVector<PlayerView*> >("QVector<PlayerView*>");
            emit createRoom(client->menu->player, std::move(players_));

            return;
        } else if(event_type == "updated_successfully"){
             client->room->is_updated_data = false;
             qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
            return;
            // server update our state successfully
        } else if(event_type == "scene_data") {
              client->room->is_got_scene = false;
              QJsonObject scene = json_data.value("data").toObject();
              QJsonArray json_players = scene.value("clients").toArray();
              QVector<PlayerView *> players_;
              for(auto QjsonArray: json_players) {
                  players_.push_back(new PlayerView(Player(QjsonArray.toObject())));
              }
              QMutexLocker player_locker{&client->room->player_mutex}; // То, что Женя подсказал
              client->room->next_frame = std::move(players_);
             qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
            return;
        }
    }
}



void NetworkManager::update_state_on_the_server(const QJsonDocument &state){
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


void NetworkManager::request_get_scene_on_the_server(){
    QMutexLocker locker(socket_mutex);

    QJsonObject req;
    req.insert("type", "get_scene");
    req.insert("client_id", client_id);
    QJsonDocument doc(req);

    sendData(doc.toJson());
}


void NetworkManager::return_to_menu(const QString &reason){
    QMutexLocker locker(socket_mutex);

    QJsonObject req;
    req.insert("type", "return_to_menu");
    req.insert("reason", reason);
    req.insert("client_id", client_id);
    QJsonDocument doc(req);

    sendData(doc.toJson());
}



void NetworkManager::onWebSocketError(QAbstractSocket::SocketError error){
    switch (error) {
    case QAbstractSocket::SocketError::RemoteHostClosedError:
        //emit disconnect("Socket error: server closed connection");
        emit disconnect(""); // фича, надо будет как-нибудь пофиксить
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
        emit disconnect(""); // фича, надо будет как-нибудь пофиксить
        //emit disconnect("Socket error: unknown error");
        break;
    }
}
