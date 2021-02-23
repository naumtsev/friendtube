#include "appsettings.h"

appSettings::appSettings() {
     // TODO: при создании сихронизироваться с сервером.
}

appSettings::~appSettings() {
     // TODO: можно сохранять данные - пока опционально
}

appSettings& appSettings::settings() {
    static appSettings res;
    return res;
}


unsigned appSettings::screenWidth()  const {
    return appSettings::mScreenWidth;
}

unsigned appSettings::screenHights() const {
    return appSettings::mScreenHights;
}
