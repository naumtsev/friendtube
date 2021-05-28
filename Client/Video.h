#ifndef VIDEO_H
#define VIDEO_H

#include <QJsonObject>
#include <QJsonParseError>
#include <QObject>
#include <QString>
#include <QUrl>

enum VideoState { Pause = 1, Playing = 2, Empty = 3, Loading = 4, Syncing = 5 };

struct Video : public QObject {
  Q_OBJECT
 public:
  Video(const QUrl &stream_url_, qint64 duration_, qint64 begin_time_,
        qint64 progress_, const QString &name_, const QString &sender_name_,
        int state);

  Video &operator=(const Video &other);
  Video(const QJsonObject &json);
  Video();
  Video(const Video &other);

  ~Video();
  QJsonObject to_json();

  QUrl stream_url;
  qint64 duration;
  qint64 begin_time;
  qint64 progress;
  QString name;
  QString sender_name;
  VideoState state;
};

#endif  // VIDEO_H
