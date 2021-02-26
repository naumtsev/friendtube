#include "SocketThread.h"

SocketThread::SocketThread(qintptr ID, QString client_id_, Server *server_, QObject *parent) :
    QThread(parent) {
    server = server_;
    socketDescriptor = ID;
    client_id = client_id_;
}

void SocketThread::run() {

    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor)) {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << client_id + " client connected";

    socket->write(client_id.toUtf8());
    exec();
}



void SocketThread::readyRead() {
    if(socket->waitForConnected(500)) {
        socket->waitForReadyRead(300);
        QByteArray data = socket->readAll();
        QJsonParseError json_data_error;
        QJsonDocument json_data = QJsonDocument::fromJson(data, &json_data_error);
        qDebug() << "____________________________________________________________\n" << data;

        if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString event_type = json_data.object().value("type").toString();

            if(!event_type.size()) {
                socket->write(json_handler::generate_error("missing request type").toJson());
                return;
            }


            if(event_type == "connect") {
                QJsonObject json = json_data.object();
                person_data = json.value("person_data").toObject();

                QJsonObject jsonResponse;
                jsonResponse.insert("type", "connected");
                jsonResponse.insert("scene_data", server->get_scene_data());

                QJsonDocument doc(jsonResponse);
                socket->write(doc.toJson());

                return;

            } else if (event_type == "update_my_state") {
                QJsonObject json = json_data.object();
                person_data = json.value("person_data").toObject();

                QJsonObject jsonResponse;
                jsonResponse.insert("type", "updated_successfully");
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);

                socket->write(doc.toJson());
                return;
            } else if(event_type == "get_scene") {
                QJsonObject jsonResponse;

                jsonResponse.insert("type", "scene_data");
                jsonResponse.insert("data", server->get_scene_data());
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);

                socket->write(doc.toJson());
                return;

            }
            socket->flush();

        } else { // invalid json
           socket->write(json_handler::generate_error("Invalid json").toJson());

        }

   }


}


void SocketThread::disconnected() {
    socket->close();
    server->socket_disconnected(this);
}


void SocketThread::sendData(QString data) {
    socket->write(data.toUtf8());
}

