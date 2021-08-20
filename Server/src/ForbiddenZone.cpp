#include "ForbiddenZone.h"

ForbiddenZone::ForbiddenZone(int x_mini, int y_mini, int x_maxi, int y_maxi) :
    x_min(x_mini), y_min(y_mini), x_max(x_maxi), y_max(y_maxi){};

bool ForbiddenZone::is_crossed(QJsonObject &player_data) {
    return !(player_data["x"].toDouble() > x_max || player_data["x"].toDouble() + player_rect_size_x < x_min || player_data["y"].toDouble() > y_max || player_data["y"].toDouble() + player_rect_size_y < y_min);
}

bool ForbiddenZone::is_in(QJsonObject &player_data) {
    return (player_data["x"].toDouble() > x_min && player_data["x"].toDouble() + player_rect_size_x < x_max && player_data["y"].toDouble() > y_min && player_data["y"].toDouble() + player_rect_size_y < y_max);
}
