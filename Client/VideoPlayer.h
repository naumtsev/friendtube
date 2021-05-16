#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QProcess>
#include <QCoreApplication>


enum VideoPlayerState {
    Pause,
    Playing,
    QueueIsEmpty,
    Loading
};


QString yandex_disk_url_to_stream_url(const QString &url);


class VideoPlayer : QObject {
public:
    VideoPlayer(QObject *parent = nullptr);

    QVideoWidget *output;
    QMediaPlayer *m_player;
    VideoPlayerState state;
};

#endif // VIDEOPLAYER_H
