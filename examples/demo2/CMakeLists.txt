cmake_minimum_required(VERSION 3.14)

#project(jsonmodeldemo2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS  Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick)

qt_add_executable(jsonmodeldemo2
  main.cpp
)

qt_add_qml_module(jsonmodeldemo2
    URI "JsonModelDemo2"
    VERSION 1.0
    IMPORTS JsonModels
    QML_FILES
        main.qml
)
set_target_properties(jsonmodeldemo2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_compile_definitions(jsonmodeldemo2
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


target_link_libraries(jsonmodeldemo2 PRIVATE Qt${QT_VERSION_MAJOR}::Quick jsonmodel jsonmodelplugin)

install(TARGETS jsonmodeldemo2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

