#include "VideoPlayer.h"

QString yandex_disk_url_to_stream_url(const QString &url) {
    QStringList pythonCommandArguments;
    pythonCommandArguments << "--link" << url;
    QProcess process;
    process.start ("get_url.exe", pythonCommandArguments);
    process.waitForFinished();
    return process.readAllStandardOutput();
}


VideoPlayer::VideoPlayer(QVideoWidget *output_, QObject *parent): QObject(parent) {
    output = output_;
    m_player = new QMediaPlayer();
    m_player->setVideoOutput(output);
}


void VideoPlayer::run(){
    QString url = yandex_disk_url_to_stream_url("https://disk.yandex.ru/i/maQWX1KvkNJlhQ");
    m_player->setMedia(QUrl(url));
    m_player->play();
    qDebug() << "Video Add";
}


VideoPlayer::~VideoPlayer() {
    m_player->~QMediaPlayer();
}





