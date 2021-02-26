#include "Server.h"

namespace {
    QString generate_client_id(qint16 size){
        QString client_id;
        for(int i = 0; i < size; i++) {
            client_id += (char)(QRandomGenerator::global()->bounded('9','Z'));
        }

        return client_id;
    }
}


Server::Server(qint16 port_) {
    port = port_;
}

void Server::startServer() {
    if(!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Could not start server";
    } else {
        qDebug() << "Listening to port " << port << "...";
        qDebug() << "My ip " << serverAddress().toString() << "...";

    }
}

void Server::incomingConnection(qintptr socketDescriptor){

    SocketThread *s_thread = new SocketThread(socketDescriptor, generate_client_id(client_id_size), this);
    sockets.push_back(s_thread);

    connect(s_thread, SIGNAL(finished()), s_thread, SLOT(deleteLater()));
    s_thread->run();

}


Server::~Server(){
    for(auto socket: sockets) {
            socket->~SocketThread();
    }

}


void Server::send_data_to_all_users(QString data){ //
    for(auto socket: sockets) {
        socket->sendData(data);
    }
}

void Server::socket_disconnected(SocketThread* socket){
    qDebug() << "Disconnected " << socket->client_id << "," << sockets.size() - 1 << " clients on connections";
    for(int i = 0; i < sockets.size(); i++){
        if(socket == sockets[i]) {
            sockets.erase(sockets.begin() + i);
            break;
        }
    }
    qDebug() << sockets.size();
}


QJsonObject Server::get_scene_data(){
    QJsonArray clients_data;


    for(SocketThread *socket: sockets) {
        clients_data.append(socket->person_data);
    }

    QJsonObject res;
    res.insert("clients", clients_data);

    return res;
}
