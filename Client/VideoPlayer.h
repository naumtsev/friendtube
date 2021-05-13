#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H


#include "Video.h"
#include <QObject>
#include <QMediaPlayer>
#include <QThread>

enum VideoPlayerState {
    Pause,
    Playing,
    QueueIsEmpty,
    Loading
};

// TODO
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

#endif // VIDEOPLAYER_H
