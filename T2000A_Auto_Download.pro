#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T14:27:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = T2000A_Auto_Download
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    burningdevice.cpp \
    qdeviceflashmanager.cpp \
    flashcommands.cpp \
    texthelper.cpp \
    detectdevice.cpp \
    flashdevice.cpp \
    mesnetwork.cpp \
    settingfwver.cpp

HEADERS  += mainwindow.h \
    burningdevice.h \
    qdeviceflashmanager.h \
    flashcommands.h \
    texthelper.h \
    detectdevice.h \
    flashdevice.h \
    mesnetwork.h \
    settingfwver.h

FORMS    += mainwindow.ui \
    burningdevice.ui \
    settingfwver.ui
