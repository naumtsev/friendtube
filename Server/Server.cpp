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


Server::Server(quint16 port_, QObject *parent) : QObject(parent),
    web_socket_server(nullptr) {
    port = port_;
    web_socket_server = new QWebSocketServer(QStringLiteral("Server friendTube"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
}

void Server::startServer() {
    if (web_socket_server->listen(QHostAddress::Any, port)) {
        qDebug() << "Listening to port " << port << "...";
        qDebug() << "My ip " << web_socket_server->serverAddress().toString() << "... THREAD: " << QThread::currentThreadId();

        connect(web_socket_server, &QWebSocketServer::newConnection,
                this, &Server::incomingConnection);
    } else {
        qDebug() << "Could not start server";
    }
}

void Server::incomingConnection(){
    qDebug() << "New connection Thread:" << QThread::currentThreadId();
    QMutexLocker locker(&data_mutex);
    SocketThread *s_thread = new SocketThread(web_socket_server->nativeDescriptor(), generate_client_id(client_id_size), this);
    s_thread->socket = web_socket_server->nextPendingConnection();

    sockets.push_back(s_thread);
    connect(s_thread, SIGNAL(finished()), s_thread, SLOT(deleteLater()));
    s_thread->start();
}


Server::~Server(){
    for(auto socket: sockets) {
            socket->~SocketThread();
    }

}


void Server::send_data_to_all_users(QString data){ //
    for(auto socket: sockets) {
        socket->sendData(data.toUtf8());
    }
}

void Server::socket_disconnected(SocketThread* socket){
    QMutexLocker locker(&data_mutex);
    qDebug() << "Disconnected " << socket->client_id << "," << sockets.size() - 1 << " clients on connections" << sockets.size() - 1;
    for(int i = 0; i < sockets.size(); i++){
        if(socket == sockets[i]) {
            sockets.erase(sockets.begin() + i);
            break;
        }
    }
}


QJsonObject Server::get_scene_data(){
    QMutexLocker locker(&data_mutex);
    QJsonArray clients_data;
    for(SocketThread *socket: sockets) {
        clients_data.push_back(socket->person_data);
    }

    QJsonObject res;
    res.insert("clients", clients_data);

    return res;
}
