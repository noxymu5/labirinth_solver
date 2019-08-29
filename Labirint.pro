#-------------------------------------------------
#
# Project created by QtCreator 2017-11-09T20:22:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Labirint
TEMPLATE = app
CONFIG += qt warn_on release thread


SOURCES += main.cpp\
        labproj.cpp \
    runner.cpp \
    labirintcell.cpp

HEADERS  += labproj.h \
    runner.h \
    labirintcell.h

FORMS    += labproj.ui
