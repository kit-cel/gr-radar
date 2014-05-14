#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Wed May 14 16:35:47 2014
##################################################

execfile("/home/stefan/.grc_gnuradio/ts_fft_py_cc.py")
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
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
        self.packet_len = packet_len = 2**17
        self.samp_rate = samp_rate = 1000000
        self.min_output_buffer = min_output_buffer = int(packet_len*2)
        self.max_output_buffer = max_output_buffer = 0
        self.decimator_fac = decimator_fac = 2**8
        self.center_freq = center_freq = 5.7e9

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate/float(decimator_fac),
        	fft_size=packet_len/decimator_fac,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.ts_fft_py_cc_0 = ts_fft_py_cc(
            samp_rate=samp_rate,
            packet_len=packet_len/decimator_fac,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decimator_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc(((10, 15, 20, 15)), ((20, 15, 10, -15)), ((1e9, 1e9, 1e9, 1e9)), ((0, 0, 0, 0)), samp_rate, center_freq, -10, True, True, "packet_len")
        (self.radar_static_target_simulator_cc_0).set_min_output_buffer(262144)
        self.radar_signal_generator_cw_c_0 = radar.signal_generator_cw_c(packet_len, samp_rate, ((0), ), 1, "packet_len")
        (self.radar_signal_generator_cw_c_0).set_min_output_buffer(262144)
        self.radar_print_results_0 = radar.print_results()
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate/decimator_fac, 15, 0, 0.78, 10, True, "packet_len")
        (self.radar_os_cfar_c_0).set_min_output_buffer(262144)
        self.radar_estimator_cw_0 = radar.estimator_cw(center_freq)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(262144)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1/float(decimator_fac))
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        (self.blocks_multiply_xx_0).set_min_output_buffer(262144)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        (self.blocks_add_xx_0).set_min_output_buffer(262144)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.5, 0)
        (self.analog_noise_source_x_0).set_min_output_buffer(262144)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.radar_signal_generator_cw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.ts_fft_py_cc_0, 0), (self.radar_os_cfar_c_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_estimator_cw_0, "Msg in")
        self.msg_connect(self.radar_estimator_cw_0, "Msg out", self.radar_print_results_0, "Msg in")

# QT sink close method reimplementation

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.set_min_output_buffer(int(self.packet_len*2))
        self.ts_fft_py_cc_0.set_packet_len(self.packet_len/self.decimator_fac)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate/float(self.decimator_fac))
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate)

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_max_output_buffer(self):
        return self.max_output_buffer

    def set_max_output_buffer(self, max_output_buffer):
        self.max_output_buffer = max_output_buffer

    def get_decimator_fac(self):
        return self.decimator_fac

    def set_decimator_fac(self, decimator_fac):
        self.decimator_fac = decimator_fac
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate/float(self.decimator_fac))
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1/float(self.decimator_fac))
        self.ts_fft_py_cc_0.set_packet_len(self.packet_len/self.decimator_fac)

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

