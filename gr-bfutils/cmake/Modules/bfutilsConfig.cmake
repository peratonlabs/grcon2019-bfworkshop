INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BFUTILS bfutils)

FIND_PATH(
    BFUTILS_INCLUDE_DIRS
    NAMES bfutils/api.h
    HINTS $ENV{BFUTILS_DIR}/include
        ${PC_BFUTILS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BFUTILS_LIBRARIES
    NAMES gnuradio-bfutils
    HINTS $ENV{BFUTILS_DIR}/lib
        ${PC_BFUTILS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/bfutilsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BFUTILS DEFAULT_MSG BFUTILS_LIBRARIES BFUTILS_INCLUDE_DIRS)
MARK_AS_ADVANCED(BFUTILS_LIBRARIES BFUTILS_INCLUDE_DIRS)
