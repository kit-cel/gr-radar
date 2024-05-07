install(
    FILES simulation/simulator_cw.grc
          simulation/simulator_dual_cw_estimation_rcs.grc
          simulation/simulator_dual_cw.grc
          simulation/simulator_fmcw_rcs.grc
          simulation/simulator_fmcw.grc
          simulation/simulator_fsk_tracking_singletarget.grc
          simulation/simulator_fsk.grc
          simulation/simulator_ofdm.grc
          simulation/simulator_sync_pulse.grc
          tests/test_find_max_peak_c.grc
          tests/test_msg_manipulator.grc
          tests/test_signal_generator_cw_c.grc
          tests/test_signal_generator_fmcw_c.grc
          tests/test_split_cc.grc
          tests/test_static_target_simulator_cc.grc
          tests/test_ts_fft_cc.grc
          tests/test_uhd_sink_source_inbuild.grc
          tests/test_usrp_echotimer_cc.grc
          usrp/usrp_echotimer_cw.grc
          usrp/usrp_echotimer_dual_cw_rcs.grc
          usrp/usrp_echotimer_dual_cw_tracking.grc
          usrp/usrp_echotimer_dual_cw.grc
          usrp/usrp_echotimer_fmcw.grc
          usrp/usrp_echotimer_fsk_tracking_singletarget.grc
          usrp/usrp_echotimer_fsk.grc
          usrp/usrp_echotimer_sync_pulse.grc
    DESTINATION share/gnuradio/examples/radar)
