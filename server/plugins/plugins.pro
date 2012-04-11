TEMPLATE = subdirs

SUBDIRS += \
    directory \
    export

# install
#target.path = ../plugins
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS plugins.pro
#sources.path = ../plugins
#INSTALLS += target sources
