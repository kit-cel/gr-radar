#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Apr 28 13:42:32 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import radar
import wx

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")

        ##################################################
        # Variables
        ##################################################
        self.samp_up = samp_up = 1024
        self.samp_rate = samp_rate = 4000
        self.samp_down = samp_down = 1024
        self.samp_cw = samp_cw = 1024
        self.center_freq = center_freq = 1e9

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0_0_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=samp_down,
        	fft_rate=100,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0_0_0.win)
        self.wxgui_fftsink2_0_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=samp_up,
        	fft_rate=100,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0_0.win)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=samp_cw,
        	fft_rate=100,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.radar_ts_fft_cc_0 = radar.ts_fft_cc( "packet_len")
        self.radar_split_cc_0_0_0 = radar.split_cc(2, (samp_cw, samp_up, samp_down), "packet_len")
        self.radar_split_cc_0_0 = radar.split_cc(1, (samp_cw, samp_up, samp_down), "packet_len")
        self.radar_split_cc_0 = radar.split_cc(0, (samp_cw, samp_up, samp_down), "packet_len")
        self.radar_signal_generator_fmcw_c_0 = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, 0, 500, 1, "packet_len")
        self.radar_r_v_rcs_simulator_cc_0 = radar.r_v_rcs_simulator_cc(((5,10)), ((15,30)), ((1e8,1e8)), samp_rate, center_freq, 1)
        self.radar_print_results_0 = radar.print_results()
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate, 15, 5, 0.78, 15, True, "packet_len")
        self.radar_estimator_cw_0 = radar.estimator_cw(center_freq)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.3, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.radar_split_cc_0_0_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.radar_split_cc_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.blocks_null_sink_1, 0))
        self.connect((self.radar_split_cc_0, 0), (self.radar_ts_fft_cc_0, 0))
        self.connect((self.radar_ts_fft_cc_0, 0), (self.radar_os_cfar_c_0, 0))
        self.connect((self.radar_split_cc_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.radar_r_v_rcs_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_throttle_0, 0), (self.radar_r_v_rcs_simulator_cc_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.wxgui_fftsink2_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.wxgui_fftsink2_0_0_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.radar_split_cc_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_estimator_cw_0, "Msg in")
        self.msg_connect(self.radar_estimator_cw_0, "Msg out", self.radar_print_results_0, "Msg in")

# QT sink close method reimplementation

    def get_samp_up(self):
        return self.samp_up

    def set_samp_up(self, samp_up):
        self.samp_up = samp_up

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0_0_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_samp_down(self):
        return self.samp_down

    def set_samp_down(self, samp_down):
        self.samp_down = samp_down

    def get_samp_cw(self):
        return self.samp_cw

    def set_samp_cw(self, samp_cw):
        self.samp_cw = samp_cw

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()

