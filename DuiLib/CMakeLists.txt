# cmake file for duilib
#Author Qi Gao(monkgau@gmail.com)
#Created: 2012/09/16

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} Root_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Control Control_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core Core_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Layout Layout_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Utils Utils_src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Control)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Layout)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utils)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(duilib SHARED ${Control_src} ${Core_src} ${Layout_src} ${Utils_src} ${Root_src})

add_definitions(-DUILIB_EXPORTS)

# Set UTF-8 encoding for source files and runtime library to MT for static linking
if(WIN32 AND MSVC)
    target_compile_options(duilib PRIVATE /utf-8)
    # Set runtime library to MT (MultiThreaded) for static linking
    target_compile_options(duilib PRIVATE /MT$<$<CONFIG:Debug>:d>)
endif()

target_link_libraries(duilib comctl32)
target_include_directories(duilib PUBLIC $<INSTALL_INTERFACE:include>)
target_compile_definitions(duilib PRIVATE UILIB_EXPORTS)
set_target_properties(duilib PROPERTIES OUTPUT_NAME "duilib")

# Install headers
file(GLOB DUILIB_PUBLIC_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB DUILIB_CORE_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.h")
file(GLOB DUILIB_UTILS_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.h")
file(GLOB DUILIB_CONTROL_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/Control/*.h")
file(GLOB DUILIB_LAYOUT_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/Layout/*.h")

install(FILES ${DUILIB_PUBLIC_HDRS}     DESTINATION include/duilib)
install(FILES ${DUILIB_CORE_HDRS}       DESTINATION include/duilib/Core)
install(FILES ${DUILIB_UTILS_HDRS}      DESTINATION include/duilib/Utils)
install(FILES ${DUILIB_CONTROL_HDRS}    DESTINATION include/duilib/Control)
install(FILES ${DUILIB_LAYOUT_HDRS}     DESTINATION include/duilib/Layout)

# Install binaries
install(
  TARGETS duilib
  EXPORT  duilib-config
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# Install cmake configure files
install(EXPORT duilib-config
    NAMESPACE dui::
    DESTINATION share/duilib
)
