#-------------------------------------------------
#
# Project created by QtCreator 2017-12-26T21:13:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TinyPhotoshop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    colorinspector.cpp \
    baseinspector.cpp \
    binaryinspector.cpp \
    arithmeticinspector.cpp \
    geometryinspector.cpp \
    contrastinspector.cpp \
    histograminspector.cpp \
    imagehistogram.cpp \
    imagefilter.cpp \
    smoothinginspector.cpp \
    edgedetectioninspector.cpp \
    binaryimage.cpp \
    bwmorph.cpp \
    bwmorphinspector.cpp \
    uiutility.cpp \
    signedimage.cpp

HEADERS += \
        mainwindow.h \
    colorinspector.h \
    baseinspector.h \
    binaryinspector.h \
    arithmeticinspector.h \
    geometryinspector.h \
    contrastinspector.h \
    histograminspector.h \
    imagehistogram.h \
    imagefilter.h \
    smoothinginspector.h \
    edgedetectioninspector.h \
    binaryimage.h \
    bwmorph.h \
    bwmorphinspector.h \
    uiutility.h \
    signedimage.h

FORMS += \
        mainwindow.ui \
    colorinspector.ui \
    binaryinspector.ui \
    arithmeticinspector.ui \
    geometryinspector.ui \
    contrastinspector.ui \
    histograminspector.ui \
    smoothinginspector.ui \
    edgedetectioninspector.ui \
    bwmorphinspector.ui
