#-------------------------------------------------
#
# Project created by QtCreator 2015-12-10T20:18:44
#
#-------------------------------------------------

QT       += core gui
#         multimedia
#QT       +=multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spacegame
TEMPLATE = app


SOURCES += main.cpp \
    Bullet.cpp \
    spaceship.cpp \
    Ship.cpp \
    Gamecontrol.cpp \
    Trace.cpp \
    mainwindow.cpp \
    Message.cpp \
    Button.cpp \
    Supportive.cpp \
    State.cpp \
    settingui.cpp \
    File.cpp

HEADERS  += \
    Bullet.h \
    spaceship.h \
    Ship.h \
    Gamecontrol.h \
    Trace.h \
    mainwindow.h \
    Message.h \
    Button.h \
    Supportive.h \
    State.h \
    settingui.h \
    File.h
#   ../build-spacegame-Desktop_Qt_5_5_1_MinGW_32bit-Debug/ui_settingui.h

FORMS    += \
    settingui.ui

RESOURCES += \
    res.qrc
