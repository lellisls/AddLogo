#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:48:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AddLogo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ipmodule.cpp

HEADERS  += mainwindow.h \
    ipmodule.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

win32{
RC_FILE = windows.rc
DISTFILES += windows.rc
LIBS += -lGdiplus
DEFINES += IS_WINDOWS_PLATFORM

SOURCES += qimgorient.cpp
HEADERS += qimgorient.h
}

DISTFILES += \
    windows.rc

