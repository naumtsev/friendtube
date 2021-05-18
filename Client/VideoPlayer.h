#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "Video.h"

#include <QObject>
#include <QMediaPlayer>
#include <QThread>
#include <QCoreApplication>
#include <QProcess>
#include <QQueue>

enum VideoPlayerState {
    Pause,
    Playing,
    Empty,
    Loading
};


struct VideoPlayer : QObject {
    Q_OBJECT
public:
    VideoPlayer(QVideoWidget *output_, QObject *parent = nullptr);
    ~VideoPlayer();

signals:
    void make_advert(QString message);

public slots:
    void pause();
    void stop();
    void set_video(QString url);
    void change_volume(int);
public:
    QVideoWidget *output;
    QThread *m_thread;
    QMediaPlayer *m_player;
    VideoPlayerState state;
    Video *current_video;
    QQueue<Video> q_videos;
};



QJsonObject yandex_disk_url_to_json_video(const QString &url);



#endif // VIDEOPLAYER_H
