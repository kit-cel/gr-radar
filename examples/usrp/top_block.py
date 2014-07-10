#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Thu Jul 10 18:59:01 2014
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
        self.samp_rate = samp_rate = 20000000
        self.packet_len = packet_len = 2**22
        self.freq_res = freq_res = samp_rate/float(packet_len)
        self.freq = freq = (-6000000,6000000)
        self.center_freq = center_freq = 2.45e9
        self.v_res = v_res = freq_res*3e8/2/center_freq
        self.time_res = time_res = packet_len/float(samp_rate)
        self.threshold = threshold = -35
        self.samp_protect = samp_protect = 1
        self.range_time = range_time = 30
        self.range_res = range_res = 3e8/2/float((freq[1]-freq[0]))
        self.min_output_buffer = min_output_buffer = int(packet_len*2)
        self.max_output_buffer = max_output_buffer = 0
        self.gain_tx = gain_tx = 10
        self.gain_rx = gain_rx = 10
        self.decim_fac = decim_fac = 2**12
        self.amplitude = amplitude = 0.5

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
        self.top_grid_layout.addLayout(self._threshold_layout, 1,0)
        self._samp_protect_layout = Qt.QVBoxLayout()
        self._samp_protect_tool_bar = Qt.QToolBar(self)
        self._samp_protect_layout.addWidget(self._samp_protect_tool_bar)
        self._samp_protect_tool_bar.addWidget(Qt.QLabel("Find peak protected samples"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._samp_protect_counter = qwt_counter_pyslot()
        self._samp_protect_counter.setRange(0, 100, 1)
        self._samp_protect_counter.setNumButtons(2)
        self._samp_protect_counter.setValue(self.samp_protect)
        self._samp_protect_tool_bar.addWidget(self._samp_protect_counter)
        self._samp_protect_counter.valueChanged.connect(self.set_samp_protect)
        self._samp_protect_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._samp_protect_slider.setRange(0, 100, 1)
        self._samp_protect_slider.setValue(self.samp_protect)
        self._samp_protect_slider.setMinimumWidth(200)
        self._samp_protect_slider.valueChanged.connect(self.set_samp_protect)
        self._samp_protect_layout.addWidget(self._samp_protect_slider)
        self.top_grid_layout.addLayout(self._samp_protect_layout, 1,1)
        self._gain_tx_layout = Qt.QVBoxLayout()
        self._gain_tx_tool_bar = Qt.QToolBar(self)
        self._gain_tx_layout.addWidget(self._gain_tx_tool_bar)
        self._gain_tx_tool_bar.addWidget(Qt.QLabel("TX gain"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._gain_tx_counter = qwt_counter_pyslot()
        self._gain_tx_counter.setRange(0, 100, 1)
        self._gain_tx_counter.setNumButtons(2)
        self._gain_tx_counter.setValue(self.gain_tx)
        self._gain_tx_tool_bar.addWidget(self._gain_tx_counter)
        self._gain_tx_counter.valueChanged.connect(self.set_gain_tx)
        self._gain_tx_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._gain_tx_slider.setRange(0, 100, 1)
        self._gain_tx_slider.setValue(self.gain_tx)
        self._gain_tx_slider.setMinimumWidth(200)
        self._gain_tx_slider.valueChanged.connect(self.set_gain_tx)
        self._gain_tx_layout.addWidget(self._gain_tx_slider)
        self.top_grid_layout.addLayout(self._gain_tx_layout, 0,0)
        self._gain_rx_layout = Qt.QVBoxLayout()
        self._gain_rx_tool_bar = Qt.QToolBar(self)
        self._gain_rx_layout.addWidget(self._gain_rx_tool_bar)
        self._gain_rx_tool_bar.addWidget(Qt.QLabel("RX gain"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._gain_rx_counter = qwt_counter_pyslot()
        self._gain_rx_counter.setRange(0, 100, 1)
        self._gain_rx_counter.setNumButtons(2)
        self._gain_rx_counter.setValue(self.gain_rx)
        self._gain_rx_tool_bar.addWidget(self._gain_rx_counter)
        self._gain_rx_counter.valueChanged.connect(self.set_gain_rx)
        self._gain_rx_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._gain_rx_slider.setRange(0, 100, 1)
        self._gain_rx_slider.setValue(self.gain_rx)
        self._gain_rx_slider.setMinimumWidth(200)
        self._gain_rx_slider.valueChanged.connect(self.set_gain_rx)
        self._gain_rx_layout.addWidget(self._gain_rx_slider)
        self.top_grid_layout.addLayout(self._gain_rx_layout, 0,1)
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
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(1, samp_rate, center_freq, (24,), 'addr=192.168.10.6', '', 'internal', 'none', 'J1', gain_tx, 0.1, 0.05, 0, ('addr=192.168.10.4',), ('',), ('mimo',), ('mimo',), ('J1',), (gain_rx,), (0.1,), (0.05,), (0,), "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(8388608)
        self.radar_ts_fft_cc_0_0 = radar.ts_fft_cc(packet_len/decim_fac,  "packet_len")
        (self.radar_ts_fft_cc_0_0).set_min_output_buffer(8388608)
        self.radar_ts_fft_cc_0 = radar.ts_fft_cc(packet_len/decim_fac,  "packet_len")
        (self.radar_ts_fft_cc_0).set_min_output_buffer(8388608)
        self.radar_signal_generator_cw_c_0_0 = radar.signal_generator_cw_c(packet_len, samp_rate, (freq[1], ), amplitude, "packet_len")
        (self.radar_signal_generator_cw_c_0_0).set_min_output_buffer(8388608)
        self.radar_signal_generator_cw_c_0 = radar.signal_generator_cw_c(packet_len, samp_rate, (freq[0], ), amplitude, "packet_len")
        (self.radar_signal_generator_cw_c_0).set_min_output_buffer(8388608)
        self.radar_qtgui_time_plot_1 = radar.qtgui_time_plot(100, 'velocity', (-3,3), range_time, "")
        self.radar_qtgui_time_plot_0 = radar.qtgui_time_plot(100, 'range', (0,range_res), range_time, "")
        self.radar_print_results_0 = radar.print_results(False, "")
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, int(samp_protect), ((-300,300)), True, "packet_len")
        self.radar_estimator_fsk_0 = radar.estimator_fsk(center_freq, (freq[1]-freq[0]))
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	packet_len/decim_fac, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac, #bw
        	"QT GUI Plot", #name
        	2 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(2):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0_0).set_min_output_buffer(8388608)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(8388608)
        self.blocks_tagged_stream_multiply_length_0_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0_0).set_min_output_buffer(8388608)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(8388608)
        self.blocks_multiply_conjugate_cc_1 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_1).set_min_output_buffer(8388608)
        self.blocks_multiply_conjugate_cc_0_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0_0).set_min_output_buffer(8388608)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(8388608)
        self.blocks_add_xx_1 = blocks.add_vcc(1)
        (self.blocks_add_xx_1).set_min_output_buffer(8388608)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_cw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.radar_signal_generator_cw_c_0_0, 0), (self.blocks_throttle_0_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_add_xx_1, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_add_xx_1, 1))
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_multiply_conjugate_cc_0_0, 1))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0_0, 0), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_ts_fft_cc_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0_0, 0), (self.radar_ts_fft_cc_0_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.blocks_tagged_stream_multiply_length_0_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.qtgui_freq_sink_x_0, 1))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))
        self.connect((self.blocks_multiply_conjugate_cc_1, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.blocks_add_xx_1, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_multiply_conjugate_cc_0_0, 0))
        self.connect((self.radar_ts_fft_cc_0_0, 0), (self.blocks_multiply_conjugate_cc_1, 1))
        self.connect((self.radar_ts_fft_cc_0, 0), (self.blocks_multiply_conjugate_cc_1, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_find_max_peak_c_0, "Msg out", self.radar_estimator_fsk_0, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_qtgui_time_plot_0, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_qtgui_time_plot_1, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_print_results_0, "Msg in")

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_freq_res(self.samp_rate/float(self.packet_len))
        self.set_time_res(self.packet_len/float(self.samp_rate))
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.set_min_output_buffer(int(self.packet_len*2))
        self.set_freq_res(self.samp_rate/float(self.packet_len))
        self.set_time_res(self.packet_len/float(self.samp_rate))

    def get_freq_res(self):
        return self.freq_res

    def set_freq_res(self, freq_res):
        self.freq_res = freq_res
        self.set_v_res(self.freq_res*3e8/2/self.center_freq)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.set_range_res(3e8/2/float((self.freq[1]-self.freq[0])))

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_v_res(self.freq_res*3e8/2/self.center_freq)

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_time_res(self):
        return self.time_res

    def set_time_res(self, time_res):
        self.time_res = time_res

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        self.radar_find_max_peak_c_0.set_threshold(self.threshold)
        Qt.QMetaObject.invokeMethod(self._threshold_counter, "setValue", Qt.Q_ARG("double", self.threshold))
        Qt.QMetaObject.invokeMethod(self._threshold_slider, "setValue", Qt.Q_ARG("double", self.threshold))

    def get_samp_protect(self):
        return self.samp_protect

    def set_samp_protect(self, samp_protect):
        self.samp_protect = samp_protect
        Qt.QMetaObject.invokeMethod(self._samp_protect_counter, "setValue", Qt.Q_ARG("double", self.samp_protect))
        Qt.QMetaObject.invokeMethod(self._samp_protect_slider, "setValue", Qt.Q_ARG("double", self.samp_protect))
        self.radar_find_max_peak_c_0.set_samp_protect(int(self.samp_protect))

    def get_range_time(self):
        return self.range_time

    def set_range_time(self, range_time):
        self.range_time = range_time

    def get_range_res(self):
        return self.range_res

    def set_range_res(self, range_res):
        self.range_res = range_res

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_max_output_buffer(self):
        return self.max_output_buffer

    def set_max_output_buffer(self, max_output_buffer):
        self.max_output_buffer = max_output_buffer

    def get_gain_tx(self):
        return self.gain_tx

    def set_gain_tx(self, gain_tx):
        self.gain_tx = gain_tx
        Qt.QMetaObject.invokeMethod(self._gain_tx_counter, "setValue", Qt.Q_ARG("double", self.gain_tx))
        Qt.QMetaObject.invokeMethod(self._gain_tx_slider, "setValue", Qt.Q_ARG("double", self.gain_tx))
        self.radar_usrp_echotimer_cc_0.set_tx_gain(self.gain_tx)

    def get_gain_rx(self):
        return self.gain_rx

    def set_gain_rx(self, gain_rx):
        self.gain_rx = gain_rx
        Qt.QMetaObject.invokeMethod(self._gain_rx_counter, "setValue", Qt.Q_ARG("double", self.gain_rx))
        Qt.QMetaObject.invokeMethod(self._gain_rx_slider, "setValue", Qt.Q_ARG("double", self.gain_rx))
        self.radar_usrp_echotimer_cc_0.set_rx_gain((self.gain_rx,))

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/float(self.decim_fac))
        self.blocks_tagged_stream_multiply_length_0_0.set_scalar(1.0/float(self.decim_fac))
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_amplitude(self):
        return self.amplitude

    def set_amplitude(self, amplitude):
        self.amplitude = amplitude

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

