#include "clientwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clientWindow w;
    w.show();
    return a.exec();
}
