#ifndef FORBIDDENZONE_H
#define FORBIDDENZONE_H
#include <QVector>
#include <QJsonObject>

struct ForbiddenZone {
    ForbiddenZone() = default;
    ForbiddenZone(int x_mini, int y_mini, int x_maxi, int y_maxi);

    bool is_crossed(QJsonObject &player_data);
    bool is_in(QJsonObject & player_data);
private:
    int x_min = 0;
    int y_min = 0;
    int x_max = 0;
    int y_max = 0;
};

#endif // FORBIDDENZONE_H
