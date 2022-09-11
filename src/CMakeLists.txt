cmake_minimum_required(VERSION 3.16)

find_package(Qt6  COMPONENTS Core REQUIRED)

add_library(json-model STATIC
    jsonmodel.cpp jsonmodel.h
    networkedjsonmodel.cpp networkedjsonmodel.h
)

target_link_libraries(json-model PRIVATE Qt6::Core)
target_include_directories(json-model INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
