#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server(6666);
    server.startServer();
    return a.exec();
}
