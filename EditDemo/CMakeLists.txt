# CMake file for EditDemo
# Edit 控件演示程序

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(EditDemo ${SRC})
add_dependencies(EditDemo duilib)
target_link_libraries(EditDemo duilib)
set_target_properties(EditDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Install EditDemo.xml to bin/assets directory
install(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/EditDemo.xml
  DESTINATION bin/assets
)

install(
  TARGETS EditDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

