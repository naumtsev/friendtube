#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "Video.h"
#include <QObject>
#include <QMediaPlayer>
#include <QThread>
#include <QCoreApplication>
#include <QProcess>


enum VideoPlayerState {
    Pause,
    Playing,
    QueueIsEmpty,
    Loading
};


struct VideoPlayer : QObject {
    Q_OBJECT
public:
    VideoPlayer(QVideoWidget *output_, QObject *parent = nullptr);
    ~VideoPlayer();
public slots:
    void run();
public:
    QVideoWidget *output;
    QMediaPlayer *m_player;
    VideoPlayerState state;
    Video *current_video;
};



QString yandex_disk_url_to_stream_url(const QString &url);



#endif // VIDEOPLAYER_H
