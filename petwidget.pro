QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): 

QT += widgets
CONFIG += c++17
QT += network
QT += multimedia


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

INCLUDEPATH += $$PWD/weeedheaders/headers
SOURCES += \
    weedsources/cores/unit.cpp \
    weedsources/cores/framework.cpp \
    weedsources/utils/resources.cpp \
    weedsources/views/mainwindow.cpp \
    weedsources/views/dialogs/startdialog.cpp \
    weedsources/views/dialogs/aboutdialog.cpp \
    weedsources/views/dialogs/rankdialog.cpp \
    weedsources/views/dialogs/successdialog.cpp \
    weedsources/views/widgets/scenewidget.cpp \
    weedsources/views/dialogs/customdialog.cpp

HEADERS += \
    weeedheaders/headers/cores/unit.h \
    weeedheaders/headers/cores/framework.h \
    weeedheaders/headers/utils/constants.h \
    weeedheaders/headers/utils/resources.h \
    weeedheaders/headers/views/mainwindow.h \
    weeedheaders/headers/views/dialogs/startdialog.h \
    weeedheaders/headers/views/dialogs/aboutdialog.h \
    weeedheaders/headers/views/dialogs/rankdialog.h \
    weeedheaders/headers/views/dialogs/successdialog.h \
    weeedheaders/headers/views/widgets/scenewidget.h \
    weeedheaders/headers/views/dialogs/customdialog.h   

FORMS += \
    petwidget.ui \
    weedform/mainwindow.ui \
    weedform/dialogs/startdialog.ui \
    weedform/dialogs/aboutdialog.ui \
    weedform/dialogs/rankdialog.ui \
    weedform/dialogs/successdialog.ui \
    weedform/dialogs/customdialog.ui

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

DISTFILES += \
    gifs/weedresources/resources/images/icon.ico \
    gifs/weedresources/resources/images/uncover.png \
    gifs/weedresources/resources/images/cover.png \
    gifs/weedresources/resources/images/error.png \
    gifs/weedresources/resources/images/flag.png \
    gifs/weedresources/resources/images/mine.png \
    gifs/weedresources/resources/images/touch.png \
    gifs/weedresources/resources/images/number_1.png \
    gifs/weedresources/resources/images/number_2.png \
    gifs/weedresources/resources/images/number_3.png \
    gifs/weedresources/resources/images/number_4.png \
    gifs/weedresources/resources/images/number_5.png \
    gifs/weedresources/resources/images/number_6.png \
    gifs/weedresources/resources/images/number_7.png \
    gifs/weedresources/resources/images/number_8.png \
    gifs/weedresources/resources/images/title.png \
    gifs/weedresources/resources/sounds/click.wav \
    gifs/weedresources/resources/sounds/failure.wav \
    gifs/weedresources/resources/sounds/success.wav \
    gifs/weedresources/resources/images/colorback.png \
    gifs/weedresources/resources/images/certify.png \
    gifs/weedresources/resources/images/open.png \
    gifs/weedresources/resources/images/tips.png \
