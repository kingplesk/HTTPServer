#! [0]
TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../..
HEADERS       = filter.h
SOURCES       = filter.cpp
TARGET        = $$qtLibraryTarget(pnp_directoryFilter)
DESTDIR       = ../../server/plugins
#! [0]

# install
target.path = ../../server/plugins
sources.files = $$SOURCES $$HEADERS directory.pro
sources.path = ../../plugins/directory
INSTALLS += target sources
