#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Jun  3 17:15:17 2014
##################################################

execfile("/home/stefan/.grc_gnuradio/ts_fft_py_cc.py")
from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
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
        self.samp_rate = samp_rate = 5000000
        self.samp_per_freq = samp_per_freq = 1
        self.delta_freq = delta_freq = 1.25e6
        self.block_per_tag = block_per_tag = 2**18
        self.wait_to_start = wait_to_start = 0.02
        self.threshold = threshold = -200
        self.range_res = range_res = 3e8/2/delta_freq
        self.min_output_buffer = min_output_buffer = 2*samp_per_freq*block_per_tag*2
        self.freq_res = freq_res = samp_rate/block_per_tag/samp_per_freq
        self.decim_fac = decim_fac = 2**10
        self.center_freq = center_freq = 2400000000

        ##################################################
        # Blocks
        ##################################################
        self._threshold_layout = Qt.QVBoxLayout()
        self._threshold_tool_bar = Qt.QToolBar(self)
        self._threshold_layout.addWidget(self._threshold_tool_bar)
        self._threshold_tool_bar.addWidget(Qt.QLabel("Find peak threshold"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._threshold_counter = qwt_counter_pyslot()
        self._threshold_counter.setRange(-200, 100, 1)
        self._threshold_counter.setNumButtons(2)
        self._threshold_counter.setValue(self.threshold)
        self._threshold_tool_bar.addWidget(self._threshold_counter)
        self._threshold_counter.valueChanged.connect(self.set_threshold)
        self._threshold_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._threshold_slider.setRange(-200, 100, 1)
        self._threshold_slider.setValue(self.threshold)
        self._threshold_slider.setMinimumWidth(200)
        self._threshold_slider.valueChanged.connect(self.set_threshold)
        self._threshold_layout.addWidget(self._threshold_slider)
        self.top_layout.addLayout(self._threshold_layout)
        self.ts_fft_py_cc_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac/2,
            packet_len=samp_per_freq*block_per_tag/decim_fac,
        )
        self.ts_fft_py_cc_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac/2,
            packet_len=samp_per_freq*block_per_tag/decim_fac,
        )
        self.rational_resampler_xxx_0_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decim_fac,
                taps=None,
                fractional_bw=None,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decim_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(samp_rate, center_freq, 24, 'addr=192.168.10.6', '', 'internal', 'none', 'J1', 10, 0.1, wait_to_start, 0, 'addr=192.168.10.4', '', 'mimo', 'mimo', 'J1', 30, 0.1, wait_to_start, 0, "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(1048576)
        self.radar_split_fsk_cc_0 = radar.split_fsk_cc(samp_per_freq, 0, "packet_len")
        (self.radar_split_fsk_cc_0).set_min_output_buffer(524288)
        self.radar_signal_generator_fsk_c_0 = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, block_per_tag, 0, delta_freq, 0.5, "packet_len")
        (self.radar_signal_generator_fsk_c_0).set_min_output_buffer(1048576)
        self.radar_print_results_0 = radar.print_results()
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate/decim_fac/2, threshold, "packet_len")
        self.radar_estimator_msg_gate_0 = radar.estimator_msg_gate(('velocity',), (0.1, ), (20, ), 0)
        self.radar_estimator_fsk_0 = radar.estimator_fsk(center_freq, delta_freq)
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	samp_per_freq*block_per_tag/decim_fac, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac/2, #bw
        	"QT GUI Plot", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        
        
        self.blocks_tagged_stream_multiply_length_0_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0_0).set_min_output_buffer(512)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(512)
        self.blocks_multiply_conjugate_cc_1 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_1).set_min_output_buffer(1024)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(1048576)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_fsk_c_0, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_1, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.ts_fft_py_cc_0, 0), (self.blocks_multiply_conjugate_cc_1, 1))
        self.connect((self.ts_fft_py_cc_0_0, 0), (self.blocks_multiply_conjugate_cc_1, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0_0, 0), (self.ts_fft_py_cc_0_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.blocks_tagged_stream_multiply_length_0_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 1), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.radar_split_fsk_cc_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.radar_signal_generator_fsk_c_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_find_max_peak_c_0, "Msg out", self.radar_estimator_fsk_0, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_estimator_msg_gate_0, "Msg in")
        self.msg_connect(self.radar_estimator_msg_gate_0, "Msg out", self.radar_print_results_0, "Msg in")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_freq_res(self.samp_rate/self.block_per_tag/self.samp_per_freq)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac/2)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac/2)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac/2)

    def get_samp_per_freq(self):
        return self.samp_per_freq

    def set_samp_per_freq(self, samp_per_freq):
        self.samp_per_freq = samp_per_freq
        self.set_min_output_buffer(2*self.samp_per_freq*self.block_per_tag*2)
        self.set_freq_res(self.samp_rate/self.block_per_tag/self.samp_per_freq)
        self.ts_fft_py_cc_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)

    def get_delta_freq(self):
        return self.delta_freq

    def set_delta_freq(self, delta_freq):
        self.delta_freq = delta_freq
        self.set_range_res(3e8/2/self.delta_freq)

    def get_block_per_tag(self):
        return self.block_per_tag

    def set_block_per_tag(self, block_per_tag):
        self.block_per_tag = block_per_tag
        self.set_min_output_buffer(2*self.samp_per_freq*self.block_per_tag*2)
        self.set_freq_res(self.samp_rate/self.block_per_tag/self.samp_per_freq)
        self.ts_fft_py_cc_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)

    def get_wait_to_start(self):
        return self.wait_to_start

    def set_wait_to_start(self, wait_to_start):
        self.wait_to_start = wait_to_start

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        Qt.QMetaObject.invokeMethod(self._threshold_counter, "setValue", Qt.Q_ARG("double", self.threshold))
        Qt.QMetaObject.invokeMethod(self._threshold_slider, "setValue", Qt.Q_ARG("double", self.threshold))
        self.radar_find_max_peak_c_0.set_threshold(self.threshold)

    def get_range_res(self):
        return self.range_res

    def set_range_res(self, range_res):
        self.range_res = range_res

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_freq_res(self):
        return self.freq_res

    def set_freq_res(self, freq_res):
        self.freq_res = freq_res

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1/float(self.decim_fac))
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac/2)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac/2)
        self.ts_fft_py_cc_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac/2)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_per_freq*self.block_per_tag/self.decim_fac)
        self.blocks_tagged_stream_multiply_length_0_0.set_scalar(1/float(self.decim_fac))

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

