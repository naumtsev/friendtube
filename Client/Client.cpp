#include "Client.h"



Client::Client(QWidget *parent) {
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
}

Client::~Client(){
}


bool Client::connect_to_server(QString ip, int port) {
   socket->connectToHost(ip.toUtf8(), port);
   socket->waitForConnected(3000);

   if(socket->state() == QTcpSocket::ConnectedState) {

       return true;
   }

   return false;
}



void Client::socketDisconnect() {
    socket->deleteLater();
}


void Client::socketReady() {
    if(socket->waitForConnected(500)){
        socket->waitForReadyRead(500);

        QByteArray data = socket->readAll();


        QJsonParseError json_data_error;
        QJsonObject json_data = QJsonDocument::fromJson(data, &json_data_error).object();
        qDebug() << "_______________\n" << data << "______________\n";

        if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString event_type = json_data.value("type").toString();
            if(event_type == "first_connection") {
               client_id = json_data.value("client_id").toString();
               main_window->player->client_id = client_id;
               qDebug() << "\nID:" << client_id << "\n";

               QJsonObject req;
               req.insert("type", "connect");
               req.insert("person_data", main_window->player->to_json().object());
               QJsonDocument doc(req);
               socket->write(doc.toJson());
               socket->flush();
               return;

            } else if(event_type == "connected") {
                QJsonObject scene = json_data.value("scene_data").toObject();
                QJsonArray json_players = scene.value("clients").toArray();

                QVector<PlayerView> players_;
                for(auto json_player: json_players) {
                    players_.push_back(PlayerView(std::move(Player(json_player.toObject()))));
                }
                //    Room(Player *player_, QVector<Player> players_, QWidget *parent);


                main_window->room = new Room(main_window->player, players_);
                main_window->room->main_window = main_window;
                main_window->room->show();
                main_window->setVisible(false);

                //
            } else if(event_type == "updated_successfully"){
                // server update our state successfully
            } else if(event_type == "scene_data") {
                 // get scene
                  qDebug() << "ok";
                  QJsonObject scene = json_data.value("data").toObject();
                  QJsonArray json_players = scene.value("clients").toArray();
                  QVector<PlayerView> players_;
                  for(auto json_player: json_players) {
                      players_.push_back(PlayerView(std::move(Player(json_player.toObject()))));
                  }
                  main_window->room->players = std::move(players_);

            }

        }
    }
}



void Client::update_state_on_the_server(QJsonDocument state){
     socket->waitForReadyRead(300);
    qDebug() << "Update State";
    QJsonObject req;
    req.insert("type", "update_my_state");
    req.insert("client_id", client_id);
    req.insert("person_data", state.object());
    QJsonDocument doc(req);

    socket->write(doc.toJson());
    socket->flush();

}


void Client::request_get_scene_on_the_server(){
     socket->waitForReadyRead(300);
    qDebug() << "Request Scene";
    QJsonObject req;
    req.insert("type", "get_scene");
    req.insert("client_id", client_id);
    QJsonDocument doc(req);
    socket->write(doc.toJson());
    socket->flush();

}
