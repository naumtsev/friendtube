#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QVideoWidget *output_, QObject *parent): QObject(parent) {
    output = output_;
    m_player = new QMediaPlayer();
    m_player->setVideoOutput(output);
    m_player->setMedia(QUrl("https://cdn.dribbble.com/users/1373613/screenshots/5627768/___-1.gif"));




   // m_player->setMedia(QUrl("https://streaming.disk.yandex.net/hls/U2FsdGVkX1-11KxQUF7Ycljw0Gl5V6O5Fxm-N9R7gc9g-Vj2TsbRZJOXw40Nf70NrnPPdw5MnmKZOIufTjAWZpaz1OGulvp9f0hD0TdOdorwxZ-nVYjKgYBki5Y_N1pk_OgRiSigoPtkctbTSfdYsEZa1EdMt-3u6yqfflQNK6mrbD7eUI8fuzf7CBFgsfqxMYxx0sn02KHPvjWQxWgi7ujXOAM4tvlh1eexAsWUeabnF6POaA3L1ZOLDEzZ-NnSHAIKqn1zesvrhxY1Oepk4SxyohO0-KVs8RtFbmXGhLA/5c1d38c61b4b8/22444519fc45be48229af173f0e9216202ee89272f90b9ca060140e7608eadde/master-playlist.m3u8"));
    m_player->play();
}

void VideoPlayer::run(){



    qDebug() << "Video Add";
}


VideoPlayer::~VideoPlayer() {
    delete m_player;
}
