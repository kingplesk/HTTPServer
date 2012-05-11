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

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app

#---
# Qjson
#---
QJSON_LIB = $${HOME_BIN}/qjson/lib
system('cp $${QJSON_LIB}/libqjson.so* $${LIB_DIR}/')

LIBS += -L"$${LIB_DIR}" -l"qjson"
INCLUDEPATH += "../../../qjson/include"
#---

SOURCES += main.cpp \
    server.cpp \
    httprequest.cpp \
    httpresponse.cpp \
    httpparser.cpp \
    clienthandler.cpp \
    #comet.cpp \
    http.cpp \
    requesthandler.cpp \

HEADERS += \
    server.h \
    httprequest.h \
    httpresponse.h \
    httpparser.h \
    clienthandler.h \
    myinterface.h \
    #comet.h \
    http.h \
    #http_p.h
    requesthandler.h \

OTHER_FILES += \
    qt.conf

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS server.pro qt.conf
