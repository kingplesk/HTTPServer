
#CONFIG   += qt debug_and_release
CONFIG   += ordered

TEMPLATE = subdirs

SUBDIRS += \
    server \
    plugins

OTHER_FILES += \
    README.txt

server.depends = plugins

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS server.pro README.txt
