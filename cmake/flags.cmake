
add_definitions(-DWITH_LIBJPEG)
add_definitions(-DWITH_SSE)
add_definitions(-DWITH_LIBPNG)
add_definitions(-DWITH_LAPACK)

set(KORTEX_FLAGS_DEBUG    "-DDEBUG  -Wno-write-strings -Wall -Wno-deprecated -fno-strict-aliasing -fPIC  -g")
set(KORTEX_FLAGS_RELEASE  "-DNDEBUG -Wno-write-strings -Wall -Wno-deprecated -fno-strict-aliasing -fPIC -O3")

set(KORTEX_FLAGS_DEBUG     "${KORTEX_FLAGS_DEBUG}   -msse2 -msse4.2 -mfpmath=sse -fopenmp")
set(KORTEX_FLAGS_RELEASE   "${KORTEX_FLAGS_RELEASE} -msse2 -msse4.2 -mfpmath=sse -fopenmp")

set(KORTEX_FLAGS_DEBUG     "${KORTEX_FLAGS_DEBUG}    ")
set(KORTEX_FLAGS_RELEASE   "${KORTEX_FLAGS_RELEASE} -DHAVE_INLINE")

set(CMAKE_C_FLAGS_DEBUG      ${KORTEX_FLAGS_DEBUG}         )
set(CMAKE_C_FLAGS_RELEASE    ${KORTEX_FLAGS_RELEASE}       )

set(CMAKE_CXX_FLAGS_DEBUG   "${KORTEX_FLAGS_DEBUG}   -std=c++0x -rdynamic")
set(CMAKE_CXX_FLAGS_RELEASE "${KORTEX_FLAGS_RELEASE} -std=c++0x -rdynamic")

