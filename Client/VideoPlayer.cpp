#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
<<<<<<< HEAD
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
=======
>>>>>>> ac66bba91203221e4730ea025560bdd78fe58093

void VideoPlayer::setVideo(const QUrl &url){
    this->m_player->setMedia(url);
    changeState(Loading);
}

