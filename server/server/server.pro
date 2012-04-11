#-------------------------------------------------
#
# Project created by QtCreator 2012-03-16T21:55:49
#
#-------------------------------------------------

!include(../config.pri):error("base config file (config.pri) not available")

QT       += core
QT       += network
QT       += script
QT       -= gui

TARGET    = server

CONFIG   += console
CONFIG   -= app_bundle
#CONFIG   += qt debug_and_release

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    httprequest.cpp \
    #clientsocket.cpp \
    httpresponse.cpp \
    httpparser.cpp \
    clienthandler.cpp \
    test.cpp \
    #comet.cpp \
    http.cpp \
    requesthandler.cpp

HEADERS += \
    server.h \
    httprequest.h \
    #clientsocket.h \
    httpresponse.h \
    httpparser.h \
    clienthandler.h \
    myinterface.h \
    test.h \
    #comet.h \
    http.h \
    #http_p.h
    requesthandler.h

OTHER_FILES += \
    qt.conf

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS server.pro qt.conf
