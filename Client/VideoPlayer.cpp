#include "VideoPlayer.h"



VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {

}



QString yandex_disk_url_to_stream_url(const QString &url) {
    QString scriptFile =  QCoreApplication::applicationDirPath() + "/get_url.py";
    QStringList pythonCommandArguments;
    pythonCommandArguments << scriptFile << "--link" << url;

    QProcess process;
    process.start ("python", pythonCommandArguments);
    process.waitForFinished();
    return process.readAllStandardOutput();
}


