#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QJsonObject>
#include <QJsonParseError>

enum VideoState {
    NoVideo,

};

struct Video : public QObject {
    Q_OBJECT
public:
    Video(const QUrl &stream_url_, qint64 duration_, const QString &name_, const QString &sender_name_);
    Video(const QJsonObject &json);
    QJsonDocument to_json();
    void set_begin_time(qint64 b_time);
    QUrl& getStreamUrl();
    qint64 getDuration();
    QString& getName();

private:
    QUrl stream_url;
    qint64 duration;
    qint64 begin_time;
    QString name;
    QString sender_name;
};

#endif // VIDEO_H
