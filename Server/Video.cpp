#include "Video.h"

Video::Video() { state = Empty; }

Video::Video(const QJsonObject &json) {
  stream_url = QUrl(json["stream_url"].toString());
  duration = json["duration"].toInt();
  begin_time = json["begin_time"].toInt();
  progress = json["progress"].toInt();
  name = json["name"].toString();
  sender_id = json["sender_id"].toString();
  state = static_cast<VideoState>(json["state"].toInt());
}

Video::Video(const QUrl &stream_url_, qint64 duration_, qint64 begin_time_,
             qint64 progress_, const QString &name_, const QString &sender_id_,
             int state_)
    : QObject(),
      stream_url(stream_url_),
      duration(duration_),
      begin_time(begin_time_),
      progress(progress_),
      name(name_),
      sender_id(sender_id_),
      state(static_cast<VideoState>(state_)) {}

Video::Video(const Video &other) {
  stream_url = other.stream_url;
  duration = other.duration;
  begin_time = other.begin_time;
  progress = other.progress;
  name = other.name;
  sender_id = other.sender_id;
  state = other.state;
}

Video &Video::operator=(const Video &other) {
  stream_url = other.stream_url;
  duration = other.duration;
  begin_time = other.begin_time;
  progress = other.progress;
  name = other.name;
  sender_id = other.sender_id;
  state = other.state;
  return *this;
}

QJsonObject Video::to_json() {
  QJsonObject json_video;
  json_video.insert("stream_url", stream_url.toString());
  json_video.insert("duration", duration);
  json_video.insert("begin_time", begin_time);
  json_video.insert("progress", progress);
  json_video.insert("name", name);
  json_video.insert("sender_id", sender_id);
  json_video.insert("state", static_cast<int>(state));
  return json_video;
}

Video::~Video() {}
