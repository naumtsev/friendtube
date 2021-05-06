#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QWidget>
#include "Video.h"
#include "videoserver.h"

enum VideoPlayerState {
    Paused,
    Playing,
    QueueIsEmpty,
    Loading
};

// TODO
class VideoPlayer : QObject {
public:
    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();
    void show();
    void play();
    void setVideo(const QUrl &url);
    VideoPlayerState getState();
    void getVideo();
    void changeState(VideoPlayerState);

private:
    QMediaPlayer *m_player;
    QVideoWidget *m_videowidget;
    VideoPlayerState state;
};

#endif // VIDEOPLAYER_H
