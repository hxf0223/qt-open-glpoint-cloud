cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(imp/inc)

add_subdirectory(imp)
add_subdirectory(src)

set(target_gl_widget_2d ${target_gl_widget_2d} PARENT_SCOPE)
