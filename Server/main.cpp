#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qint16 port = 6666;

    if(argc > 1) {
        bool ok;
        QString port_(argv[1]);
        qint16 new_port = port_.toInt(&ok, 10);
        if(ok) {
            port = new_port;
        } else {
           qDebug() << "Invalid port";
           return 1;
        }
    }


    Server server(port);
    server.start_server();

    return a.exec();
}
