#include "ChatManager.h"

ChatManager::ChatManager(Server *server_) : QObject(), server(server_) {
}

void ChatManager::sendMessageToAllUsers(const QString &sender_name, const QString &message, const QString &color) {
    QJsonObject req;
    req.insert("type", "receiving_message");
    req.insert("sender_name", sender_name);
    req.insert("send_message", message);
    req.insert("color", color);
    QJsonDocument doc(req);
    qDebug() << "message sent back" << message;
    server->send_data_to_all_users(doc.toJson());
}

void ChatManager::sendAllChat(PlayerSocket* p_socket) {
    for (auto& message : all_chat) {
        QJsonObject jsonResponse;
        jsonResponse.insert("type", "receiving_message");
        jsonResponse.insert("sender_name", message->sender_name);
        jsonResponse.insert("send_message", message->send_message);
        jsonResponse.insert("color", message->color);
        QJsonDocument doc(jsonResponse);
        p_socket->sendData(doc.toJson());
    }
    return;
}
