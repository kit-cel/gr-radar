# Copyright 2011,2012 Free Software Foundation, Inc.
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
# QT GUI file generation stuff (Q_OBJECT)
########################################################################
set(QTGUI_MOC_HEADERS
	scatter_plot.h
	time_plot.h
	spectrogram_plot.h)
QT4_WRAP_CPP(QTGUI_MOC_OUTFILES ${QTGUI_MOC_HEADERS})

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND radar_sources
	${QTGUI_MOC_OUTFILES}
	scatter_plot.cc
	time_plot.cc
	spectrogram_plot.cc
    signal_generator_cw_c_impl.cc
    signal_generator_fmcw_c_impl.cc
    split_cc_impl.cc
    os_cfar_c_impl.cc
    ts_fft_cc_impl.cc
    estimator_cw_impl.cc
    print_results_impl.cc
    static_target_simulator_cc_impl.cc
    signal_generator_fsk_c_impl.cc
    split_fsk_cc_impl.cc
    estimator_fsk_impl.cc
    usrp_echotimer_cc_impl.cc
    estimator_fmcw_impl.cc
    signal_generator_sync_pulse_c_impl.cc
    estimator_sync_pulse_c_impl.cc
    find_max_peak_c_impl.cc
    qtgui_scatter_plot_impl.cc
    qtgui_time_plot_impl.cc
    tracking_singletarget_impl.cc
    msg_gate_impl.cc
    msg_manipulator_impl.cc
    ofdm_cyclic_prefix_remover_cvc_impl.cc
    transpose_matrix_vcvc_impl.cc
    qtgui_spectrogram_plot_impl.cc
    crop_matrix_vcvc_impl.cc
    ofdm_divide_vcvc_impl.cc
    os_cfar_2d_vc_impl.cc
    estimator_ofdm_impl.cc
    estimator_rcs_impl.cc
    trigger_command_impl.cc
    cross_correlate_vcvc_impl.cc )

set(radar_sources "${radar_sources}" PARENT_SCOPE)
if(NOT radar_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT radar_sources)

add_library(gnuradio-radar SHARED ${radar_sources})
target_link_libraries(gnuradio-radar ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${FFTW3F_LIBRARIES} ${QWT_LIBRARIES} ${QT_LIBRARIES} ${UHD_LIBRARIES})
set_target_properties(gnuradio-radar PROPERTIES DEFINE_SYMBOL "gnuradio_radar_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-radar PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-radar
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_radar_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_radar.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_radar.cc
)

add_executable(test-radar ${test_radar_sources})

target_link_libraries(
  test-radar
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${FFTW3F_LIBRARY_DIRS}
  ${QWT_LIBRARIES}
  ${QT_LIBRARIES}
  ${UHD_LIBRARY_DIRS}
  gnuradio-radar
)

GR_ADD_TEST(test_radar test-radar)
