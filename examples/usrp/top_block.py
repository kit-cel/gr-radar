#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Fri Jun 13 18:10:23 2014
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
        self.samp_rate = samp_rate = 2000000
        self.samp_per_freq = samp_per_freq = 2
        self.decim_fac = decim_fac = 2**9
        self.block_per_tag = block_per_tag = 2**17
        self.samp_rate_red = samp_rate_red = samp_rate/2/samp_per_freq/decim_fac
        self.packet_len_red = packet_len_red = block_per_tag/decim_fac
        self.freq_res = freq_res = samp_rate_red/packet_len_red
        self.delta_freq = delta_freq = 1.25e6
        self.center_freq = center_freq = 2400000000
        self.wait_to_start = wait_to_start = 0.02
        self.vel_res = vel_res = freq_res/2.0/center_freq*3e8
        self.threshold = threshold = -200
        self.samp_protect = samp_protect = 3
        self.range_res = range_res = 3e8/2/delta_freq
        self.min_output_buffer = min_output_buffer = 2*samp_per_freq*block_per_tag*2
        self.measure_time = measure_time = (block_per_tag*samp_per_freq*2)/float(samp_rate)
        self.gain_tx = gain_tx = 10
        self.gain_rx = gain_rx = 10
        self.delay_samp = delay_samp = 24

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
        self._delay_samp_layout = Qt.QVBoxLayout()
        self._delay_samp_tool_bar = Qt.QToolBar(self)
        self._delay_samp_layout.addWidget(self._delay_samp_tool_bar)
        self._delay_samp_tool_bar.addWidget(Qt.QLabel("Number delay samples"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._delay_samp_counter = qwt_counter_pyslot()
        self._delay_samp_counter.setRange(0, 100, 1)
        self._delay_samp_counter.setNumButtons(2)
        self._delay_samp_counter.setValue(self.delay_samp)
        self._delay_samp_tool_bar.addWidget(self._delay_samp_counter)
        self._delay_samp_counter.valueChanged.connect(self.set_delay_samp)
        self._delay_samp_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._delay_samp_slider.setRange(0, 100, 1)
        self._delay_samp_slider.setValue(self.delay_samp)
        self._delay_samp_slider.setMinimumWidth(200)
        self._delay_samp_slider.valueChanged.connect(self.set_delay_samp)
        self._delay_samp_layout.addWidget(self._delay_samp_slider)
        self.top_layout.addLayout(self._delay_samp_layout)
        self.ts_fft_py_cc_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate_red,
            packet_len=block_per_tag/decim_fac,
        )
        self.ts_fft_py_cc_0 = ts_fft_py_cc(
            samp_rate=samp_rate_red,
            packet_len=packet_len_red,
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
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(samp_rate, center_freq, int(delay_samp), 'addr=192.168.10.6', '', 'internal', 'none', 'J1', gain_tx, 0.1, wait_to_start, 0, 'addr=192.168.10.4', '', 'mimo', 'mimo', 'J1', gain_rx, 0.1, wait_to_start, 0, "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(1048576)
        self.radar_split_fsk_cc_0 = radar.split_fsk_cc(samp_per_freq, samp_per_freq-1, "packet_len")
        (self.radar_split_fsk_cc_0).set_min_output_buffer(524288)
        self.radar_signal_generator_fsk_c_0 = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, block_per_tag, -delta_freq/2, delta_freq/2, 0.5, "packet_len")
        (self.radar_signal_generator_fsk_c_0).set_min_output_buffer(1048576)
        self.radar_print_results_0 = radar.print_results()
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate_red, threshold, int(samp_protect), "packet_len")
        self.radar_estimator_msg_gate_0 = radar.estimator_msg_gate(('range',), (0, ), (20, ), 0)
        self.radar_estimator_fsk_0 = radar.estimator_fsk(center_freq, delta_freq)
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	packet_len_red, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate_red, #bw
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
        (self.blocks_multiply_conjugate_cc_1).set_min_output_buffer(512)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(1048576)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_fsk_c_0, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_1, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.blocks_tagged_stream_multiply_length_0_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.radar_split_fsk_cc_0, 1), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.radar_split_fsk_cc_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0_0, 0), (self.ts_fft_py_cc_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.radar_signal_generator_fsk_c_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.ts_fft_py_cc_0_0, 0), (self.blocks_multiply_conjugate_cc_1, 1))
        self.connect((self.ts_fft_py_cc_0, 0), (self.blocks_multiply_conjugate_cc_1, 0))

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
        self.set_measure_time((self.block_per_tag*self.samp_per_freq*2)/float(self.samp_rate))
        self.set_samp_rate_red(self.samp_rate/2/self.samp_per_freq/self.decim_fac)

    def get_samp_per_freq(self):
        return self.samp_per_freq

    def set_samp_per_freq(self, samp_per_freq):
        self.samp_per_freq = samp_per_freq
        self.set_measure_time((self.block_per_tag*self.samp_per_freq*2)/float(self.samp_rate))
        self.set_min_output_buffer(2*self.samp_per_freq*self.block_per_tag*2)
        self.set_samp_rate_red(self.samp_rate/2/self.samp_per_freq/self.decim_fac)

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.set_packet_len_red(self.block_per_tag/self.decim_fac)
        self.set_samp_rate_red(self.samp_rate/2/self.samp_per_freq/self.decim_fac)
        self.blocks_tagged_stream_multiply_length_0_0.set_scalar(1/float(self.decim_fac))
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1/float(self.decim_fac))
        self.ts_fft_py_cc_0_0.set_packet_len(self.block_per_tag/self.decim_fac)

    def get_block_per_tag(self):
        return self.block_per_tag

    def set_block_per_tag(self, block_per_tag):
        self.block_per_tag = block_per_tag
        self.set_measure_time((self.block_per_tag*self.samp_per_freq*2)/float(self.samp_rate))
        self.set_min_output_buffer(2*self.samp_per_freq*self.block_per_tag*2)
        self.set_packet_len_red(self.block_per_tag/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.block_per_tag/self.decim_fac)

    def get_samp_rate_red(self):
        return self.samp_rate_red

    def set_samp_rate_red(self, samp_rate_red):
        self.samp_rate_red = samp_rate_red
        self.set_freq_res(self.samp_rate_red/self.packet_len_red)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate_red)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate_red)
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate_red)

    def get_packet_len_red(self):
        return self.packet_len_red

    def set_packet_len_red(self, packet_len_red):
        self.packet_len_red = packet_len_red
        self.set_freq_res(self.samp_rate_red/self.packet_len_red)
        self.ts_fft_py_cc_0.set_packet_len(self.packet_len_red)

    def get_freq_res(self):
        return self.freq_res

    def set_freq_res(self, freq_res):
        self.freq_res = freq_res
        self.set_vel_res(self.freq_res/2.0/self.center_freq*3e8)

    def get_delta_freq(self):
        return self.delta_freq

    def set_delta_freq(self, delta_freq):
        self.delta_freq = delta_freq
        self.set_range_res(3e8/2/self.delta_freq)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_vel_res(self.freq_res/2.0/self.center_freq*3e8)

    def get_wait_to_start(self):
        return self.wait_to_start

    def set_wait_to_start(self, wait_to_start):
        self.wait_to_start = wait_to_start

    def get_vel_res(self):
        return self.vel_res

    def set_vel_res(self, vel_res):
        self.vel_res = vel_res

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
        self.radar_find_max_peak_c_0.set_samp_protect(int(self.samp_protect))
        Qt.QMetaObject.invokeMethod(self._samp_protect_counter, "setValue", Qt.Q_ARG("double", self.samp_protect))
        Qt.QMetaObject.invokeMethod(self._samp_protect_slider, "setValue", Qt.Q_ARG("double", self.samp_protect))

    def get_range_res(self):
        return self.range_res

    def set_range_res(self, range_res):
        self.range_res = range_res

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_measure_time(self):
        return self.measure_time

    def set_measure_time(self, measure_time):
        self.measure_time = measure_time

    def get_gain_tx(self):
        return self.gain_tx

    def set_gain_tx(self, gain_tx):
        self.gain_tx = gain_tx
        self.radar_usrp_echotimer_cc_0.set_tx_gain(self.gain_tx)
        Qt.QMetaObject.invokeMethod(self._gain_tx_counter, "setValue", Qt.Q_ARG("double", self.gain_tx))
        Qt.QMetaObject.invokeMethod(self._gain_tx_slider, "setValue", Qt.Q_ARG("double", self.gain_tx))

    def get_gain_rx(self):
        return self.gain_rx

    def set_gain_rx(self, gain_rx):
        self.gain_rx = gain_rx
        self.radar_usrp_echotimer_cc_0.set_rx_gain(self.gain_rx)
        Qt.QMetaObject.invokeMethod(self._gain_rx_counter, "setValue", Qt.Q_ARG("double", self.gain_rx))
        Qt.QMetaObject.invokeMethod(self._gain_rx_slider, "setValue", Qt.Q_ARG("double", self.gain_rx))

    def get_delay_samp(self):
        return self.delay_samp

    def set_delay_samp(self, delay_samp):
        self.delay_samp = delay_samp
        Qt.QMetaObject.invokeMethod(self._delay_samp_counter, "setValue", Qt.Q_ARG("double", self.delay_samp))
        Qt.QMetaObject.invokeMethod(self._delay_samp_slider, "setValue", Qt.Q_ARG("double", self.delay_samp))
        self.radar_usrp_echotimer_cc_0.set_num_delay_samps(int(self.delay_samp))

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

