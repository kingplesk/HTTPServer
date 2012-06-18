#! [0]

!include(../../config.pri):error("base config file (config.pri) not available")

BIN_DIR      = $${HOME_BIN}/bin/$${DEBUG_OR_RELEASE}/plugins
DESTDIR      = $${BIN_DIR}


TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../..
HEADERS       = paint.h \
                painted.h
SOURCES       = paint.cpp \
                painted.cpp
TARGET        = $$qtLibraryTarget(_paint)

sources.files = $$SOURCES $$HEADERS paint.pro
