#include "VideoPlayer.h"

QJsonObject yandex_disk_url_to_stream_url(const QString &url) {
    QStringList pythonCommandArguments;
    pythonCommandArguments << "--link" << url;
    QProcess process;
    process.start("get_url.exe", pythonCommandArguments);
    process.waitForFinished();
    QByteArray data = process.readAllStandardOutput();
    QJsonObject json_data = QJsonDocument::fromJson(data).object();
    return json_data;
}

VideoPlayer::VideoPlayer(Room *room_, QVideoWidget *output_, QObject *parent)
    : QObject(parent) {
    room = room_;
    output = output_;
    m_thread = new QThread();
    m_player = new QMediaPlayer();
    m_player->moveToThread(m_thread);
    m_thread->start();
    m_player->setVideoOutput(output);
    current_video.state = Empty;
}

void VideoPlayer::try_pause() {
    if (get_state() == Playing || get_state() == Pause) {
        QJsonObject req;
        req.insert("type", "video_event");
        req.insert("event_type", "pause");
        emit video_request(req);
    }
}

void VideoPlayer::pause() {
    if (current_video.state == Pause) {
        room->push_button_pause_video->setIcon(
                QIcon(QPixmap(":/pics/pause_new_video_push_button.png")));
        m_player->pause();
    } else if (current_video.state == Playing) {
        room->push_button_pause_video->setIcon(
                QIcon(QPixmap(":/pics/resume_video_push_button.png")));
        m_player->play();
    }
}

void VideoPlayer::try_set_video(const QString &url) {
    UrlHandler handler;
    QJsonObject part_video = handler.get_url(url);
    if (part_video.value("status") == "ok") {
        Video new_video;
        new_video.sender_id = room->local_player->client_id;
        new_video.stream_url = part_video.value("stream_url").toString();
        new_video.duration = part_video.value("duration").toString().toLongLong();

        QJsonObject req;
        req.insert("type", "video_event");
        req.insert("event_type", "set_new_video");
        req.insert("video", new_video.to_json());
        emit video_request(req);
    } else {
        emit make_advert("Вы указали некорректную ссылку на видео");
    }
}

void VideoPlayer::set_video() {
    m_player->stop();
    m_player->setMedia(QUrl(current_video.stream_url));
    m_player->setPosition(current_video.progress);
    if (current_video.state == Playing) {
        m_player->play();
        room->push_button_pause_video->setIcon(
                QIcon(QPixmap(":/pics/resume_video_push_button.png")));
    } else if (current_video.state == Pause) {
        m_player->pause();
        room->push_button_pause_video->setIcon(
                QIcon(QPixmap(":/pics/pause_new_video_push_button.png")));
    }
}

void VideoPlayer::try_stop() {
    QJsonObject req;
    req.insert("type", "video_event");
    req.insert("event_type", "stop");
    emit video_request(req);
}

void VideoPlayer::stop() {
    current_video = Video();

    m_player->stop();
    room->push_button_pause_video->setIcon(
            QIcon(QPixmap(":/pics/pause_new_video_push_button.png")));
}

void VideoPlayer::change_volume(int volume) {
    m_player->setVolume(volume);
    emit set_focus_room();
}

VideoPlayer::~VideoPlayer() { delete m_player; }

QJsonObject UrlHandler::get_url(QString url) {
    QNetworkRequest request(QUrl(req_url + "/get_url"));
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("url", url);

    QJsonDocument doc(obj);

    QNetworkAccessManager mngr;

    QNetworkReply *reply = mngr.post(request, doc.toJson());

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QJsonDocument json_data = QJsonDocument::fromJson(reply->readAll());

    return json_data.object();
}

VideoState VideoPlayer::get_state() { return current_video.state; }
