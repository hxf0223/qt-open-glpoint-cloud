cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(gl_cloud_widget_2d)

file(GLOB glcwSRCS gl_cloud_widget_2d/*.cc)
file(GLOB mainSRCS *.cc)
list(APPEND SRCS ${mainSRCS} ${glcwSRCS})

set(target_name gldraw_points_2d)
add_executable(${target_name} ${SRCS})

# set_target_properties(${target_name} PROPERTIES DEBUG_POSTFIX "_d")
target_link_libraries(${target_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)

if(WIN32)
    if(MSVC)
        set_target_properties(
            ${target_name} PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS
            "/ENTRY:mainCRTStartup")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
    else()
        message(SEND_ERROR "Unsupported Windows compiler! (Not MSVC or GCC)")
    endif(MSVC)
elseif(APPLE)
    set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE YES)
elseif(UNIX)
# Nothing special required
else()
    message(SEND_ERROR "Unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
