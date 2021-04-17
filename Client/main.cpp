#include "Client.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug() << "main" << QThread::currentThreadId();
    Client client1;
    client1.start();

    Client client2;
    client2.start();
    return a.exec();
}
