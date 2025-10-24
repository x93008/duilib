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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # 警告级别设置到最高
    string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # 多线程编译，不捕获c异常，
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} /EHsc /MP /Zc:__cplusplus")

    # relase版本也产生调试信息(RelWithDebInfo会做这个事，但是现有项目都不会构建RelWithDebInfo)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")

    # relase版本也产生调试信息, 移出未被使用的符号，合并内容相同的函数
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")

    # 动态库设置符号导出到PDB中
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    if (NOT BUILD_SHARED_LIBS)
        # 静态库生成调试信息并嵌入到目标文件中
        string (REGEX REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        string (REGEX REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    endif()

    # 禁用安全异常处理
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

include_directories(${CMAKE_SOURCE_DIR})

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
    add_subdirectory(EditDemo)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION share/${PROJECT_NAME})
