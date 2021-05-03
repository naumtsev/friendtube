#include "Video.h"


Video::Video(const QUrl &stream_url_, qint64 duration_, const QString &name_, const QString &sender_name_) : stream_url(stream_url_), duration(duration_), name(name_), sender_name(sender_name_) {
}


Video::Video(const QJsonObject &json) {
    stream_url = QUrl(json["stream_url"].toString());
    duration = json["duration"].toInt();
    name = json["name"].toString();
    sender_name = json["sender_name"].toString();
}

Video::~Video() {

}

QJsonDocument Video::to_json() {
    QJsonObject json_video;
    json_video.insert("stream_url", QJsonValue::fromVariant(stream_url));
    json_video.insert("sender_name", QJsonValue::fromVariant(sender_name));

    QJsonDocument doc(json_video);
    return doc;
}

