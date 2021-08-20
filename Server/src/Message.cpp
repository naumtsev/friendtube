#include "Message.h"

Message::Message(const QString sender_name_, const QString send_message_, const QString color_) :
    sender_name(sender_name_), send_message(send_message_), color(color_) {};

QJsonObject Message::from_message_to_json() {
    QJsonObject json_message;
    json_message.insert("send_message", QJsonValue::fromVariant(send_message));
    json_message.insert("sender_name", QJsonValue::fromVariant(sender_name));
    json_message.insert("color",  QJsonValue::fromVariant(color));
    return json_message;
}

Message from_json_to_message(QJsonObject json_message) {
    Message message;
    message.send_message = json_message["send_message"].toString();
    message.sender_name = json_message["sender_name"].toString();
    message.color = json_message["color"].toString();
    return message;
}
