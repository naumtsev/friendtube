#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "Video.h"
#include "Room.h"

#include <QObject>
#include <QMediaPlayer>
#include <QThread>
#include <QCoreApplication>
#include <QProcess>
#include <QQueue>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QEventLoop>



struct Room;

struct VideoPlayer : QObject {
    Q_OBJECT
public:
    VideoPlayer(Room *room, QVideoWidget *output_, QObject *parent = nullptr);
    ~VideoPlayer();
    VideoState get_state();

signals:
    void make_advert(QString message);
    void video_request(QJsonObject);
    void set_focus_room();

public slots:
    void try_pause();
    void pause();
    void try_stop();
    void stop();
    void try_set_video(const QString &url);
    void set_video();
    void change_volume(int);
public:
    QVideoWidget *output;
    QThread *m_thread;
    Video current_video;

    QMediaPlayer *m_player;
    Room *room;
};

QJsonObject yandex_disk_url_to_stream_url(const QString &url);


struct UrlHandler : public QObject {
    QJsonObject get_url(QString url);
    QString req_url = "http://195.133.145.54:1234";
};





#endif // VIDEOPLAYER_H
