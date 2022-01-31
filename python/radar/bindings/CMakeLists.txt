########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND radar_python_files
    crop_matrix_vcvc_python.cc
    estimator_cw_python.cc
    estimator_fmcw_python.cc
    estimator_fsk_python.cc
    estimator_ofdm_python.cc
    estimator_rcs_python.cc
    estimator_sync_pulse_c_python.cc
    find_max_peak_c_python.cc
    msg_gate_python.cc
    msg_manipulator_python.cc
    ofdm_cyclic_prefix_remover_cvc_python.cc
    ofdm_divide_vcvc_python.cc
    os_cfar_2d_vc_python.cc
    os_cfar_c_python.cc
    print_results_python.cc
    qtgui_scatter_plot_python.cc
    qtgui_spectrogram_plot_python.cc
    qtgui_time_plot_python.cc
    signal_generator_cw_c_python.cc
    signal_generator_fmcw_c_python.cc
    signal_generator_fsk_c_python.cc
    signal_generator_sync_pulse_c_python.cc
    split_cc_python.cc
    split_fsk_cc_python.cc
    static_target_simulator_cc_python.cc
    tracking_singletarget_python.cc
    transpose_matrix_vcvc_python.cc
    trigger_command_python.cc
    ts_fft_cc_python.cc
    usrp_echotimer_cc_python.cc
    python_bindings.cc)

GR_PYBIND_MAKE_OOT(radar 
   ../../.. 
   gr::radar
   "${radar_python_files}")

install(TARGETS radar_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/radar COMPONENT pythonapi)
