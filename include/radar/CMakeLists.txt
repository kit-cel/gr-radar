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
# Install public header files
########################################################################
install(FILES
    api.h
    signal_generator_cw_c.h
    signal_generator_fmcw_c.h
    split_cc.h
    os_cfar_c.h
    ts_fft_cc.h
    estimator_cw.h
    print_results.h
    static_target_simulator_cc.h
    signal_generator_fsk_c.h
    split_fsk_cc.h
    estimator_fsk.h
    usrp_echotimer_cc.h
    estimator_fmcw.h
    signal_generator_sync_pulse_c.h
    estimator_sync_pulse_c.h
    find_max_peak_c.h
    qtgui_scatter_plot.h
    qtgui_time_plot.h
    tracking_singletarget.h
    msg_gate.h
    msg_manipulator.h
    ofdm_cyclic_prefix_remover_cvc.h DESTINATION include/radar
)
