#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QVideoWidget *output_, QObject *parent): QObject(parent) {
    output = output_;
    m_player = new QMediaPlayer();
    m_player->setVideoOutput(output);
    run();
}

void VideoPlayer::run(){

    qDebug() << "VIDEO PLAYER THREAD " << QThread::currentThreadId();

    m_player->setMedia(QUrl("https://streaming.disk.yandex.net/hls/U2FsdGVkX18cOOup7_mzckKMH6_pERRSmZ8w3ClpmICifgRf1DbsMtfAeHHOTQAiaiHkFSKheAjED-y0Ha7DkLMqwYfMzT12oTZX6lVUy7Yztqd9Lya-rPoMfMI1b1xKGctkE5sKPLvrhAq864FPJV45u4G9ecdZEc0DTmqsdPTu-Wa5Ce17cpk2i26TQQit3Tu8588FVsEfOibzCn2MIfUYIT-zxAQbX81sIoOo3UMYJVQozQnI1RlgqCT9kyfiqAYadLB-BX12thD9P7D4DN-uyFinN7eJnymPDB75HSzDd2huG3DGB2vuMFIyVOYV/5c182bcad98e0/5aa09d867e20141c036607c615f6cd79efbdcb9e970ffe3b39721446e9e58723/master-playlist.m3u8"));
    m_player->play();
    qDebug() << "Video Add";
}


VideoPlayer::~VideoPlayer() {
    m_player->~QMediaPlayer();
}

