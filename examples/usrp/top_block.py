#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Jun 10 21:02:22 2014
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
        self.samp_up = samp_up = 2**14
        self.samp_rate = samp_rate = 10000000
        self.sweep_freq = sweep_freq = samp_rate/2
        self.samp_down = samp_down = samp_up
        self.samp_cw = samp_cw = 2**14
        self.center_freq = center_freq = 5.9e9
        self.wait_to_start = wait_to_start = 0.02
        self.v_res = v_res = samp_rate/samp_cw*3e8/2/center_freq
        self.tx_gain = tx_gain = 10
        self.threshold = threshold = -150
        self.rx_gain = rx_gain = 30
        self.range_res = range_res = 3e8/2/sweep_freq
        self.protect_samp = protect_samp = 0
        self.min_output_buffer = min_output_buffer = int((samp_up+samp_down+samp_cw)*2)
        self.meas_duration = meas_duration = (samp_cw+samp_up+samp_down)/float(samp_rate)
        self.max_output_buffer = max_output_buffer = 0
        self.freq_res_up = freq_res_up = samp_rate/samp_up
        self.freq_res_cw = freq_res_cw = samp_rate/samp_cw
        self.delay_samp = delay_samp = 24
        self.decim_fac = decim_fac = 2**3

        ##################################################
        # Blocks
        ##################################################
        self._tx_gain_layout = Qt.QVBoxLayout()
        self._tx_gain_tool_bar = Qt.QToolBar(self)
        self._tx_gain_layout.addWidget(self._tx_gain_tool_bar)
        self._tx_gain_tool_bar.addWidget(Qt.QLabel("TX Gain"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._tx_gain_counter = qwt_counter_pyslot()
        self._tx_gain_counter.setRange(0, 100, 1)
        self._tx_gain_counter.setNumButtons(2)
        self._tx_gain_counter.setValue(self.tx_gain)
        self._tx_gain_tool_bar.addWidget(self._tx_gain_counter)
        self._tx_gain_counter.valueChanged.connect(self.set_tx_gain)
        self._tx_gain_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._tx_gain_slider.setRange(0, 100, 1)
        self._tx_gain_slider.setValue(self.tx_gain)
        self._tx_gain_slider.setMinimumWidth(200)
        self._tx_gain_slider.valueChanged.connect(self.set_tx_gain)
        self._tx_gain_layout.addWidget(self._tx_gain_slider)
        self.top_grid_layout.addLayout(self._tx_gain_layout, 0,0)
        self._threshold_layout = Qt.QVBoxLayout()
        self._threshold_tool_bar = Qt.QToolBar(self)
        self._threshold_layout.addWidget(self._threshold_tool_bar)
        self._threshold_tool_bar.addWidget(Qt.QLabel("threshold"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._threshold_counter = qwt_counter_pyslot()
        self._threshold_counter.setRange(-150, 0, 1)
        self._threshold_counter.setNumButtons(2)
        self._threshold_counter.setValue(self.threshold)
        self._threshold_tool_bar.addWidget(self._threshold_counter)
        self._threshold_counter.valueChanged.connect(self.set_threshold)
        self._threshold_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._threshold_slider.setRange(-150, 0, 1)
        self._threshold_slider.setValue(self.threshold)
        self._threshold_slider.setMinimumWidth(200)
        self._threshold_slider.valueChanged.connect(self.set_threshold)
        self._threshold_layout.addWidget(self._threshold_slider)
        self.top_grid_layout.addLayout(self._threshold_layout, 1,0)
        self._rx_gain_layout = Qt.QVBoxLayout()
        self._rx_gain_tool_bar = Qt.QToolBar(self)
        self._rx_gain_layout.addWidget(self._rx_gain_tool_bar)
        self._rx_gain_tool_bar.addWidget(Qt.QLabel("RX Gain"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._rx_gain_counter = qwt_counter_pyslot()
        self._rx_gain_counter.setRange(0, 100, 1)
        self._rx_gain_counter.setNumButtons(2)
        self._rx_gain_counter.setValue(self.rx_gain)
        self._rx_gain_tool_bar.addWidget(self._rx_gain_counter)
        self._rx_gain_counter.valueChanged.connect(self.set_rx_gain)
        self._rx_gain_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._rx_gain_slider.setRange(0, 100, 1)
        self._rx_gain_slider.setValue(self.rx_gain)
        self._rx_gain_slider.setMinimumWidth(200)
        self._rx_gain_slider.valueChanged.connect(self.set_rx_gain)
        self._rx_gain_layout.addWidget(self._rx_gain_slider)
        self.top_grid_layout.addLayout(self._rx_gain_layout, 0,1)
        self._protect_samp_layout = Qt.QVBoxLayout()
        self._protect_samp_tool_bar = Qt.QToolBar(self)
        self._protect_samp_layout.addWidget(self._protect_samp_tool_bar)
        self._protect_samp_tool_bar.addWidget(Qt.QLabel("protect_samp"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._protect_samp_counter = qwt_counter_pyslot()
        self._protect_samp_counter.setRange(0, 100, 1)
        self._protect_samp_counter.setNumButtons(2)
        self._protect_samp_counter.setValue(self.protect_samp)
        self._protect_samp_tool_bar.addWidget(self._protect_samp_counter)
        self._protect_samp_counter.valueChanged.connect(self.set_protect_samp)
        self._protect_samp_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._protect_samp_slider.setRange(0, 100, 1)
        self._protect_samp_slider.setValue(self.protect_samp)
        self._protect_samp_slider.setMinimumWidth(200)
        self._protect_samp_slider.valueChanged.connect(self.set_protect_samp)
        self._protect_samp_layout.addWidget(self._protect_samp_slider)
        self.top_grid_layout.addLayout(self._protect_samp_layout, 1,1)
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
        self.ts_fft_py_cc_0_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac,
            packet_len=samp_down/decim_fac,
        )
        self.ts_fft_py_cc_0_0 = ts_fft_py_cc(
            samp_rate=samp_rate/decim_fac,
            packet_len=samp_up/decim_fac,
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
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(samp_rate, center_freq, int(delay_samp), 'addr=192.168.10.6', '', 'internal', 'none', 'J1', tx_gain, 0.1, wait_to_start, 0, 'addr=192.168.10.4', '', 'mimo', 'mimo', 'J1', rx_gain, 0.1, wait_to_start, 0, "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0_0 = radar.split_cc(2, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0 = radar.split_cc(1, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0 = radar.split_cc(0, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0).set_min_output_buffer(98304)
        self.radar_signal_generator_fmcw_c_0 = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, -sweep_freq/2, sweep_freq, 1, "packet_len")
        (self.radar_signal_generator_fmcw_c_0).set_min_output_buffer(98304)
        self.radar_print_peaks_0_0_0 = radar.print_peaks()
        self.radar_find_max_peak_c_0_0_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, int(protect_samp), "packet_len")
        (self.radar_find_max_peak_c_0_0_0).set_min_output_buffer(98304)
        self.radar_find_max_peak_c_0_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, int(protect_samp), "packet_len")
        (self.radar_find_max_peak_c_0_0).set_min_output_buffer(98304)
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, int(protect_samp), "packet_len")
        (self.radar_find_max_peak_c_0).set_min_output_buffer(98304)
        self.radar_estimator_fmcw_0 = radar.estimator_fmcw(samp_rate/decim_fac, center_freq, sweep_freq, samp_up/decim_fac, samp_down/decim_fac)
        self.qtgui_sink_x_0_0_0 = qtgui.sink_c(
        	samp_up/decim_fac, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac, #bw
        	"DOWN", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0_0_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_0_0_win = sip.wrapinstance(self.qtgui_sink_x_0_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_0_0_win)
        
        
        self.qtgui_sink_x_0_0 = qtgui.sink_c(
        	samp_up/decim_fac, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate/decim_fac, #bw
        	"UP", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_0_win = sip.wrapinstance(self.qtgui_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_0_win)
        
        
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/decim_fac)
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(98304)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(98304)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0, 0))
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.qtgui_sink_x_0_0, 0))
        self.connect((self.radar_split_cc_0, 0), (self.ts_fft_py_cc_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.ts_fft_py_cc_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.ts_fft_py_cc_0_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.qtgui_sink_x_0_0_0, 0))
        self.connect((self.ts_fft_py_cc_0, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.ts_fft_py_cc_0_0, 0), (self.radar_find_max_peak_c_0_0, 0))
        self.connect((self.ts_fft_py_cc_0_0_0, 0), (self.radar_find_max_peak_c_0_0_0, 0))
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_find_max_peak_c_0_0_0, "Msg out", self.radar_print_peaks_0_0_0, "Msg in")
        self.msg_connect(self.radar_find_max_peak_c_0_0_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in DOWN")
        self.msg_connect(self.radar_find_max_peak_c_0_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in UP")
        self.msg_connect(self.radar_find_max_peak_c_0, "Msg out", self.radar_estimator_fmcw_0, "Msg in CW")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_up(self):
        return self.samp_up

    def set_samp_up(self, samp_up):
        self.samp_up = samp_up
        self.set_freq_res_up(self.samp_rate/self.samp_up)
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))
        self.set_samp_down(self.samp_up)
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_up/self.decim_fac)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_sweep_freq(self.samp_rate/2)
        self.set_freq_res_up(self.samp_rate/self.samp_up)
        self.set_freq_res_cw(self.samp_rate/self.samp_cw)
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.qtgui_sink_x_0_0.set_frequency_range(0, self.samp_rate/self.decim_fac)
        self.qtgui_sink_x_0_0_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

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
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))
        self.ts_fft_py_cc_0_0_0.set_packet_len(self.samp_down/self.decim_fac)

    def get_samp_cw(self):
        return self.samp_cw

    def set_samp_cw(self, samp_cw):
        self.samp_cw = samp_cw
        self.set_freq_res_cw(self.samp_rate/self.samp_cw)
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))
        self.ts_fft_py_cc_0.set_packet_len(self.samp_cw/self.decim_fac)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)

    def get_wait_to_start(self):
        return self.wait_to_start

    def set_wait_to_start(self, wait_to_start):
        self.wait_to_start = wait_to_start

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain
        self.radar_usrp_echotimer_cc_0.set_tx_gain(self.tx_gain)
        Qt.QMetaObject.invokeMethod(self._tx_gain_counter, "setValue", Qt.Q_ARG("double", self.tx_gain))
        Qt.QMetaObject.invokeMethod(self._tx_gain_slider, "setValue", Qt.Q_ARG("double", self.tx_gain))

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        Qt.QMetaObject.invokeMethod(self._threshold_counter, "setValue", Qt.Q_ARG("double", self.threshold))
        Qt.QMetaObject.invokeMethod(self._threshold_slider, "setValue", Qt.Q_ARG("double", self.threshold))
        self.radar_find_max_peak_c_0_0_0.set_threshold(self.threshold)
        self.radar_find_max_peak_c_0.set_threshold(self.threshold)
        self.radar_find_max_peak_c_0_0.set_threshold(self.threshold)

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.radar_usrp_echotimer_cc_0.set_rx_gain(self.rx_gain)
        Qt.QMetaObject.invokeMethod(self._rx_gain_counter, "setValue", Qt.Q_ARG("double", self.rx_gain))
        Qt.QMetaObject.invokeMethod(self._rx_gain_slider, "setValue", Qt.Q_ARG("double", self.rx_gain))

    def get_range_res(self):
        return self.range_res

    def set_range_res(self, range_res):
        self.range_res = range_res

    def get_protect_samp(self):
        return self.protect_samp

    def set_protect_samp(self, protect_samp):
        self.protect_samp = protect_samp
        Qt.QMetaObject.invokeMethod(self._protect_samp_counter, "setValue", Qt.Q_ARG("double", self.protect_samp))
        Qt.QMetaObject.invokeMethod(self._protect_samp_slider, "setValue", Qt.Q_ARG("double", self.protect_samp))
        self.radar_find_max_peak_c_0_0_0.set_samp_protect(int(self.protect_samp))
        self.radar_find_max_peak_c_0.set_samp_protect(int(self.protect_samp))
        self.radar_find_max_peak_c_0_0.set_samp_protect(int(self.protect_samp))

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_meas_duration(self):
        return self.meas_duration

    def set_meas_duration(self, meas_duration):
        self.meas_duration = meas_duration

    def get_max_output_buffer(self):
        return self.max_output_buffer

    def set_max_output_buffer(self, max_output_buffer):
        self.max_output_buffer = max_output_buffer

    def get_freq_res_up(self):
        return self.freq_res_up

    def set_freq_res_up(self, freq_res_up):
        self.freq_res_up = freq_res_up

    def get_freq_res_cw(self):
        return self.freq_res_cw

    def set_freq_res_cw(self, freq_res_cw):
        self.freq_res_cw = freq_res_cw

    def get_delay_samp(self):
        return self.delay_samp

    def set_delay_samp(self, delay_samp):
        self.delay_samp = delay_samp
        Qt.QMetaObject.invokeMethod(self._delay_samp_counter, "setValue", Qt.Q_ARG("double", self.delay_samp))
        Qt.QMetaObject.invokeMethod(self._delay_samp_slider, "setValue", Qt.Q_ARG("double", self.delay_samp))
        self.radar_usrp_echotimer_cc_0.set_num_delay_samps(int(self.delay_samp))

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.ts_fft_py_cc_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0.set_packet_len(self.samp_cw/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0.set_packet_len(self.samp_up/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_samp_rate(self.samp_rate/self.decim_fac)
        self.ts_fft_py_cc_0_0_0.set_packet_len(self.samp_down/self.decim_fac)
        self.qtgui_sink_x_0_0.set_frequency_range(0, self.samp_rate/self.decim_fac)
        self.qtgui_sink_x_0_0_0.set_frequency_range(0, self.samp_rate/self.decim_fac)
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/self.decim_fac)

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

