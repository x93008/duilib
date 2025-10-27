# CMake file for CheckBoxDemo
# CheckBox 复选框控件演示程序

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(CheckBoxDemo ${SRC})
add_dependencies(CheckBoxDemo duilib)
target_link_libraries(CheckBoxDemo duilib)
set_target_properties(CheckBoxDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Install CheckBoxDemo.xml to bin/assets directory
install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/CheckBoxDemo.xml
  DESTINATION bin/assets
)

install(
  TARGETS CheckBoxDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)



