#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T16:49:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PainBot3Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    kinematics.cpp \
    forwardKinematics.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    kinematics.h \
    forwardKinematics.h

FORMS    += mainwindow.ui \
    dialog.ui
