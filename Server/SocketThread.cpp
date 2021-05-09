#include "SocketThread.h"

SocketThread::SocketThread(qintptr ID, QString client_id_, Server *server_, QObject *parent) :
    QThread(parent) {
    server = server_;
    socketDescriptor = ID;
    client_id = client_id_;
}

void SocketThread::run() {
    qDebug() << "RUN THREAD " << QThread::currentThreadId();
    connect(socket, SIGNAL(binaryMessageReceived(const QByteArray &)), this, SLOT(read_data(const QByteArray&)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << client_id + " client connected";


    QJsonObject jsonResponse;
    jsonResponse.insert("type", "first_connection");
    jsonResponse.insert("client_id", client_id);
    QJsonDocument doc(jsonResponse);
    sendData(doc.toJson());
    exec();
}



void SocketThread::read_data(const QByteArray &data) {
        qDebug() << "read_data Thread:" << QThread::currentThreadId();

        QJsonParseError json_data_error;
        QJsonDocument json_data = QJsonDocument::fromJson(data, &json_data_error);

        if(json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString event_type = json_data.object().value("type").toString();

            if(!event_type.size()) {
                sendData(json_handler::generate_error("missing request type").toJson());
                return;
            }


            if(event_type == "connect") {
                QJsonObject json = json_data.object();
                person_data = json.value("person_data").toObject();

                QJsonObject jsonResponse;
                jsonResponse.insert("type", "connected");
                jsonResponse.insert("scene_data", server->get_scene_data());

                QJsonDocument doc(jsonResponse);
                sendData(doc.toJson());
                return;

            } else if (event_type == "update_my_state") {
                server->data_mutex.lock();
                QJsonObject json = json_data.object();
                server->data_mutex.unlock();
                person_data = json.value("person_data").toObject();

                QJsonObject jsonResponse;
                jsonResponse.insert("type", "updated_successfully");
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);

                sendData(doc.toJson());
                return;
            } else if(event_type == "return_to_menu") {
                disconnected();
                return;
            } else if(event_type == "get_scene") {
                QJsonObject jsonResponse;

                jsonResponse.insert("type", "scene_data");
                jsonResponse.insert("data", server->get_scene_data());
                jsonResponse.insert("client_id", client_id);

                QJsonDocument doc(jsonResponse);
                sendData(doc.toJson());
                return;

            }

        } else { // invalid json
           sendData(json_handler::generate_error("Invalid json").toJson());
        }
}


void SocketThread::disconnected() {
    socket->close();
    server->socket_disconnected(this);
}


void SocketThread::sendData(QString data) {
    socket->sendBinaryMessage(data.toUtf8());
    socket->flush();
}

