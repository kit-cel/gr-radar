/* -*- c++ -*- */

#define RADAR_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "radar_swig_doc.i"

%{
#include "radar/signal_generator_cw_c.h"
#include "radar/signal_generator_fmcw_c.h"
#include "radar/split_cc.h"
#include "radar/os_cfar_c.h"
#include "radar/ts_fft_cc.h"
#include "radar/estimator_cw.h"
#include "radar/print_results.h"
#include "radar/static_target_simulator_cc.h"
#include "radar/signal_generator_fsk_c.h"
#include "radar/split_fsk_cc.h"
#include "radar/estimator_fsk.h"
#include "radar/usrp_echotimer_cc.h"
#include "radar/estimator_fmcw.h"
#include "radar/signal_generator_sync_pulse_c.h"
#include "radar/estimator_sync_pulse_c.h"
#include "radar/find_max_peak_c.h"
#include "radar/qtgui_scatter_plot.h"
#include "radar/qtgui_time_plot.h"
#include "radar/tracking_singletarget.h"
#include "radar/msg_gate.h"
#include "radar/msg_manipulator.h"
#include "radar/ofdm_cyclic_prefix_remover_cvc.h"
#include "radar/transpose_matrix_vcvc.h"
#include "radar/qtgui_spectrogram_plot.h"
#include "radar/crop_matrix_vcvc.h"
#include "radar/ofdm_divide_vcvc.h"
#include "radar/os_cfar_2d_vc.h"
#include "radar/estimator_ofdm.h"
#include "radar/estimator_rcs.h"
#include "radar/trigger_command.h"
%}


%include "radar/signal_generator_cw_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, signal_generator_cw_c);
%include "radar/signal_generator_fmcw_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, signal_generator_fmcw_c);


%include "radar/split_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, split_cc);
%include "radar/os_cfar_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, os_cfar_c);
%include "radar/ts_fft_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, ts_fft_cc);
%include "radar/estimator_cw.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_cw);
%include "radar/print_results.h"
GR_SWIG_BLOCK_MAGIC2(radar, print_results);
%include "radar/static_target_simulator_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, static_target_simulator_cc);

%include "radar/signal_generator_fsk_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, signal_generator_fsk_c);
%include "radar/split_fsk_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, split_fsk_cc);
%include "radar/estimator_fsk.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_fsk);
%include "radar/usrp_echotimer_cc.h"
GR_SWIG_BLOCK_MAGIC2(radar, usrp_echotimer_cc);

%include "radar/estimator_fmcw.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_fmcw);
%include "radar/signal_generator_sync_pulse_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, signal_generator_sync_pulse_c);
%include "radar/estimator_sync_pulse_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_sync_pulse_c);
%include "radar/find_max_peak_c.h"
GR_SWIG_BLOCK_MAGIC2(radar, find_max_peak_c);

%include "radar/qtgui_scatter_plot.h"
GR_SWIG_BLOCK_MAGIC2(radar, qtgui_scatter_plot);
%include "radar/qtgui_time_plot.h"
GR_SWIG_BLOCK_MAGIC2(radar, qtgui_time_plot);

%include "radar/tracking_singletarget.h"
GR_SWIG_BLOCK_MAGIC2(radar, tracking_singletarget);
%include "radar/msg_gate.h"
GR_SWIG_BLOCK_MAGIC2(radar, msg_gate);
%include "radar/msg_manipulator.h"
GR_SWIG_BLOCK_MAGIC2(radar, msg_manipulator);
%include "radar/ofdm_cyclic_prefix_remover_cvc.h"
GR_SWIG_BLOCK_MAGIC2(radar, ofdm_cyclic_prefix_remover_cvc);
%include "radar/transpose_matrix_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(radar, transpose_matrix_vcvc);
%include "radar/qtgui_spectrogram_plot.h"
GR_SWIG_BLOCK_MAGIC2(radar, qtgui_spectrogram_plot);
%include "radar/crop_matrix_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(radar, crop_matrix_vcvc);
%include "radar/ofdm_divide_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(radar, ofdm_divide_vcvc);
%include "radar/os_cfar_2d_vc.h"
GR_SWIG_BLOCK_MAGIC2(radar, os_cfar_2d_vc);
%include "radar/estimator_ofdm.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_ofdm);
%include "radar/estimator_rcs.h"
GR_SWIG_BLOCK_MAGIC2(radar, estimator_rcs);
%include "radar/trigger_command.h"
GR_SWIG_BLOCK_MAGIC2(radar, trigger_command);

