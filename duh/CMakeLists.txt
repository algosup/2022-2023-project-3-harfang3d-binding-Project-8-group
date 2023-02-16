
cmake_minimum_required(VERSION 3.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

project(hg)
enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(hg SHARED "wrapper.cpp")
set_target_properties(hg PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "./duh")

install(TARGETS hg DESTINATION "${CMAKE_SOURCE_DIR}/" COMPONENT hg)
