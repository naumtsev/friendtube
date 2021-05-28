#include "Client.h"

Client::Client(QObject *parent)
    : QObject(parent), menu(nullptr), room(nullptr), n_manager(nullptr) {}

Client::~Client() {
  delete menu;
  delete room;
  delete n_manager;

  n_thread->quit();
  n_thread->wait();
  n_thread->deleteLater();
}

void Client::start() {
  menu = new Menu(this);
  menu->show();
}

void Client::connect_to_server(const QString &ip, int port) {
  n_manager = new NetworkManager(this, ip, port);

  connect(n_manager, SIGNAL(disconnect(const QString &)), this,
          SLOT(return_to_menu(const QString &)));

  n_thread = new QThread();

  connect(n_thread, &QThread::started, n_manager, &NetworkManager::run);

  n_manager->moveToThread(n_thread);

  n_thread->start();
}

void Client::createRoom(Player *player, QVector<PlayerView *> players_,
                        Video video) {
  room = new Room(this, player, players_);
  connect(room, SIGNAL(request_get_scene_on_the_server()), n_manager,
          SLOT(request_get_scene_on_the_server()));
  connect(room, SIGNAL(update_state_on_the_server(QJsonDocument)), n_manager,
          SLOT(update_state_on_the_server(QJsonDocument)));

  menu->setVisible(false);
  room->show();

  room->video_player->current_video = video;
  room->video_player->set_video();
}

void Client::return_to_menu(const QString &reason) {
  if (room != nullptr) {
    delete room;
    room = nullptr;
  }

  if (n_manager != nullptr) {
    n_manager->finish();
    n_thread->quit();

    n_thread = nullptr;
  }

  if (!reason.isEmpty()) {
    emit menu->make_advert(reason);
  }
  menu->setVisible(true);
}
