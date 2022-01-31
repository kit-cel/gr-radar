
/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

void bind_crop_matrix_vcvc(py::module&);
void bind_estimator_cw(py::module&);
void bind_estimator_fmcw(py::module&);
void bind_estimator_fsk(py::module&);
void bind_estimator_ofdm(py::module&);
void bind_estimator_rcs(py::module&);
void bind_estimator_sync_pulse_c(py::module&);
void bind_find_max_peak_c(py::module&);
void bind_msg_gate(py::module&);
void bind_msg_manipulator(py::module&);
void bind_ofdm_cyclic_prefix_remover_cvc(py::module&);
void bind_ofdm_divide_vcvc(py::module&);
void bind_os_cfar_2d_vc(py::module&);
void bind_os_cfar_c(py::module&);
void bind_print_results(py::module&);
void bind_qtgui_scatter_plot(py::module&);
void bind_qtgui_spectrogram_plot(py::module&);
void bind_qtgui_time_plot(py::module&);
void bind_signal_generator_cw_c(py::module&);
void bind_signal_generator_fmcw_c(py::module&);
void bind_signal_generator_fsk_c(py::module&);
void bind_signal_generator_sync_pulse_c(py::module&);
void bind_split_cc(py::module&);
void bind_split_fsk_cc(py::module&);
void bind_static_target_simulator_cc(py::module&);
void bind_tracking_singletarget(py::module&);
void bind_transpose_matrix_vcvc(py::module&);
void bind_trigger_command(py::module&);
void bind_ts_fft_cc(py::module&);
void bind_usrp_echotimer_cc(py::module&);

// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(radar_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    bind_crop_matrix_vcvc(m);
    bind_estimator_cw(m);
    bind_estimator_fmcw(m);
    bind_estimator_fsk(m);
    bind_estimator_ofdm(m);
    bind_estimator_rcs(m);
    bind_estimator_sync_pulse_c(m);
    bind_find_max_peak_c(m);
    bind_msg_gate(m);
    bind_msg_manipulator(m);
    bind_ofdm_cyclic_prefix_remover_cvc(m);
    bind_ofdm_divide_vcvc(m);
    bind_os_cfar_2d_vc(m);
    bind_os_cfar_c(m);
    bind_print_results(m);
    bind_qtgui_scatter_plot(m);
    bind_qtgui_spectrogram_plot(m);
    bind_qtgui_time_plot(m);
    bind_signal_generator_cw_c(m);
    bind_signal_generator_fmcw_c(m);
    bind_signal_generator_fsk_c(m);
    bind_signal_generator_sync_pulse_c(m);
    bind_split_cc(m);
    bind_split_fsk_cc(m);
    bind_static_target_simulator_cc(m);
    bind_tracking_singletarget(m);
    bind_transpose_matrix_vcvc(m);
    bind_trigger_command(m);
    bind_ts_fft_cc(m);
    bind_usrp_echotimer_cc(m);
}
