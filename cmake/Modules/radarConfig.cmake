INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_RADAR radar)

FIND_PATH(
    RADAR_INCLUDE_DIRS
    NAMES radar/api.h
    HINTS $ENV{RADAR_DIR}/include
        ${PC_RADAR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    RADAR_LIBRARIES
    NAMES gnuradio-radar
    HINTS $ENV{RADAR_DIR}/lib
        ${PC_RADAR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RADAR DEFAULT_MSG RADAR_LIBRARIES RADAR_INCLUDE_DIRS)
MARK_AS_ADVANCED(RADAR_LIBRARIES RADAR_INCLUDE_DIRS)

