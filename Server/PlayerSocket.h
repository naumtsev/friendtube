#ifndef PLAYERSOCKET_H
#define PLAYERSOCKET_H

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

class PlayerSocket : QObject {
    Q_OBJECT
public:
    explicit PlayerSocket(qintptr ID, QString client_id, Server *server_, QObject *parent = 0);
    ~PlayerSocket();
    void sendData(QString data);
    QString get_id();
    QJsonObject get_person_data();
public slots:
    void read_data(const QByteArray &data);
    void disconnected();
private:
    QString client_id;
    Server* server;
    qintptr socketDescriptor;
    QWebSocket *socket;
    QJsonObject person_data;
};


#endif // PLAYERSOCKET_H
