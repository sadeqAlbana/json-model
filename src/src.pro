TEMPLATE = lib
QT += core
CONFIG += staticlib
TARGET    = json-model
SOURCES += \
        $$PWD/jsonmodel.cpp \
        $$PWD/jsonmodelrecord.cpp \
        $$PWD/jsonmodelfield.cpp \
        $$PWD/networkedjsonmodel.cpp

HEADERS += \
        $$PWD/jsonmodel.h \
        $$PWD/jsonmodelrecord.h \
        $$PWD/jsonmodelfield.h \
        $$PWD/networkedjsonmodel.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
