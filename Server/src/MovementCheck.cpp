#include "MovementCheck.h"
#include <QFile>

MovementCheck::MovementCheck(Server *server_) : server(server_) {
   init_forbidden_territory();
}

void MovementCheck::init_forbidden_territory() {
    forbidden_territory.push_back({22, 46, 1244, 668});//Внешний
    forbidden_territory.push_back({282, 138, 988, 582});//Внутренний
    forbidden_territory.push_back({0, 106, 158, 170});//верх-лево дом
    forbidden_territory.push_back({0, 150, 96, 228});//верх-лево чел
    forbidden_territory.push_back({1074, 178, 1280, 184});//верх-право дом
    forbidden_territory.push_back({1124, 178, 1154, 238});//верх-право чел
    forbidden_territory.push_back({1108, 608, 1280, 720});//низ-право дом
    forbidden_territory.push_back({1044, 608, 1280, 720});//низ-право чел
    QString json_string;
    QFile file;
    file.setFileName("D:\\Project\\friendtube\\Client\\json_pictures\\all_summer.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    json_string = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json_string.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray trees_array = json["trees"].toArray();
    qDebug() << trees_array.size();
    QJsonArray bushes_array = json["bushes"].toArray();
    QJsonArray threebushes_array = json["threebushes"].toArray();
    QJsonArray stones_array = json["stones"].toArray();

    foreach (const QJsonValue& tree_value, trees_array) {
            QJsonObject tree_obj = tree_value.toObject();
            forbidden_territory.push_back({tree_obj["x"].toInt()+ 40, tree_obj["y"].toInt() + 100, tree_obj["x"].toInt() + 60, tree_obj["y"].toInt() + 115});
    }

    foreach (const QJsonValue& bush_value, bushes_array) {
        if (bush_value.isObject()) {
            QJsonObject bush_obj = bush_value.toObject();
            forbidden_territory.push_back({bush_obj["x"].toInt(), bush_obj["y"].toInt(), bush_obj["x"].toInt() + 15, bush_obj["y"].toInt() + 15});
        }
    }

    foreach (const QJsonValue& threebush_value, threebushes_array) {
        if (threebush_value.isObject()) {
            QJsonObject threebush_obj = threebush_value.toObject();
            forbidden_territory.push_back({threebush_obj["x"].toInt() + 10, threebush_obj["y"].toInt() + 6, threebush_obj["x"].toInt() + 14, threebush_obj["y"].toInt() + 16});
        }
    }

    foreach (const QJsonValue& stone_value, stones_array) {
        if (stone_value.isObject()) {
            QJsonObject stone_obj = stone_value.toObject();
            forbidden_territory.push_back({stone_obj["x"].toInt() + 10, stone_obj["y"].toInt() + 6, stone_obj["x"].toInt() + 14, stone_obj["y"].toInt() + 16});
        }
    }
}

void MovementCheck::check_player_position(QJsonObject &person_data) {
    bool ok = true;
    qreal max_dist = 10;
    qreal dist = (person_data["x"].toDouble() - current_position[person_data["id"].toString()].first) * (person_data["x"].toDouble() - current_position[person_data["id"].toString()].first) +
            (person_data["y"].toDouble() - current_position[person_data["id"].toString()].second) * (person_data["y"].toDouble() - current_position[person_data["id"].toString()].second);
    if (dist > max_dist * max_dist) {
        ok = false;
    }
    for (int i = 0; i < forbidden_territory.size(); ++i) {
        if (i == 0) {
            ok &= forbidden_territory[i].is_in(person_data);
        }
        else {
            ok &= !forbidden_territory[i].is_crossed(person_data);
        }
    }
    qDebug() << ok << "\n";
    if (ok) {
        current_position[person_data["id"].toString()] = {person_data["x"].toDouble(), person_data["y"].toDouble()};
    }
    else {
        person_data["x"] = current_position[person_data["id"].toString()].first;
        person_data["y"] = current_position[person_data["id"].toString()].second;
    }
    qDebug() << person_data["x"].toDouble() << "\n" << person_data["y"].toDouble() << "\n\n\n";
}
