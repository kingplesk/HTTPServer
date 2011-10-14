#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T20:48:51
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += script

QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    httprequest.cpp \
    clientsocket.cpp \
    httpresponse.cpp \
    httpparser.cpp \
    clienthandler.cpp

HEADERS += \
    server.h \
    httprequest.h \
    clientsocket.h \
    httpresponse.h \
    httpparser.h \
    clienthandler.h

OTHER_FILES += \
    README.txt
