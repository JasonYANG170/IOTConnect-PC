QT       += core gui
QT += mqtt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    dialog2.cpp \
    form.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialog.h \
    dialog2.h \
    form.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    dialog2.ui \
    form.ui \
    mainwindow.ui

TRANSLATIONS += \
    IOT_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .env
