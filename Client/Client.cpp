#include "Client.h"

Client::Client(QObject *parent) : QObject(parent), menu(nullptr), room(nullptr), n_manager(nullptr){

}


Client::~Client(){
    if(room != nullptr) {
        delete room;
        delete n_manager;
    }
    delete menu;
}

void Client::start() {
    menu = new Menu(this);
    menu->show();
}


void Client::connect_to_server(const QString &ip, int port) {
    qDebug() << "Try connect";

    n_manager = new NetworkManager(this, ip, port);
    n_thread = new QThread();

    connect(n_thread, &QThread::started, n_manager, &NetworkManager::run);
    connect(n_manager, &NetworkManager::close_socket(const QString &), this, &Client::return_to_menu(const QString &));

    n_manager->moveToThread(n_thread);

    n_thread->start();


}


void Client::createRoom(Player *player, QVector<PlayerView *> players_) {

    qDebug() << QThread::currentThreadId() << "CREATE ROOM";

    room = new Room(this, player, players_);
    connect(room, SIGNAL(return_to_menu(const QString &)), n_manager, SLOT(return_to_menu(const QString &)));
    connect(room, SIGNAL(request_get_scene_on_the_server()), n_manager, SLOT(request_get_scene_on_the_server()));
    connect(room, SIGNAL(update_state_on_the_server(QJsonDocument)), n_manager, SLOT(update_state_on_the_server(QJsonDocument)));


    room->show();
    menu->setVisible(false);
}


void Client::return_to_menu(const QString &reason) {

    qDebug() << "return to menu. Reason: " << reason;


    if(room != nullptr) {
        delete room;
        room = nullptr;
    }

    if(n_manager != nullptr) {
        delete n_manager;
        n_manager = nullptr;
        n_thread->quit();
        n_thread->wait();
        delete n_thread;
        n_thread = nullptr;
    }



    if(!reason.isEmpty()){
        emit menu->make_advert(reason);
    }
    menu->setVisible(true);
}
