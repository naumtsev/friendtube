#include "Message.h"

QJsonObject Message::from_message_to_json(){
    QJsonObject json_message;
    json_message.insert("send_message", QJsonValue::fromVariant(send_message));
    json_message.insert("type", QJsonValue::fromVariant(type));
    return json_message;
}

Message from_json_to_message(QJsonObject json_message){
    Message message;
    message.send_message = json_message["send_message"].toString();
    message.type = json_message["type"].toString();
    return message;
}
