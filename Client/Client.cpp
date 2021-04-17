#include "Client.h"

Client::Client(QObject *parent) : QObject(parent), menu(nullptr), room(nullptr), n_manager(nullptr){

}


void Client::start() {
    menu = new Menu(this);
    menu->show();
}


void Client::connect_to_server(const QString &ip, int port) {
    n_manager = new NetworkManager(this, ip, port, this);

    n_thread = new QThread();

    connect(n_thread, &QThread::started, n_manager, &NetworkManager::run);

    n_manager->moveToThread(n_thread);

    n_thread->start();
}



void Client::createRoom(Player *player, QVector<PlayerView> players_) {

    qDebug() << QThread::currentThreadId() << "CREATE ROOM MAINWINDOW";
    room = new Room(this, player, players_);
    connect(room, SIGNAL(request_get_scene_on_the_server()), this, SLOT(request_get_scene_on_the_server()));
    connect(room, SIGNAL(update_state_on_the_server(QJsonDocument)), this, SLOT(update_state_on_the_server(QJsonDocument)));


    room->show();
    menu->setVisible(false);
}
