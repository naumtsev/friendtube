#include "Client.h"



Client::Client(QString ip_, int port_, QWidget *parent) {
    ip = ip_;
    port = port_;
}

Client::~Client(){
}


void Client::run() {
   socket = new QTcpSocket(this);
   socket->connectToHost(ip.toUtf8(), port);
   socket->waitForConnected(3000);

   connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
   connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
   connect(this, SIGNAL(createRoom(Player*, QVector<PlayerView>)), this->main_window, SLOT(createRoom(Player*, QVector<PlayerView>)));

   //qDebug() << "Try connect" << ip.toUtf8() << " ";


   if(socket->state() == QTcpSocket::ConnectedState) {
       // qDebug() << "SUCCES";
   }
}



void Client::socketDisconnect() {
   socket->close();
}


void Client::socketReady() {
    if(socket->waitForConnected(500)){
        QMutexLocker locker(&socket_mutex);
        QByteArray data = socket->readAll();
       // qDebug() << "Client Thread" << QThread::currentThreadId();



        QJsonParseError json_data_error;
        QJsonObject json_data = QJsonDocument::fromJson(data, &json_data_error).object();
        qDebug() << "_______________\n" << data << "\n______________\n";

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


               socket->write(doc.toJson());
               socket->waitForBytesWritten();
               return;
            } else if(event_type == "connected") {
                QJsonObject scene = json_data.value("scene_data").toObject();
                QJsonArray json_players = scene.value("clients").toArray();

                QVector<PlayerView> players_;
                for(auto json_player: json_players) {
                    players_.push_back(PlayerView(std::move(Player(json_player.toObject()))));
                }
               // qDebug() << QThread::currentThreadId() << "CREATE ROOM CLIENT";
                qRegisterMetaType<QVector<PlayerView> >("QVector<PlayerView>");
                emit createRoom(main_window->player, std::move(players_));
                //    Room(Player *player_, QVector<Player> players_, QWidget *parent);


            } else if(event_type == "updated_successfully"){
                main_window->room->is_updated_data = false;
                // server update our state successfully
            } else if(event_type == "scene_data") {

                  main_window->room->is_got_scene = false;
                 // get scene
                 // qDebug() << "ok";
                  QJsonObject scene = json_data.value("data").toObject();
                  QJsonArray json_players = scene.value("clients").toArray();
                  QVector<PlayerView> players_;
                  for(auto json_player: json_players) {
                      players_.push_back(PlayerView(std::move(Player(json_player.toObject()))));
                  }
                  //qDebug() << "UPDATE PLAYER_STATES";
                  main_window->room->players = std::move(players_);
            }
        }
    }
}



void Client::update_state_on_the_server(QJsonDocument state){
    QMutexLocker locker(&socket_mutex);
    qDebug() << "UPDATE BEGIN";

    //qDebug() << "Update State";
    QJsonObject req;
    req.insert("type", "update_my_state");
    req.insert("client_id", client_id);
    req.insert("person_data", state.object());
    QJsonDocument doc(req);
    socket->write(doc.toJson(), doc.toJson().size());
    qDebug() << "UPDATE ENDED1";
    socket->flush();

    qDebug() << "UPDATE ENDED2";
}


void Client::request_get_scene_on_the_server(){
    QMutexLocker locker(&socket_mutex);
    qDebug() << "GET BEGIN";

    //qDebug() << "Request Scene";
    QJsonObject req;
    req.insert("type", "get_scene");
    req.insert("client_id", client_id);
    QJsonDocument doc(req);
    qDebug() << "GET1 ENED";

    socket->write(doc.toJson(), doc.toJson().size());
    socket->flush();
}
