#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include <QDateTime>
#include <QObject>

#include "Server.h"
#include "Video.h"

class Server;

class VideoManager : QObject {
  Q_OBJECT
 public:
  VideoManager(Server *server_);

  void set_new_video(const QJsonObject &json_video);
  void pause_video();
  void stop_video();
  Video get_current_video();

  Server *server;
  Video current_video;
  qint64 last_time_pause;
};

#endif  // VIDEOMANAGER_H
