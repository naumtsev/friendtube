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
    m_thread = new QThread();
    m_player = new QMediaPlayer();
    m_player->moveToThread(m_thread);
    m_thread->start();
    m_player->setVideoOutput(output);
    state = Empty;
}


void VideoPlayer::pause() {
    if(state == Playing){
        m_player->pause();
        state = Pause;
    } else if(state == Pause){
        m_player->play();
        state = Playing;
    }
}

void VideoPlayer::set_video(QString url){

    //default url "https://disk.yandex.ru/i/maQWX1KvkNJlhQ"
    QString strem_url = yandex_disk_url_to_stream_url(url);
    if(strem_url == "error"){
        emit make_advert("Вы указали некорректную ссылку на видео");
    } else if(strem_url == "incorrect url") {
       emit make_advert("Произошла неизвестная ошибка");
    } else {
        m_player->stop();
        m_player->setMedia(QUrl(strem_url));
        m_player->play();
        state = Playing;
    }
}

void VideoPlayer::stop(){
    if(state == Playing) {
           m_player->stop();
           state = Empty;
    }
}


void VideoPlayer::change_volume(int volume) {
    m_player->setVolume(volume);
}

VideoPlayer::~VideoPlayer() {
        delete m_player;
}





