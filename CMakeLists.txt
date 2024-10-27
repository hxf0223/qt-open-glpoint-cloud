cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(CloudPointsViewer CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "MSVC_VERSION = ${MSVC_VERSION}")
  message(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION}")
endif()

# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to be used")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE STRING "C++ standard to be used")

if(NOT CMAKE_CONFIGURATION_TYPES)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
    Release
    CACHE STRING "Release" FORCE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(-DSPDLOG_WCHAR_TO_UTF8_SUPPORT) # VS 输出窗口支持 UTF8
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
  add_compile_definitions(-DNDEBUG)
else()
  add_compile_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_POLICY_DEFAULT_CMP0079 NEW)
  cmake_policy(SET CMP0074 NEW) # PCL_ROOT not set warning
  cmake_policy(SET CMP0144 NEW)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # https://cmake.org/cmake/help/latest/variable/CMAKE_MSVC_RUNTIME_LIBRARY.html#variable:CMAKE_MSVC_RUNTIME_LIBRARY
  cmake_policy(SET CMP0091 NEW)
  cmake_policy(SET CMP0167 NEW)
endif()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /GS /TP /W3 /Zc:wchar_t
# /DAPL_CORE_UWP  /Zc:inline /fp:precise /DWIN32_LEAN_AND_MEAN /DNOGDI /DWIN32
# /D_WINDOWS /D_USE_MATH_DEFINES /utf-8")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/release")
  else()
    list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/debug")
  endif()

  # list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/pcl_1.14.1")
  # list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/pcl_1.14.1/3rdParty/Boost")
  list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/opencv/x64/vc16/lib")
  list(APPEND CMAKE_PREFIX_PATH "D:/dev_libs/oneAPI/tbb/latest/lib")
endif()

if(MSVC)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ox /Ot /Qpar /Gy /Gw /GF")
  endif()

  string(REGEX REPLACE "/W[1-3]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}\n")

# find package Qt5
find_package(
  Qt5
  COMPONENTS Core Gui Widgets Network Svg Charts OpenGL
  REQUIRED)

message(STATUS "Qt5 core libraries: ${Qt5Core_LIBRARIES}")
message(STATUS "Qt5 widgets libraries: ${Qt5Widgets_LIBRARIES}")
message(STATUS "Qt5 widgets include dirs: ${Qt5Widgets_INCLUDE_DIRS}\n")

# package boost
find_package(Boost REQUIRED COMPONENTS filesystem regex)
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")

# package opencv
find_package(OpenCV CONFIG REQUIRED)
message(STATUS "OpenCV lib dirs: ${OpenCV_LIB_DIRS}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}\n")

option(ENABLE_PCL "Enable PCL" OFF)
set(PACKAGE_PCL_FOUND OFF)

if(ENABLE_PCL)
  find_package(PCL)

  if(NOT PCL_FOUND)
    message(FATAL_ERROR "PCL not found.")
    set(PACKAGE_PCL_FOUND ON)
  else()
    set(PACKAGE_PCL_FOUND ON)
    add_definitions(${PCL_DEFINITIONS})
    message(STATUS "PCL libs: ${PCL_LIBRARIES}\n")
    message(STATUS "PCL definitions: ${PCL_DEFINITIONS}")
    message(STATUS "PCL include dirs: ${PCL_INCLUDE_DIRS}\n")
  endif()
endif(ENABLE_PCL)

# option(ENABLE_TBB "Enable intel TBB" OFF)
find_package(TBB CONFIG REQUIRED)
message(STATUS "TBB version: ${TBB_VERSION}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin") # /lib

# avoid build errors caused by chinese character in source code
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_definitions(-DQT_MESSAGELOGCONTEXT)

include_directories(system ${PROJECT_SOURCE_DIR}/3rd/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)
