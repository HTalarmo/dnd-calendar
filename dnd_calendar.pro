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


SOURCES += main.cpp\
        mainwindow.cpp \
    calendar.cpp \
    restdialog.cpp \
    setdate.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    calendar.h \
    restdialog.h \
    setdate.h \
    utils.h

FORMS    += mainwindow.ui \
    restdialog.ui \
    setdate.ui
