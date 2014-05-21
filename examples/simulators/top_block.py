#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Wed May 21 18:44:46 2014
##################################################

execfile("/home/stefan/.grc_gnuradio/ts_fft_py_cc.py")
from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import radar
import sip
import sys

class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
             self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
             pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.samp_up = samp_up = 2**14
        self.samp_rate = samp_rate = 10000000
        self.sweep_freq = sweep_freq = samp_rate/2
        self.samp_down = samp_down = samp_up
        self.samp_cw = samp_cw = 2**14
        self.center_freq = center_freq = 5.9e9
        self.v_res = v_res = samp_rate/samp_cw*3e8/2/center_freq
        self.range_res = range_res = 3e8/2/sweep_freq
        self.mult_threshold = mult_threshold = 30
        self.min_output_buffer = min_output_buffer = int((samp_up+samp_down+samp_cw)*2)
        self.max_output_buffer = max_output_buffer = 0
        self.decim_fac = decim_fac = 2**5
        self.compared_samples = compared_samples = 15

        ##################################################
        # Blocks
        ##################################################
        self.ts_fft_py_cc_0_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac,
            packet_len=samp_cw/decim_fac,
        )
        self.ts_fft_py_cc_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac,
            packet_len=samp_cw/decim_fac,
        )
        self.ts_fft_py_cc_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac,
            packet_len=samp_cw/decim_fac,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decim_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc(((100), ), ((50), ), ((1e16), ), ((0), ), samp_rate, center_freq, -10, True, True, "packet_len")
        (self.radar_static_target_simulator_cc_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0_0 = radar.split_cc(2, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0 = radar.split_cc(1, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0 = radar.split_cc(0, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0).set_min_output_buffer(98304)
        self.radar_signal_generator_fmcw_c_0 = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, sweep_freq/2, sweep_freq, 1, "packet_len")
        (self.radar_signal_generator_fmcw_c_0).set_min_output_buffer(98304)
        self.radar_print_results_0 = radar.print_results()
        self.radar_os_cfar_c_0_0_0 = radar.os_cfar_c(samp_rate/decim_fac, compared_samples, 0, 0.78, mult_threshold, True, "packet_len")
        (self.radar_os_cfar_c_0_0_0).set_min_output_buffer(98304)
        self.radar_os_cfar_c_0_0 = radar.os_cfar_c(samp_rate/decim_fac, compared_samples, 0, 0.78, mult_threshold, True, "packet_len")
        (self.radar_os_cfar_c_0_0).set_min_output_buffer(98304)
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate/decim_fac, compared_samples, 0, 0.78, mult_threshold, True, "packet_len")
        (self.radar_os_cfar_c_0).set_min_output_buffer(98304)
        self.radar_estimator_fmcw_0 = radar.estimator_fmcw(samp_rate/decim_fac, center_freq, sweep_freq, samp_up/decim_fac, samp_down/decim_fac)
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	samp_cw/decim_fac, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac, #bw
        	"CW", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        
        
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(98304)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "", 1.0/decim_fac)
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(98304)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(98304)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        (self.blocks_add_xx_0).set_min_output_buffer(98304)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.1, 0)
        (self.analog_noise_source_x_0).set_min_output_buffer(98304)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_throttle_0, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.radar_split_cc_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0, 0))
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0_0, 0))
        self.connect((self.radar_split_cc_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.ts_fft_py_cc_0, 0), (self.radar_os_cfar_c_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.ts_fft_py_cc_0_0, 0))
        self.connect((self.ts_fft_py_cc_0_0, 0), (self.radar_os_cfar_c_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.ts_fft_py_cc_0_0_0, 0))
        self.connect((self.ts_fft_py_cc_0_0_0, 0), (self.radar_os_cfar_c_0_0_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in CW")
        self.msg_connect(self.radar_os_cfar_c_0_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in UP")
        self.msg_connect(self.radar_os_cfar_c_0_0_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in DOWN")
        self.msg_connect(self.radar_estimator_fmcw_0, "Msg out", self.radar_print_results_0, "Msg in")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_up(self):
        return self.samp_up

    def set_samp_up(self, samp_up):
        self.samp_up = samp_up
        self.set_samp_down(self.samp_up)
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_sweep_freq(self.samp_rate/2)
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_sweep_freq(self):
        return self.sweep_freq

    def set_sweep_freq(self, sweep_freq):
        self.sweep_freq = sweep_freq
        self.set_range_res(3e8/2/self.sweep_freq)

    def get_samp_down(self):
        return self.samp_down

    def set_samp_down(self, samp_down):
        self.samp_down = samp_down
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))

    def get_samp_cw(self):
        return self.samp_cw

    def set_samp_cw(self, samp_cw):
        self.samp_cw = samp_cw
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))
        self.ts_fft_py_cc_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_cw/self.decim_fac)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_range_res(self):
        return self.range_res

    def set_range_res(self, range_res):
        self.range_res = range_res

    def get_mult_threshold(self):
        return self.mult_threshold

    def set_mult_threshold(self, mult_threshold):
        self.mult_threshold = mult_threshold

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_max_output_buffer(self):
        return self.max_output_buffer

    def set_max_output_buffer(self, max_output_buffer):
        self.max_output_buffer = max_output_buffer

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/self.decim_fac)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_compared_samples(self):
        return self.compared_samples

    def set_compared_samples(self, compared_samples):
        self.compared_samples = compared_samples

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
    Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)
    tb = top_block()
    tb.start()
    tb.show()
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None #to clean up Qt widgets

