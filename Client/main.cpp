#include "Client.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug() << "main" << QThread::currentThreadId();
    Client client;
    client.start();
    return a.exec();
}
