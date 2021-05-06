#ifndef VIDEOSERVER_H
#define VIDEOSERVER_H

#include "Video.h"
#include <QList>

class VideoServer
{
public:
    VideoServer();
    ~VideoServer();
    Video& getCurVideo();
    void addNewVideo(Video &video);

private:
    QList<Video> *m_playlist;
};

#endif // VIDEOSERVER_H
