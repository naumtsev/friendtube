#ifndef HSL_H
#define HSL_H

#include <QTime>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QTextStream>

class HSL
{
public:
    HSL();
    int h = 100;
    int s = 80;
    int l = 150;
    QJsonObject from_hsl_to_json();
};

HSL from_json_to_hsl(QJsonObject);

#endif // HSL_H
