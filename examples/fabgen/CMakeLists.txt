
cmake_minimum_required(VERSION 3.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
add_subdirectory(matplotplusplus)
project(mayplot)
enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)
add_library(matplot_l SHARED "wrapper.cpp")
target_link_libraries(matplot_l PUBLIC matplot)
set_target_properties(matplot_l PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "C:/Users/louis/Documents/matplot/fabgen")

install(TARGETS matplot_l DESTINATION "${CMAKE_SOURCE_DIR}/" COMPONENT matplot_l)
