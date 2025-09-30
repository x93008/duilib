#Cmake file for ListDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

set(source_files Main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(ListDemo ${source_files})
add_dependencies(ListDemo duilib)
target_link_libraries(ListDemo duilib)
set_target_properties(ListDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Install assets directly to bin directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/skin/ListRes/
  DESTINATION bin/assets/skin/ListRes
  FILES_MATCHING PATTERN "*"
)
install(FILES ${CMAKE_SOURCE_DIR}/assets/skin/ListRes.zip
  DESTINATION bin/assets/skin
)

install(
  TARGETS ListDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
