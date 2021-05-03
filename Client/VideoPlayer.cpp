#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
    m_player = new QMediaPlayer(this);
    m_videowidget = new QVideoWidget();
    m_player->setVideoOutput(m_videowidget);
    m_videowidget->setGeometry(100, 100, 1000, 800);
    state = QueueIsEmpty;
}

VideoPlayer::~VideoPlayer(){}

void VideoPlayer::changeState(VideoPlayerState new_state) {
    this->state = new_state;
}

VideoPlayerState VideoPlayer::getState() {
    return this->state;
}

void VideoPlayer::play(){
    this->m_player->play();
    changeState(Playing);
}

void VideoPlayer::show(){
    this->m_videowidget->show();
}

void VideoPlayer::setVideo(const QUrl &url){
    this->m_player->setMedia(url);
    changeState(Loading);
}
