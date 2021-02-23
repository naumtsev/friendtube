#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QApplication>
#include <QtGui>

struct appSettings               // TODO: посмотреть Singeltone!!!!!!!!!!
{
    appSettings (const appSettings &) = delete;
    appSettings &operator=(const appSettings&) = delete;

    static appSettings &settings();

    //void load();  //
    //void save();  // сохранение данных (TODO: обсудить с Антоном)

    unsigned screenWidth()  const;
    unsigned screenHights() const;
private:
    appSettings();
    ~appSettings();

private:
    unsigned mScreenWidth {800};
    unsigned mScreenHights{600};
};

#endif // APPSETTINGS_H
