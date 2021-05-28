#include "HSL.h"

HSL::HSL() {}

QJsonObject HSL::from_hsl_to_json() {
    QJsonObject json_color;
    json_color.insert("h", QJsonValue::fromVariant(h));
    json_color.insert("s", QJsonValue::fromVariant(s));
    json_color.insert("l", QJsonValue::fromVariant(l));
    return json_color;
}

HSL from_json_to_hsl(QJsonObject json_color) {
    HSL color;
    color.h = json_color["h"].toInt();
    color.s = json_color["s"].toInt();
    color.l = json_color["l"].toInt();
    return color;
}

QColor HSL::to_qcolor() {
    QColor color;
    color.setHsl(h, s, l);
    return color;
}
