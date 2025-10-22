#Cmake file for MenuDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

set(source_files 
    main_frame.cpp
    main.cpp
    StdAfx.cpp
    UICrack.cpp
    UIMenu.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/DuiLib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(MenuDemo ${source_files} MenuDemo.rc)
add_dependencies(MenuDemo duilib)
set_target_properties(MenuDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_link_libraries(MenuDemo duilib)

# Set UTF-8 encoding and runtime library
if(WIN32 AND MSVC)
    target_compile_options(MenuDemo PRIVATE /utf-8)
    target_compile_options(MenuDemo PRIVATE /MT$<$<CONFIG:Debug>:d>)
endif()

# Install assets directly to bin directory
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/
  DESTINATION bin/res
  FILES_MATCHING PATTERN "*"
)

install(
  TARGETS MenuDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
