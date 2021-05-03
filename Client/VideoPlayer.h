#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QMediaPlayer>

enum VideoPlayerState {
    Pause,
    Playing,
    QueueIsEmpty,
    Loading
};

// TODO
class VideoPlayer : QObject {
public:
    VideoPlayer(QObject *parent = nullptr);

    QVideoWidget *output;
    QMediaPlayer *m_player;
    VideoPlayerState state;
};

#endif // VIDEOPLAYER_H
