#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon May 26 18:29:57 2014
##################################################

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
        self.samp_rate = samp_rate = 250000
        self.packet_len = packet_len = 2**16
        self.decim_fac = decim_fac = 2**2
        self.wait_to_start = wait_to_start = 0.05
        self.packet_len_red = packet_len_red = packet_len/decim_fac
        self.num_delay_samps = num_delay_samps = 0
        self.min_output_buffer = min_output_buffer = packet_len*2
        self.freq_res = freq_res = samp_rate/packet_len
        self.center_freq = center_freq = 5800000000

        ##################################################
        # Blocks
        ##################################################
        self._num_delay_samps_layout = Qt.QVBoxLayout()
        self._num_delay_samps_tool_bar = Qt.QToolBar(self)
        self._num_delay_samps_layout.addWidget(self._num_delay_samps_tool_bar)
        self._num_delay_samps_tool_bar.addWidget(Qt.QLabel("num_delay_samps"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._num_delay_samps_counter = qwt_counter_pyslot()
        self._num_delay_samps_counter.setRange(0, 1000, 1)
        self._num_delay_samps_counter.setNumButtons(2)
        self._num_delay_samps_counter.setValue(self.num_delay_samps)
        self._num_delay_samps_tool_bar.addWidget(self._num_delay_samps_counter)
        self._num_delay_samps_counter.valueChanged.connect(self.set_num_delay_samps)
        self._num_delay_samps_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._num_delay_samps_slider.setRange(0, 1000, 1)
        self._num_delay_samps_slider.setValue(self.num_delay_samps)
        self._num_delay_samps_slider.setMinimumWidth(200)
        self._num_delay_samps_slider.valueChanged.connect(self.set_num_delay_samps)
        self._num_delay_samps_layout.addWidget(self._num_delay_samps_slider)
        self.top_layout.addLayout(self._num_delay_samps_layout)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decim_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(samp_rate, center_freq, int(num_delay_samps), 'addr=192.168.10.6', '', 'internal', 'none', 'J1', 0.1, wait_to_start, 0, 'addr=192.168.10.4', '', 'mimo', 'mimo', 'J1', 0.1, wait_to_start, samp_rate/2, "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(131072)
        self.radar_ts_fft_cc_0 = radar.ts_fft_cc(0,  "packet_len")
        (self.radar_ts_fft_cc_0).set_min_output_buffer(131072)
        self.radar_signal_generator_cw_c_0 = radar.signal_generator_cw_c(packet_len, samp_rate, (10000, ), 0.5, "packet_len")
        (self.radar_signal_generator_cw_c_0).set_min_output_buffer(131072)
        self.radar_print_peaks_0 = radar.print_peaks()
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate/decim_fac, 15, 0, 0.78, 15, True, "packet_len")
        (self.radar_os_cfar_c_0).set_min_output_buffer(131072)
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	packet_len/decim_fac, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac, #bw
        	"QT GUI Plot", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        
        
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/decim_fac)
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(131072)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.radar_ts_fft_cc_0, 0), (self.radar_os_cfar_c_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_ts_fft_cc_0, 0))
        self.connect((self.radar_signal_generator_cw_c_0, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.rational_resampler_xxx_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "Msg out", self.radar_print_peaks_0, "Msg in")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_freq_res(self.samp_rate/self.packet_len)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.set_min_output_buffer(self.packet_len*2)
        self.set_packet_len_red(self.packet_len/self.decim_fac)
        self.set_freq_res(self.samp_rate/self.packet_len)

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.set_packet_len_red(self.packet_len/self.decim_fac)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/self.decim_fac)

    def get_wait_to_start(self):
        return self.wait_to_start

    def set_wait_to_start(self, wait_to_start):
        self.wait_to_start = wait_to_start

    def get_packet_len_red(self):
        return self.packet_len_red

    def set_packet_len_red(self, packet_len_red):
        self.packet_len_red = packet_len_red

    def get_num_delay_samps(self):
        return self.num_delay_samps

    def set_num_delay_samps(self, num_delay_samps):
        self.num_delay_samps = num_delay_samps
        Qt.QMetaObject.invokeMethod(self._num_delay_samps_counter, "setValue", Qt.Q_ARG("double", self.num_delay_samps))
        Qt.QMetaObject.invokeMethod(self._num_delay_samps_slider, "setValue", Qt.Q_ARG("double", self.num_delay_samps))
        self.radar_usrp_echotimer_cc_0.set_num_delay_samps(int(self.num_delay_samps))

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_freq_res(self):
        return self.freq_res

    def set_freq_res(self, freq_res):
        self.freq_res = freq_res

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

