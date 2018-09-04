#kortex library
#base components of the kortex vision library developed by Aurvis R&D
#author:Engin Tola
#license: see license.txt
cmake_minimum_required(VERSION 3.0)

project(kortex VERSION 1.0.0)

include(GNUInstallDirs)

set( CMAKE_VERBOSE_MAKEFILE on )

set(VERSION 1.0)
# set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/usr/local/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR} )

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)

# set(EXTERNAL_LIBS png jpeg blas lapack)
INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIR})
set(EXTERNAL_LIBS ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})

include(cmake/sources.cmake)
include(cmake/flags.cmake)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# add_library(kortex SHARED ${kortex_SOURCES})
add_library(kortex STATIC ${kortex_SOURCES})

set_target_properties(kortex PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)
target_link_libraries(kortex "${EXTERNAL_LIBS}")

target_include_directories(
    kortex PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${generated_headers}>"
)


configure_file(${CMAKE_SOURCE_DIR}/cmake/kortex.pc.in ${CMAKE_BINARY_DIR}/kortex.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/kortex.pc DESTINATION lib/pkgconfig)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")


# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}${CMAKE_INSTALL_INCLUDEDIR}")
set(CONF_LIB_DIR "${CMAKE_INSTALL_PREFIX}${CMAKE_INSTALL_LIBDIR}")
set(CONF_EXTERNAL_LIBS "${EXTERNAL_LIBS}" )
get_directory_property( CONF_COMP_DEFINITIONS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )

configure_package_config_file(
    "cmake/config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
install(
    TARGETS kortex
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

message( "config_install_dir: ${CMAKE_INSTALL_INCLUDEDIR}" )

# Headers:
install(
    DIRECTORY "include/kortex"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Config
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

