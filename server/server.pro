#-------------------------------------------------
#
# Project created by QtCreator 2012-03-16T21:55:49
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += script
QT       -= gui

TARGET = server

CONFIG   += console
#CONFIG   += qt debug_and_release
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    httprequest.cpp \
    #clientsocket.cpp \
    #httpresponse.cpp \
    httpparser.cpp \
    #clienthandler.cpp \
    test.cpp \
    #comet.cpp \
    http.cpp

HEADERS += \
    server.h \
    httprequest.h \
    #clientsocket.h \
    #httpresponse.h \
    httpparser.h \
    #clienthandler.h \
    myinterface.h \
    test.h \
    #comet.h \
    http.h \
    #http_p.h

OTHER_FILES += \
    qt.conf

# install
target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS server.pro qt.conf
sources.path = ./
INSTALLS += target sources
