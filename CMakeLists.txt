CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(kortex VERSION 1.2.3 )

# fix CMake IntDir variable
if(MSVC AND "${MSVC_VERSION}" STRGREATER "1500")
	SET(CMAKE_CFG_INTDIR "$(Platform)/$(Configuration)")
endif()
SET(COTIRE_INTDIR "cotire")

# Define helper functions and macros.
# cmake_policy(SET CMP0011 OLD)
INCLUDE(cmake/utils.cmake)
if(ENABLE_PRECOMPILED_HEADERS)
	INCLUDE(cmake/cotire.cmake)
endif()

# Init session with macros defined in Utils.cmake
GetOperatingSystemArchitectureBitness(SYSTEM)
ComposePackageLibSuffix()
ConfigCompilerAndLinker()
ConfigLibrary()

set(CMAKE_CXX_STANDARD 11)
if(MSVC)
  set(CMAKE_INSTALL_PREFIX     "C:/usr")
  set(CMAKE_INSTALL_INCLUDEDIR "include/" )
  set(CMAKE_INSTALL_LIBDIR     "lib/" )
  set(CMAKE_INSTALL_BINDIR     "bin/" )
else()
  include(GNUInstallDirs)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter")
endif()

# List configuration options
SET(PROJECT_WITH_SSE ON CACHE BOOL "Enable SSE optimizations")
SET(PROJECT_WITH_PNG ON CACHE BOOL "Enable PNG file support")
SET(PROJECT_WITH_JPEG ON CACHE BOOL "Enable JPEG file support")
SET(PROJECT_WITH_BLAS ON CACHE BOOL "Enable BLAS support")
SET(PROJECT_WITH_LAPACK ON CACHE BOOL "Enable LAPACK support")
SET(PROJECT_WITH_OPENMP ON CACHE BOOL "Enable OpenMP library")

SET(PROJECT_CONFIG_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the build/platform specific header")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")



# Find required packages
set(EXTERNAL_LIBS "")

if(PROJECT_WITH_OPENMP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(WITH_OPENMP TRUE)
  else()
    message("-- Can't find OpenMP. Continuing without it.")
    set(PROJECT_WITH_OPENMP FALSE)
  endif()
endif()


if(PROJECT_WITH_PNG)
  find_package(PNG REQUIRED)
  if(PNG_FOUND)
    message("-- PNG found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PNG_CXX_FLAGS}")
    include_directories(${PNG_INCLUDE_DIRS})
    link_directories(${PNG_LIBRARY_DIRS})
    set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${PNG_LIBRARIES})
    set(WITH_PNG TRUE)
  else()
    message("-- Can't find libpng. Continuing without it.")
    set(PROJECT_WITH_PNG FALSE)
  endif()
endif()

if(PROJECT_WITH_JPEG)
  find_package(JPEG REQUIRED)
  if(JPEG_FOUND)
    message("-- JPEG found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${JPEG_CXX_FLAGS}")
    include_directories(${JPEG_INCLUDE_DIR})
    link_directories(${JPEG_LIBRARY_DIRS})
    set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${JPEG_LIBRARIES})
    set(WITH_JPEG TRUE)
  else()
    message("-- Can't find libjpeg. Continuing without it.")
    set(PROJECT_WITH_JPEG FALSE)
  endif()
endif()

if(PROJECT_WITH_BLAS)
  find_package(BLAS REQUIRED)
  if(BLAS_FOUND)
    message("-- BLAS found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BLAS_CXX_FLAGS}")
    include_directories(${BLAS_INCLUDE_DIRS})
    link_directories(${BLAS_LIBRARY_DIRS})
    set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${BLAS_LIBRARIES})
    set(WITH_BLAS TRUE)
  else()
    message("-- Can't find blas. Continuing without it.")
    set(PROJECT_WITH_BLAS FALSE)
  endif()
endif()

if(PROJECT_WITH_LAPACK)
  find_package(LAPACK REQUIRED)
  if(LAPACK_FOUND)
    message("-- LAPACK found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LAPACK_CXX_FLAGS}")
    include_directories(${LAPACK_INCLUDE_DIRS})
    link_directories(${LAPACK_LIBRARY_DIRS})
    set(EXTERNAL_LIBS ${EXTERNAL_LIBS} ${LAPACK_LIBRARIES})
    set(WITH_LAPACK TRUE)
  else()
    message("-- Can't find blas. Continuing without it.")
    set(PROJECT_WITH_LAPACK FALSE)
  endif()
endif()

# Set defines
SET(PROJECT_DEFINITIONS "")
if(PROJECT_WITH_SSE)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_SSE)
	SET(WITH_SSE TRUE)
endif()
if(PROJECT_WITH_PNG)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_LIBPNG)
	SET(WITH_LIBPNG TRUE)
endif()
if(PROJECT_WITH_JPEG)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_LIBJPEG)
	SET(WITH_LIBJPEG TRUE)
endif()
if(PROJECT_WITH_LAPACK)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_LAPACK)
	SET(WITH_LAPACK TRUE)
endif()
if(PROJECT_WITH_BLAS)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_BLAS)
	SET(WITH_BLAS TRUE)
endif()
if(PROJECT_WITH_OPENMP)
	LIST(APPEND PROJECT_DEFINITIONS -DWITH_OPENMP)
	SET(WITH_OPENMP TRUE)
endif()
add_definitions(${PROJECT_DEFINITIONS})



# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

include(cmake/sources.cmake)

set(CMAKE_BUILD_TYPE Release)

add_library( ${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

set_target_properties( ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)

# causes issues in windows
# target_link_libraries( ${PROJECT_NAME} "${EXTERNAL_LIBS}")
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${generated_headers}>"
)

configure_file(${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)

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
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
set(CONF_LIB_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CONF_EXTERNAL_LIBS "${EXTERNAL_LIBS}" )
set(CONF_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
get_directory_property( CONF_COMP_DEFINITIONS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )

configure_package_config_file(
    "cmake/config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

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


# uninstall target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
