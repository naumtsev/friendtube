#include "JsonHandler.h"

namespace json_handler {
QJsonDocument generate_error(QString error) {
  QJsonObject jsonResponse;
  jsonResponse["type"] = "error";
  jsonResponse["text"] = error;
  QJsonDocument doc(jsonResponse);
  return doc;
}

QJsonDocument json_object_to_json_document(QJsonObject obj) {
  return QJsonDocument(obj);
}

}  // namespace json_handler
