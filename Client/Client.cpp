#include "Client.h"



Client::Client(QString ip_, int port_, QWidget *parent) {
    ip = ip_;
    port = port_;
}

Client::~Client(){
}


void Client::run() {   
    socket = new QWebSocket();
    connect(socket, &QWebSocket::connected, this, &Client::onConnected);
    connect(socket, &QWebSocket::disconnected, this, &Client::socketDisconnect);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &Client::socketReady);
    connect(this, SIGNAL(createRoom(Player*, QVector<PlayerView *>)), this->main_window, SLOT(createRoom(Player*, QVector<PlayerView *>)));
    //connect(this, &Client::createRoom, this->main_window, &MainWindow::createRoom);

   //socket->open(QUrl(ip + ":" + QString::number(port)));
   socket->open(QUrl("ws://localhost:" + QString::number(port)));

   //qDebug() << "Try connect" << ip.toUtf8() << " ";
}


void Client::onConnected() {
    qDebug() << "Connection is successful";
}


void Client::socketDisconnect() {
   socket->close();
}


void Client::socketReady(const QByteArray &data) {
    QMutexLocker locker(&socket_mutex);
    qDebug() << "MUTEX LOCK" << QThread::currentThreadId();
       // qDebug() << "Client Thread" << QThread::currentThreadId();



        QJsonParseError json_data_error;
        QJsonObject json_data = QJsonDocument::fromJson(data, &json_data_error).object();
        qDebug() << "_______________\n" << json_data << "\n______________\n";

        if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString event_type = json_data.value("type").toString();
            qDebug() << event_type;

            if(event_type == "first_connection") {
               client_id = json_data.value("client_id").toString();
               main_window->player->client_id = client_id;
              // qDebug() << "\nID:" << client_id << "\n";

               QJsonObject req;
               req.insert("type", "connect");
               req.insert("person_data", main_window->player->to_json().object());
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
                // qDebug() << QThread::currentThreadId() << "CREATE ROOM CLIENT";
                emit createRoom(main_window->player, std::move(players_));
                //    Room(Player *player_, QVector<Player> players_, QWidget *parent);

                 qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
                 return;
            } else if(event_type == "updated_successfully"){
                 main_window->room->is_updated_data = false;
                 qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
                 return;
                // server update our state successfully
            } else if(event_type == "scene_data") {
                  main_window->room->is_got_scene = false;
                  QJsonObject scene = json_data.value("data").toObject();
                  QJsonArray json_players = scene.value("clients").toArray();
                  QVector<PlayerView *> players_;
                  for(auto QjsonArray: json_players) {
                      players_.push_back(new PlayerView(Player(QjsonArray.toObject())));
                  }
                  QMutexLocker player_locker{&main_window->room->player_mutex};
                  main_window->room->players = std::move(players_);
                  qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
                return;
            }
        }
    qDebug() << "MUTEX UNLOCK" << QThread::currentThreadId();
}



void Client::update_state_on_the_server(QJsonDocument state){
    QMutexLocker locker(&socket_mutex);
    qDebug() << "UPDATE BEGIN" << QThread::currentThreadId();
    QJsonObject req;
    req.insert("type", "update_my_state");
    req.insert("client_id", client_id);
    req.insert("person_data", state.object());
    QJsonDocument doc(req);
    sendData(doc.toJson());
    qDebug() << "UPDATE ENDED" << QThread::currentThreadId();
}


void Client::sendData(const QByteArray &data) {
    socket->sendBinaryMessage(data);
    socket->flush();
}


void Client::request_get_scene_on_the_server(){
    QMutexLocker locker(&socket_mutex);
    qDebug() << "GET BEGIN" << QThread::currentThreadId();
    QJsonObject req;
    req.insert("type", "get_scene");
    req.insert("client_id", client_id);
    QJsonDocument doc(req);
    sendData(doc.toJson());
    qDebug() << "GET ENDED" << QThread::currentThreadId();
}


