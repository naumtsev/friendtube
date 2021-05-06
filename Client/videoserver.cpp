#include "VideoServer.h"

VideoServer::VideoServer()
{
    m_playlist = new QList<Video>;
}

VideoServer::~VideoServer(){}

Video& VideoServer::getCurVideo(){
    if (!m_playlist->isEmpty()) {
        return m_playlist->first();
    }
    else {
        return new ;
    }
}

void VideoServer::addNewVideo(Video &video){
    m_playlist->append(video);
}
