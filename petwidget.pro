QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatdialog.cpp \
    main.cpp \
    petgifwindow.cpp \
    petwidget.cpp

HEADERS += \
    chatdialog.h \
    petgifwindow.h \
    petwidget.h

FORMS += \
    petwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gifs.qrc

DISTFILES += \
    gifs/eat.gif \
    gifs/i-dle.gif \
    gifs/read.gif \
    gifs/revolve.gif \
    gifs/sleep.gif \
    素材/eat.gif \
    素材/i-dle.gif \
    素材/sleep.gif \
    素材/旋转抠图.gif \
    素材/读信抠图.gif
