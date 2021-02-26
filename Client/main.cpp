#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w1;
    w1.show();

    //MainWindow w2;
    //w2.show();

    return a.exec();
}
