#-------------------------------------------------
#
# Project created by QtCreator 2011-07-20T20:31:00
#
#-------------------------------------------------

QT       += core network sql xml

QT       -= gui

TARGET = Zoidblog
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    httpserver.cpp \
    http_parser.c \
    httpheader.cpp \
    worker.cpp \
    swift.cpp

HEADERS += \
    httpserver.h \
    http_parser.h \
    httpheader.h \
    worker.h \
    swift.h
