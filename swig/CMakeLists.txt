# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT radar_sources)
	MESSAGE(STATUS "No C++ sources... skipping swig/")
	return()
endif(NOT radar_sources)

########################################################################
# Include swig generation macros
########################################################################
find_package(SWIG)
find_package(PythonLibs 2)
if(NOT SWIG_FOUND OR NOT PYTHONLIBS_FOUND)
    return()
endif()
include(GrSwig)
include(GrPython)

########################################################################
# Setup swig generation
########################################################################
foreach(incdir ${GNURADIO_RUNTIME_INCLUDE_DIRS})
    list(APPEND GR_SWIG_INCLUDE_DIRS ${incdir}/gnuradio/swig)
endforeach(incdir)

set(GR_SWIG_LIBRARIES gnuradio-radar)
#set(GR_SWIG_DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/radar_swig_doc.i) # Outcomment swig documentation for python docs
#set(GR_SWIG_DOC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include)

GR_SWIG_MAKE(radar_swig radar_swig.i)

########################################################################
# Install the build swig module
########################################################################
GR_SWIG_INSTALL(TARGETS radar_swig DESTINATION ${GR_PYTHON_DIR}/radar)

########################################################################
# Install swig .i files for development
########################################################################
install(
    FILES
    radar_swig.i
    #${CMAKE_CURRENT_BINARY_DIR}/radar_swig_doc.i # Outcomment swig documentation for python docs
    DESTINATION ${GR_INCLUDE_DIR}/radar/swig
)
