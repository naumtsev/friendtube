#ifndef VIDEO_H
#define VIDEO_H


#include <QObject>
#include <QUrl>
#include <QString>
#include <QJsonObject>
#include <QJsonParseError>

struct Video : public QObject {
    Q_OBJECT
public:
    Video(const QUrl &stream_url_, qint64 duration_, const QString &name_, const QString &sender_name_);
    Video(const QJsonObject &json);
    ~Video();
    QJsonDocument to_json();

    QUrl stream_url;
    qint64 duration;
    qint64 begin_time;
    QString name;
    QString sender_name;
};

#endif // VIDEO_H
