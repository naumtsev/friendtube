#include "VideoManager.h"

VideoManager::VideoManager(Server *server_) : QObject(), server(server_) {
    current_video.state = Empty;
}




void VideoManager::set_new_video(const QJsonObject &json_video){
    Video video(json_video);
    video.begin_time = QDateTime::currentMSecsSinceEpoch();
    last_time_pause = QDateTime::currentMSecsSinceEpoch();
    video.progress = 0;
    video.state = Playing;
    current_video = video;
    QJsonObject req;
    req.insert("type", "video_event");
    req.insert("event_type", "set_video");
    req.insert("video", video.to_json());
    QJsonDocument doc(req);

    server->send_data_to_all_users(doc.toJson());
}

void VideoManager::pause_video() {
    if(current_video.state == Playing) {
        current_video.progress += QDateTime::currentMSecsSinceEpoch() - last_time_pause;
        last_time_pause = QDateTime::currentMSecsSinceEpoch();
        current_video.state = Pause;

        QJsonObject req;
        req.insert("type", "video_event");
        req.insert("event_type", "pause");
        req.insert("video", current_video.to_json());

        QJsonDocument doc(req);
        server->send_data_to_all_users(doc.toJson());
        return;
    } else if(current_video.state == Pause) {
        last_time_pause = QDateTime::currentMSecsSinceEpoch();
        current_video.state = Playing;
        QJsonObject req;
        req.insert("type", "video_event");
        req.insert("event_type", "pause");
        req.insert("video", current_video.to_json());

        QJsonDocument doc(req);
        server->send_data_to_all_users(doc.toJson());
        return;
    }
}

void VideoManager::stop_video() {
    current_video.state = Empty;

    QJsonObject req;
    req.insert("type", "video_event");
    req.insert("event_type", "stop");

    QJsonDocument doc(req);
    server->send_data_to_all_users(doc.toJson());
}
