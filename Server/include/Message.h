#ifndef MESSAGE_H
#define MESSAGE_H

#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>
#include <QTextStream>
#include <QTime>

struct Message {
    explicit Message(const QString sender_name_, const QString send_message_, const QString color_);
    explicit Message() = default;
    QString sender_name;
    QString send_message = "";
    QString color;
    QString type = "text";
    QTime sending_time;
    QJsonObject from_message_to_json();
};

Message from_json_to_message(QJsonObject);

#endif// MESSAGE_H
