TEMPLATE = lib
QT += core
CONFIG += staticlib
SOURCES += \
        $$PWD/jsonmodel.cpp \
        $$PWD/jsonmodelrecord.cpp \
        $$PWD/utils/jsonmodelfield.cpp

HEADERS += \
        $$PWD/jsonmodel.h \
        $$PWD/jsonmodelrecord.h \
        $$PWD/jsonmodelfield.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
