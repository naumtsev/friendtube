QT      += core
QT      += network
QT      += gui
QT      += websockets
QT      += widgets
QT      += xmlpatterns
QT      += multimedia
QT      += multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$PWD/include



HEADERS += \
    include/AnimationView.h \
    include/ChatWindow.h \
    include/Client.h \
    include/HSL.h \
    include/Menu.h \
    include/Message.h \
    include/NetworkManager.h \
    include/Player.h \
    include/PlayerView.h \
    include/Room.h \
    include/ToolManyItem.h \
    include/Video.h \
    include/VideoPlayer.h \
    include/chat.h

VPATH += $$PWD/src

SOURCES += \
    AnimationView.cpp \
    ChatWindow.cpp \
    Client.cpp \
    HSL.cpp \
    Menu.cpp \
    Message.cpp \
    NetworkManager.cpp \
    Player.cpp \
    PlayerView.cpp \
    Room.cpp \
    ToolManyItem.cpp \
    Video.cpp \
    VideoPlayer.cpp \
    main.cpp \
    src/chat.cpp

FORMS += \
    Menu.ui \
    Room.ui \
    include/chat.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    background_item.qrc \
    emoji.qrc \
    images.qrc \
    sprite.qrc

DISTFILES +=

CONFIG += \
    file_copies

COPIES += \
    exe_files

exe_files.files = $$files(get_url.exe)

exe_files.path = $$OUT_PWD


