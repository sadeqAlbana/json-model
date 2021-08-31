TEMPLATE = lib
QT += core
CONFIG += staticlib
TARGET    = json-model
SOURCES += \
        $$PWD/src/jsonmodel.cpp \
        $$PWD/src/jsonmodelrecord.cpp \
        $$PWD/src/jsonmodelfield.cpp \
        $$PWD/src/networkedjsonmodel.cpp

HEADERS += \
        $$PWD/src/jsonmodel.h \
        $$PWD/src/jsonmodelrecord.h \
        $$PWD/src/jsonmodelfield.h \
        $$PWD/src/networkedjsonmodel.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
