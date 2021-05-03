#include "Client.h"

Client::Client(QObject *parent) : QObject(parent), menu(nullptr), room(nullptr), n_manager(nullptr){

}


void Client::start() {
    menu = new Menu(this);
    menu->show();
}


void Client::connect_to_server(const QString &ip, int port) {
    qDebug() << "Try connect";


    n_manager = new NetworkManager(this, ip, port);
    connect(n_manager, SIGNAL(disconnect(const QString &)), this, SLOT(return_to_menu(const QString &)));

    n_thread = new QThread();

    connect(n_thread, &QThread::started, n_manager, &NetworkManager::run);

    n_manager->moveToThread(n_thread);

    n_thread->start();


}


void Client::createRoom(Player *player, QVector<PlayerView *> players_) {

    qDebug() << QThread::currentThreadId() << "CREATE ROOM";

    room = new Room(this, player, players_);
    connect(room, SIGNAL(request_get_scene_on_the_server()), n_manager, SLOT(request_get_scene_on_the_server()));
    connect(room, SIGNAL(update_state_on_the_server(QJsonDocument)), n_manager, SLOT(update_state_on_the_server(QJsonDocument)));


    room->show();
    menu->setVisible(false);
}


void Client::return_to_menu(const QString &reason) {

    qDebug() << "return to menu. Reason: " << reason;
    if(n_manager != nullptr) {

        n_manager->~NetworkManager();

        n_thread->terminate();
        n_thread->~QThread();
        n_thread = nullptr;
    }

    if(room != nullptr) {
        room->~Room();
        room = nullptr;
    }

    emit menu->make_advert(reason);
    menu->setVisible(true);
}
