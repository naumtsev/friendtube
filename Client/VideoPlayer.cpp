#include "VideoPlayer.h"

QJsonObject yandex_disk_url_to_json_video(const QString &url) {
    QStringList pythonCommandArguments;
    pythonCommandArguments << "--link" << url;
    QProcess process;
    process.start ("get_url.exe", pythonCommandArguments);
    process.waitForFinished();
    QByteArray data = process.readAllStandardOutput();
    qDebug() << data;

    QJsonObject json_data = QJsonDocument::fromJson(process.readAllStandardOutput()).object();
    qDebug() << json_data.value("duration");
    return json_data;
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
    QJsonObject video_data = yandex_disk_url_to_json_video(url);
    QString str = QJsonDocument(video_data).toJson(QJsonDocument::Compact);
    qDebug() << str;
    if(str == "error"){
        emit make_advert("Вы указали некорректную ссылку на видео");
    } else if(str == "adaptive video was not found") {
       emit make_advert("Не было найдено видео в подходящем для вас разрешении");
    } else {
        /*
        QUrl stream_url; +
        qint64 duration; +
        qint64 begin_time;
        QString name; +
        QString sender_name;
        */
        //json_data.insert("sender_name", room->local_player->);
        m_player->stop();
        m_player->setMedia(QUrl(video_data.value("stream_url").toString()));
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





