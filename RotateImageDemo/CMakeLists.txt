# CMake file for RotateImageDemo
# RotateImage 旋转图片控件演示程序

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(RotateImageDemo ${SRC})
add_dependencies(RotateImageDemo duilib)
target_link_libraries(RotateImageDemo duilib)
set_target_properties(RotateImageDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# Install RotateImageDemo.xml and image files to bin/assets directory
install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/RotateImageDemo.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/loading.png
  ${CMAKE_CURRENT_SOURCE_DIR}/success.png
  ${CMAKE_CURRENT_SOURCE_DIR}/fail.png
  DESTINATION bin/assets
)

install(
  TARGETS RotateImageDemo
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

