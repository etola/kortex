#kortex library
#base components of the kortex vision library developed by Aurvis R&D
#author:Engin Tola
#license: see license.txt
cmake_minimum_required(VERSION 2.6)

project(kortex)

set( CMAKE_VERBOSE_MAKEFILE on )

set(VERSION 1.0)
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/usr/local/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR} )

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)

set(EXTERNAL_LIBS png jpeg blas lapack)

include(cmake/sources.cmake)
include(cmake/flags.cmake)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

add_library(kortex SHARED ${kortex_SOURCES})
# add_library(kortex SHARED ${kortex_SOURCES})

set_target_properties(kortex PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)
target_link_libraries(kortex "${EXTERNAL_LIBS};${JPEG_LIBRARIES};${PNG_LIBRARIES}")
include_directories("include;${JPEG_INCLUDE_DIR};${PNG_INCLUDE_DIR}")
# include_directories(include)

configure_file(${CMAKE_SOURCE_DIR}/cmake/kortex.pc.in ${CMAKE_BINARY_DIR}/kortex.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/kortex.pc DESTINATION lib/pkgconfig)

install(TARGETS kortex
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install (DIRECTORY include
         DESTINATION .)
