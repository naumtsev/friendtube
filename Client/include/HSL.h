#ifndef HSL_H
#define HSL_H

#include <QColor>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextStream>
#include <QTime>

class HSL {
public:
    HSL();
    HSL(int h_, int s_, int l_) : h(h_), s(s_), l(l_){};

    int h = 100;
    int s = 80;
    int l = 150;
    QJsonObject from_hsl_to_json();
    QColor to_qcolor();
};

HSL from_json_to_hsl(QJsonObject);

#endif// HSL_H
