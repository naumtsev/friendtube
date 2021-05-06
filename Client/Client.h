#ifndef CLIENT_H
#define CLIENT_H

#include "Menu.h"
#include "Room.h"
#include "NetworkManager.h"

#include <QObject>


struct Menu;
struct Room;
struct NetworkManager;
namespace Ui { struct Menu; }

struct Client : QObject {
    Q_OBJECT
public:
    Client(QObject *parent = nullptr);
    void start();
    void connect_to_server(const QString &ip, int port);
    friend Menu;
    friend Room;
    friend NetworkManager;

signals:

public slots:
    void createRoom(Player *player, QVector<PlayerView *> players_);
    void return_to_menu(const QString &reason);

private:
    Menu *menu;
    Room *room;
    NetworkManager *n_manager;
    QThread *n_thread;
};



#endif // CLIENT_H
