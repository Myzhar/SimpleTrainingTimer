#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T10:43:54
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleTrainingTimer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qtimecomposingwidget.cpp

HEADERS  += mainwindow.h \
    qtimecomposingwidget.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resources.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

