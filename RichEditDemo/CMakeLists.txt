# CMake file for RichEditDemo
# RichEdit 多行文本编辑控件演示程序

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(RichEditDemo ${SRC})
add_dependencies(RichEditDemo duilib)
target_link_libraries(RichEditDemo duilib)
set_target_properties(RichEditDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Install RichEditDemo.xml to bin/assets directory
install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/RichEditDemo.xml
  DESTINATION bin/assets
)

install(
  TARGETS RichEditDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


