#include <QApplication>

#include "Client.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int count_clients = 1;
    QList<Client *> clients;
    for (int i = 0; i < count_clients; i++) {
        clients.push_back(new Client());
        clients.back()->start();
    }

    return a.exec();
}
