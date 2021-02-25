#include "MainWindow.h"
#include "appsettings.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    appSettings::settings();

    CreateMainWidget("Your name", "RED");

    return a.exec();
}
