#-------------------------------------------------
#
# Project created by QtCreator 2018-11-13T15:56:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dnd_calendar
TEMPLATE = app

CONFIG += c++11
RC_FILE = dnd_calendar.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    calendar.cpp \
    restdialog.cpp \
    setdate.cpp \
    utils.cpp \
    savemanager.cpp

HEADERS  += mainwindow.h \
    calendar.h \
    restdialog.h \
    setdate.h \
    utils.h \
    savemanager.h

FORMS    += mainwindow.ui \
    restdialog.ui \
    setdate.ui \
    savemanager.ui

RESOURCES += \
    resources.qrc
