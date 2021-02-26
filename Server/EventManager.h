#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include "JsonHandler.h"


struct EventManager : public QObject {
    Q_OBJECT

public:
    EventManager(QObject* parent = nullptr);



};

#endif // EVENTMANAGER_H
