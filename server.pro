CONFIG   += qt debug_and_release
CONFIG   += ordered

TEMPLATE = subdirs

SUBDIRS += \
    server \
    plugins

server.depends = plugins

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS server.pro README.txt
sources.path = ./
INSTALLS += target sources

OTHER_FILES += \
    README.txt
