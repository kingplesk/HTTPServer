
CONFIG   += debug
CONFIG   += warn_off

CONFIG(debug, debug|release)|CONFIG(DebugBuild) {
        CONFIG -= debug release
        CONFIG += debug
        DEBUG_OR_RELEASE = debug
        DEBUG_TAG = _debug
} else {
        CONFIG -= debug release
        CONFIG += release
        DEBUG_OR_RELEASE = release
        DEBUG_TAG =
        DEFINES += NDEBUG
}

#$$(PWD)
HOME_BIN = /home/eri/Qt
HOME_BUILD = /home/eri/Qt/server-build

LIB_DIR      = $${HOME_BIN}/libs/$${DEBUG_OR_RELEASE}
BIN_DIR      = $${HOME_BIN}/bin/$${DEBUG_OR_RELEASE}
OBJECTS_DIR  = build/$${DEBUG_OR_RELEASE}
MOC_DIR      = build/gen/moc
UI_DIR       = build/gen/ui

message( -------------- $${BIN_DIR})
message( -------------- $${DEBUG_OR_RELEASE})

QMAKE_LIBDIR += $${LIB_DIR}
DESTDIR       = $${BIN_DIR}

