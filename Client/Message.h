#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QTime>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QTextStream>
#include <QGraphicsTextItem>

struct Message{
    QString send_message = "";          // сообщение
    QString type = "text";              // тип сообщения
    QTime   sending_time;
    QGraphicsPixmapItem *emoji;
    bool metka_message = false;         // есть ли сообщение
    bool metka_message_painter = false; // чтобы заново таймер не запускать - скажем нет бесконечности
    QJsonObject from_message_to_json();
};


Message from_json_to_message(QJsonObject);

#endif // MESSAGE_H
