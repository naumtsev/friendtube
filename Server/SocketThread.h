#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QJsonParseError>
#include "Server.h"
#include "JsonHandler.h"
#include "QtWebSockets/QWebSocket"

class Server;

class SocketThread : public QThread {
    Q_OBJECT
public:
    explicit SocketThread(qintptr ID, QString client_id, Server *server_, QObject *parent = 0);

     void run();
     void sendData(QString data);


public slots:
    void read_data(const QByteArray &data);
    void disconnected();
public:
    QString client_id;
    Server* server;
    QWebSocket *socket;
    qintptr socketDescriptor;
    QJsonObject person_data;
};


#endif // SOCKETTHREAD_H
