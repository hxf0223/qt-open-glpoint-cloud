cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${PROJECT_SOURCE_DIR}/src/gl_widget_2d/include)

file(GLOB mainHEADERS *.h)
file(GLOB mainSRCS *.cc)
list(APPEND SRCS ${mainSRCS} ${mainHEADERS})

set(target_name test_gl_cloud_widget_2d)
add_executable(${target_name} ${SRCS})

# set_target_properties(${target_name} PROPERTIES DEBUG_POSTFIX "_d")
target_link_libraries(${target_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)
target_link_libraries(${target_name} PRIVATE ${target_gl_widget_2d})
message(STATUS ">>> Linking with ${target_gl_widget_2d}")

set_target_properties(${target_name} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)