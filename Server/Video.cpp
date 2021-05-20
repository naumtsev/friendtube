#include "Video.h"

Video::Video(const QUrl &stream_url_, qint64 duration_, qint64 begin_time_, qint64 progress_,
             const QString &name_, const QString &sender_name_, int state_) :
    QObject(),
    stream_url(stream_url_), duration(duration_), begin_time(begin_time_),
    progress(progress_), name(name_), sender_name(sender_name_), state(static_cast<VideoState>(state_)){
}


Video::Video(){
    state = Empty;
}

Video::Video(const QJsonObject &json) {
    stream_url = QUrl(json["stream_url"].toString());
    duration = json["duration"].toInt();
    begin_time = json["begin_time"].toInt();
    progress = json["progress"].toInt();
    name = json["name"].toString();
    sender_name = json["sender_name"].toString();
    state = static_cast<VideoState>(json["state"].toInt());
}


Video::Video(const Video &other) {
    stream_url = other.stream_url;
    duration = other.duration;
    begin_time = other.begin_time;
    progress = other.progress;
    name = other.name;
    sender_name = other.sender_name;
    state = other.state;
}


Video &Video::operator=(const Video &other) {
    stream_url = other.stream_url;
    duration = other.duration;
    begin_time = other.begin_time;
    progress = other.progress;
    name = other.name;
    sender_name = other.sender_name;
    state = other.state;
    return *this;
}


Video::~Video() {

}

QJsonObject Video::to_json() {
    QJsonObject json_video;
    json_video.insert("stream_url", QJsonValue::fromVariant(stream_url));
    json_video.insert("duration", QJsonValue::fromVariant(duration));
    json_video.insert("begin_time", QJsonValue::fromVariant(begin_time));
    json_video.insert("progress", QJsonValue::fromVariant(progress));
    json_video.insert("name", QJsonValue::fromVariant(name));
    json_video.insert("sender_name", QJsonValue::fromVariant(sender_name));
    json_video.insert("state", QJsonValue::fromVariant(static_cast<int>(state)));
    return json_video;
}
