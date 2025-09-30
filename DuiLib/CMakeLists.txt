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

# Use CMake's standard BUILD_SHARED_LIBS option
add_library(duilib ${Control_src} ${Core_src} ${Layout_src} ${Utils_src} ${Root_src})

# Set appropriate compile definitions based on library type
get_target_property(DUILIB_TYPE duilib TYPE)
if(DUILIB_TYPE STREQUAL "STATIC_LIBRARY")
    target_compile_definitions(duilib PUBLIC UILIB_STATIC)
    message(STATUS "Building DuiLib as STATIC library")
else()
    target_compile_definitions(duilib PRIVATE UILIB_EXPORTS)
    message(STATUS "Building DuiLib as SHARED library")
endif()

# Set UTF-8 encoding for source files and runtime library to MT for static linking
if(WIN32 AND MSVC)
    target_compile_options(duilib PRIVATE /utf-8)
endif()

# Link system libraries that DuiLib depends on
target_link_libraries(duilib PUBLIC 
    winmm           # Multimedia API
    comctl32        # Common Controls
    shlwapi         # Shell Lightweight API
    imm32           # Input Method Manager
    gdiplus         # GDI+ Graphics
    uuid            # GUID support
)
target_include_directories(duilib PUBLIC $<INSTALL_INTERFACE:include>)
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

# Install PDB files for Debug builds on MSVC (shared library only)
get_target_property(DUILIB_TYPE duilib TYPE)
if(MSVC AND NOT DUILIB_TYPE STREQUAL "STATIC_LIBRARY")
    install(FILES $<TARGET_PDB_FILE:duilib>
            DESTINATION bin 
            CONFIGURATIONS Debug RelWithDebInfo
            OPTIONAL)
endif()

# Install cmake configure files
install(EXPORT duilib-config
    NAMESPACE dui::
    DESTINATION share/duilib
)
