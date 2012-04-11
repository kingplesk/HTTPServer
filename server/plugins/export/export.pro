#! [0]

!include(../../config.pri):error("base config file (config.pri) not available")

BIN_DIR      = $${ERI_BIN}/bin/$${DEBUG_OR_RELEASE}/plugins
DESTDIR      = $${BIN_DIR}


TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../..
HEADERS       = filter.h
SOURCES       = filter.cpp
TARGET        = $$qtLibraryTarget(_exportFilter)

sources.files = $$SOURCES $$HEADERS export.pro
