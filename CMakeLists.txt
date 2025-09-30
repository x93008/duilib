# CMake root file for duilib related apps
#Date 2012-09-16
#Author: Qi Gao(monkgau@gmail.com)
#
#  CMake files for duilib
#  file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 3.26)

project(dui)

option(DUILIB_BUILD_EXAMPLES "Build examples" OFF)

message(STATUS," CMake project files for duilib")

# this line is for UNICODE release,which is required by DuiDesigner 
add_definitions(-DUNICODE -D_UNICODE)

# Set UTF-8 encoding for source files
if(WIN32 AND MSVC)
    # Force UTF-8 encoding for both source and execution character sets
    add_compile_options(/utf-8)
endif()

# platform config
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "default install prefix" FORCE)
endif()

# add each CMake file
add_subdirectory(duilib)

if (DUILIB_BUILD_EXAMPLES)
    add_subdirectory(360SafeDemo)
    add_subdirectory(FlashDemo)
    add_subdirectory(GameDemo)
    add_subdirectory(ListDemo)
    add_subdirectory(MenuDemo)
    add_subdirectory(QQDemo)
    add_subdirectory(RichListDemo)
    add_subdirectory(FileTree)
    add_subdirectory(ScrCapture)
    add_subdirectory(TestApp1)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION share/${PROJECT_NAME})
