#Cmake file for RichListDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})                           


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(RichListDemo ${SRC_LIST} RichListDemo.rc)
set_target_properties(RichListDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_link_libraries(RichListDemo duilib)

# Install assets directly to bin directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/skin/RichListRes/
  DESTINATION bin/assets/skin/RichListRes
  FILES_MATCHING PATTERN "*"
)
install(FILES ${CMAKE_SOURCE_DIR}/assets/skin/RichListRes.zip
  DESTINATION bin/assets/skin
)

install(
  TARGETS RichListDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
