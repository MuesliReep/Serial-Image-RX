#-------------------------------------------------
#
# Project created by QtCreator 2015-04-16T02:11:32
#
#-------------------------------------------------

QT       += core
QT       += serialport

QT       -= gui

TARGET = Serial-Image-RX
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Output.cpp \
    Datastream.cpp

HEADERS += \
    Output.h \
    Datastream.h
