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
install(FILES
    radar_signal_generator_cw_c.xml
    radar_signal_generator_fmcw_c.xml
    radar_split_cc.xml
    radar_os_cfar_c.xml
    radar_ts_fft_cc.xml
    radar_estimator_cw.xml
    radar_print_results.xml
    radar_static_target_simulator_cc.xml
    radar_print_peaks.xml
    radar_signal_generator_fsk_c.xml
    radar_split_fsk_cc.xml
    radar_estimator_fsk.xml
    radar_usrp_echotimer_cc.xml
    radar_estimator_msg_gate.xml
    radar_estimator_fmcw.xml
    radar_signal_generator_sync_pulse_c.xml
    radar_estimator_sync_pulse_c.xml
    radar_find_max_peak_c.xml
    radar_qtgui_scatter_plot.xml
    radar_qtgui_time_plot.xml
    radar_tracking_particle_singletarget.xml
    radar_tracking_singletarget.xml DESTINATION share/gnuradio/grc/blocks
)
