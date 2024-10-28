cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB impSRCS *.cc)
file(GLOB impHEADERS inc/*.h)

list(APPEND SRCS ${impSRCS})
list(APPEND HEADERS ${impHEADERS})

set(target_name "gl_widget_2d_impl")
add_library(${target_name} STATIC ${SRCS} ${HEADERS})
target_link_libraries(${target_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)

set(target_gl_widget_2d_impl ${target_name} PARENT_SCOPE)
