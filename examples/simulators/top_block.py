#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Jun  2 17:37:16 2014
##################################################

execfile("/home/stefan/.grc_gnuradio/ts_fft_py_cc.py")
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
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
        self.samp_rate = samp_rate = 5000000
        self.blocks_per_tag = blocks_per_tag = 2**17
        self.samp_per_freq = samp_per_freq = 1
        self.freq_res = freq_res = samp_rate/2/blocks_per_tag
        self.delta_freq = delta_freq = samp_rate/4
        self.center_freq = center_freq = 5.7e9
        self.v_res = v_res = freq_res*3e8/2/center_freq
        self.samp_discard = samp_discard = 0
        self.min_output_buffer = min_output_buffer = 2*(blocks_per_tag*samp_per_freq*2)
        self.decimator_fac = decimator_fac = 2**8
        self.R_max = R_max = 3e8/2/delta_freq

        ##################################################
        # Blocks
        ##################################################
        self.ts_fft_py_cc_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate,
            packet_len=blocks_per_tag/decimator_fac,
        )
        self.ts_fft_py_cc_0 = ts_fft_py_cc(
            samp_rate=samp_rate,
            packet_len=blocks_per_tag/decimator_fac,
        )
        self.rational_resampler_xxx_0_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decimator_fac,
                taps=None,
                fractional_bw=None,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decimator_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc(((10, 20, 50)), ((5, 10, 15)), ((1e9, 1e11, 1e12)), ((0,0, 0)), samp_rate, center_freq, -10, True, True, "packet_len")
        (self.radar_static_target_simulator_cc_0).set_min_output_buffer(524288)
        self.radar_split_fsk_cc_0 = radar.split_fsk_cc(samp_per_freq, samp_discard, "packet_len")
        (self.radar_split_fsk_cc_0).set_min_output_buffer(524288)
        self.radar_signal_generator_fsk_c_0 = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, 0, delta_freq, 1, "packet_len")
        (self.radar_signal_generator_fsk_c_0).set_min_output_buffer(524288)
        self.radar_print_peaks_0 = radar.print_peaks()
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate/2/decimator_fac, 5, 0, 0.78, 30, True, "packet_len")
        (self.radar_os_cfar_c_0).set_min_output_buffer(524288)
        self.radar_estimator_msg_gate_0 = radar.estimator_msg_gate(('velocity','range'), (5,23), (13,25), 0)
        self.radar_estimator_fsk_0 = radar.estimator_fsk(center_freq, delta_freq)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(524288)
        self.blocks_tagged_stream_multiply_length_0_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/decimator_fac)
        (self.blocks_tagged_stream_multiply_length_0_0).set_min_output_buffer(524288)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/decimator_fac)
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(524288)
        self.blocks_multiply_conjugate_cc_1 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_1).set_min_output_buffer(524288)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(524288)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        (self.blocks_add_xx_0).set_min_output_buffer(524288)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.5, 0)
        (self.analog_noise_source_x_0).set_min_output_buffer(524288)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.radar_signal_generator_fsk_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.radar_os_cfar_c_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.blocks_tagged_stream_multiply_length_0_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 1), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0_0, 0), (self.ts_fft_py_cc_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_1, 0), (self.radar_split_fsk_cc_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_conjugate_cc_1, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_conjugate_cc_1, 1))
        self.connect((self.ts_fft_py_cc_0_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))
        self.connect((self.ts_fft_py_cc_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_estimator_fsk_0, "Msg in")
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_print_peaks_0, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_estimator_msg_gate_0, "Msg in")

# QT sink close method reimplementation

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_freq_res(self.samp_rate/2/self.blocks_per_tag)
        self.set_delta_freq(self.samp_rate/4)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.radar_static_target_simulator_cc_0.setup_targets(((10, 20, 50)), ((5, 10, 15)), ((1e9, 1e11, 1e12)), ((0,0, 0)), self.samp_rate, self.center_freq, -10, True, True)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate)

    def get_blocks_per_tag(self):
        return self.blocks_per_tag

    def set_blocks_per_tag(self, blocks_per_tag):
        self.blocks_per_tag = blocks_per_tag
        self.set_min_output_buffer(2*(self.blocks_per_tag*self.samp_per_freq*2))
        self.set_freq_res(self.samp_rate/2/self.blocks_per_tag)
        self.ts_fft_py_cc_0_0.set_packet_len(self.blocks_per_tag/self.decimator_fac)
        self.ts_fft_py_cc_0.set_packet_len(self.blocks_per_tag/self.decimator_fac)

    def get_samp_per_freq(self):
        return self.samp_per_freq

    def set_samp_per_freq(self, samp_per_freq):
        self.samp_per_freq = samp_per_freq
        self.set_min_output_buffer(2*(self.blocks_per_tag*self.samp_per_freq*2))

    def get_freq_res(self):
        return self.freq_res

    def set_freq_res(self, freq_res):
        self.freq_res = freq_res
        self.set_v_res(self.freq_res*3e8/2/self.center_freq)

    def get_delta_freq(self):
        return self.delta_freq

    def set_delta_freq(self, delta_freq):
        self.delta_freq = delta_freq
        self.set_R_max(3e8/2/self.delta_freq)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_v_res(self.freq_res*3e8/2/self.center_freq)
        self.radar_static_target_simulator_cc_0.setup_targets(((10, 20, 50)), ((5, 10, 15)), ((1e9, 1e11, 1e12)), ((0,0, 0)), self.samp_rate, self.center_freq, -10, True, True)

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_samp_discard(self):
        return self.samp_discard

    def set_samp_discard(self, samp_discard):
        self.samp_discard = samp_discard

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_decimator_fac(self):
        return self.decimator_fac

    def set_decimator_fac(self, decimator_fac):
        self.decimator_fac = decimator_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/self.decimator_fac)
        self.blocks_tagged_stream_multiply_length_0_0.set_scalar(1.0/self.decimator_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.blocks_per_tag/self.decimator_fac)
        self.ts_fft_py_cc_0.set_packet_len(self.blocks_per_tag/self.decimator_fac)

    def get_R_max(self):
        return self.R_max

    def set_R_max(self, R_max):
        self.R_max = R_max

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

