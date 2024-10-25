cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-D_SILENCE_CXX20_U8PATH_DEPRECATION_WARNING)

include_directories(inc)
include_directories(csv_reader)

qt5_add_resources(qrc_FILES ${CMAKE_CURRENT_SOURCE_DIR}/opengl/shader.qrc ${CMAKE_CURRENT_SOURCE_DIR}/data/pointcloud.qrc)

file(GLOB mainSRCS *.cc)
file(GLOB openglSRCS ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.cc)
file(GLOB csvrSRCS csv_reader/*.cc)
list(APPEND SRCS ${mainSRCS} ${openglSRCS} ${csvrSRCS})

set(target_name gl_cloud_viewer_3d)
add_executable(${target_name} ${SRCS} frmmain.ui ${qrc_FILES})

# set_target_properties(${target_name} PROPERTIES DEBUG_POSTFIX "_d")
target_link_libraries(${target_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)
target_link_libraries(${target_name} PRIVATE TBB::tbb ${Boost_LIBRARIES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_link_libraries(${target_name} PUBLIC pthread dl)
endif()

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
