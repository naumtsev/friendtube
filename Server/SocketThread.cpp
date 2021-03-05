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


    QJsonObject jsonResponse;
    jsonResponse.insert("type", "first_connection");
    jsonResponse.insert("client_id", client_id);
    QJsonDocument doc(jsonResponse);
    socket->write(doc.toJson());
    socket->flush();
    exec();
}



void SocketThread::readyRead() {
    if(socket->waitForConnected(500)) {
        QByteArray data = socket->readAll();
        QJsonParseError json_data_error;
        QJsonDocument json_data = QJsonDocument::fromJson(data, &json_data_error);
        qDebug() << "____________________________________________________________\n" << data << "\n_________________________________";

        if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString event_type = json_data.object().value("type").toString();

            if(!event_type.size()) {
                socket->write(json_handler::generate_error("missing request type").toJson());                socket->flush();
                socket->flush();
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
                socket->flush();

                return;

            } else if (event_type == "update_my_state") {
                server->data_mutex.lock();
                QJsonObject json = json_data.object();
                server->data_mutex.unlock();
                person_data = json.value("person_data").toObject();
                qDebug () << "SMD REQUIRE UPDATE HIS DATE" << person_data;


                QJsonObject jsonResponse;
                jsonResponse.insert("type", "updated_successfully");
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);

                socket->write(doc.toJson());
                socket->flush();

                return;
            } else if(event_type == "get_scene") {
                 qDebug() << "smb get scene";
                QJsonObject jsonResponse;

                jsonResponse.insert("type", "scene_data");
                jsonResponse.insert("data", server->get_scene_data());
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);

                socket->write(doc.toJson());
                socket->flush();

                return;

            }

        } else { // invalid json
           socket->write(json_handler::generate_error("Invalid json").toJson());
           socket->flush();

        }

   }


}


void SocketThread::disconnected() {
    socket->close();
    server->socket_disconnected(this);
}


void SocketThread::sendData(QString data) {
    socket->write(data.toUtf8());
    socket->flush();
}

