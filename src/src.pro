TEMPLATE = lib
QT += core
CONFIG += staticlib
TARGET    = json-model
SOURCES += \
        $$PWD/jsonmodel.cpp \
        $$PWD/networkedjsonmodel.cpp

HEADERS += \
        $$PWD/jsonmodel.h \
        $$PWD/networkedjsonmodel.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
