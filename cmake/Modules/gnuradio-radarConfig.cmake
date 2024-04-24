find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_RADAR gnuradio-radar)

FIND_PATH(
    GR_RADAR_INCLUDE_DIRS
    NAMES gnuradio/radar/api.h
    HINTS $ENV{RADAR_DIR}/include
        ${PC_RADAR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_RADAR_LIBRARIES
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

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-radarTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_RADAR DEFAULT_MSG GR_RADAR_LIBRARIES GR_RADAR_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_RADAR_LIBRARIES GR_RADAR_INCLUDE_DIRS)
