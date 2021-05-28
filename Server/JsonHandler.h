#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonObject>
#include <QJsonParseError>
#include <QString>

namespace json_handler {

QJsonDocument generate_error(QString error);

QJsonDocument json_object_to_json_document(QJsonObject obj);

}  // namespace json_handler

#endif  // JSONHANDLER_H
