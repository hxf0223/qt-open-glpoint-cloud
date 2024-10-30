cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(inc)

file(GLOB apiSRCS *.cc)
file(GLOB apiHEADERS ${PROJECT_SOURCE_DIR}/include/gl_widget_2d/*.h)

list(APPEND SRCS ${apiSRCS})
list(APPEND HEADERS ${apiHEADERS})

set(target_name "gl_widget_2d")
add_library(${target_name} SHARED ${SRCS} ${HEADERS})
target_link_libraries(${target_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)
target_link_libraries(${target_name} PRIVATE ${target_gl_widget_2d_impl})
target_compile_definitions(${target_name} PRIVATE -DGL_WIDGET_2D_LIB)

set_target_properties(${target_name} PROPERTIES CXX_VISIBILITY_PRESET "hidden")
set(target_gl_widget_2d ${target_name} PARENT_SCOPE)
