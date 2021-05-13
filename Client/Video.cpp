#include "Video.h"


Video::Video(const QUrl &stream_url_, qint64 duration_, const QString &name_, const QString &sender_name_) : stream_url(stream_url_), duration(duration_), name(name_), sender_name(sender_name_) {
}


Video::Video(const QJsonObject &json) {
    stream_url = QUrl(json["stream_url"].toString());
    duration = json["duration"].toInt();
    name = json["name"].toString();
    sender_name = json["sender_name"].toString();
    begin_time = json["begin_time"].toInt();
}


void Video::set_begin_time(qint64 b_time) {
    this->begin_time = b_time;
}


Video::~Video() {
}

QJsonDocument Video::to_json() {
    QJsonObject json_video;
    json_video.insert("stream_url", QJsonValue::fromVariant(stream_url));
    json_video.insert("duration", QJsonValue::fromVariant(duration));
    json_video.insert("name", QJsonValue::fromVariant(name));
    json_video.insert("sender_name", QJsonValue::fromVariant(sender_name));
    json_video.insert("begin_time", QJsonValue::fromVariant(begin_time));

    QJsonDocument doc(json_video);
    return doc;
}

QUrl Video::getStreamUrl() {
    return this->stream_url;
}

qint64 Video::getDuration() {
    return this->duration;
}

QString Video::getName() {
    return this->name;
}
