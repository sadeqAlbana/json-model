cmake_minimum_required(VERSION 3.16)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Qml)


qt_add_library(jsonmodel STATIC
    jsonmodel.cpp jsonmodel.h
    jsonmodelcolumn.h jsonmodelcolumn.cpp
    networkedjsonmodel.cpp networkedjsonmodel.h
)
qt_add_qml_module(jsonmodel
    URI "JsonModels"
    VERSION 1.0
    SOURCES
    qmljsonmodelcolumn.h qmljsonmodelcolumn.cpp
    qmljsonmodel.h qmljsonmodel.cpp

)

target_link_libraries(jsonmodel PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml)

target_include_directories(jsonmodel INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
