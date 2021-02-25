#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QTime>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QTextStream>

struct Message{
    QString send_message = "";          // сообщение
    QString type = "text";              // тип сообщения
    QTime   sending_time;
    bool metka_message = false;         // есть ли сообщение
    bool metka_message_painter = false; // чтобы заново таймер не запускать - скажем нет бесконечности
    QJsonObject from_message_to_json();
    /*Message& operator=(Message& message_other){
        send_message = message_other.send_message;
        type = message_other.type;
        metka_message = message_other.metka_message;
        metka_message_painter = message_other.metka_message_painter;
    }*/
};
Message from_json_to_message(QJsonObject);

#endif // MESSAGE_H
