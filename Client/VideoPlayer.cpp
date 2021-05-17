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
    QString video_data = yandex_disk_url_to_stream_url(url);
    if(video_data == "error"){
        emit make_advert("Вы указали некорректную ссылку на видео");
    } else if(video_data == "adaptive video was not found") {
       emit make_advert("Не было найдено видео в подходящем для вас разрешении");
    } else {
        /*
        QUrl stream_url; +
        qint64 duration; +
        qint64 begin_time;
        QString name; +
        QString sender_name;
        */
        QJsonObject json_data = QJsonDocument::fromJson(video_data.toUtf8()).object();
    //    json_data.insert("sender_name", room->local_player->);
        m_player->stop();
        m_player->setMedia(QUrl(json_data.value("stream_url").toString()));
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





