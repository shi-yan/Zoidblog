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
    session.cpp \
    sessionmanager.cpp \
    pathparser.cpp \
    pathtreenode.cpp \
    taskhandler.cpp \
    webapp.cpp \
    pathtree.cpp \
    zoidblog.cpp \
    framework.cpp \
    httprequest.cpp \
    httpresponse.cpp \
    incommingconnectionqueue.cpp

HEADERS += \
    httpserver.h \
    http_parser.h \
    httpheader.h \
    worker.h \
    session.h \
    sessionmanager.h \
    pathparser.h \
    pathtreenode.h \
    taskhandler.h \
    webapp.h \
    pathtree.h \
    zoidblog.h \
    framework.h \
    httprequest.h \
    httpresponse.h \
    incommingconnectionqueue.h
