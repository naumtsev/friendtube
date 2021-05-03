#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QString>
#include <QJsonObject>
#include <QJsonParseError>

namespace json_handler {

QJsonDocument generate_error(QString error);

QJsonDocument json_object_to_json_document(QJsonObject obj);




}


#endif // JSONHANDLER_H
