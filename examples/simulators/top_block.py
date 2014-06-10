#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Wed Jun 11 00:19:16 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import radar
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
        self.velocity = velocity = 50
        self.v_res = v_res = samp_rate/samp_cw*3e8/2/center_freq
        self.threshold = threshold = -120
        self.range_res = range_res = 3e8/2/sweep_freq
        self.protect_samp = protect_samp = 1
        self.min_output_buffer = min_output_buffer = int((samp_up+samp_down+samp_cw)*2)
        self.meas_duration = meas_duration = (samp_cw+samp_up+samp_down)/float(samp_rate)
        self.max_output_buffer = max_output_buffer = 0
        self.decim_fac = decim_fac = 2**6
        self.Range = Range = 200

        ##################################################
        # Blocks
        ##################################################
        self._velocity_layout = Qt.QVBoxLayout()
        self._velocity_tool_bar = Qt.QToolBar(self)
        self._velocity_layout.addWidget(self._velocity_tool_bar)
        self._velocity_tool_bar.addWidget(Qt.QLabel("velocity"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._velocity_counter = qwt_counter_pyslot()
        self._velocity_counter.setRange(0, 100, 1)
        self._velocity_counter.setNumButtons(2)
        self._velocity_counter.setValue(self.velocity)
        self._velocity_tool_bar.addWidget(self._velocity_counter)
        self._velocity_counter.valueChanged.connect(self.set_velocity)
        self._velocity_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._velocity_slider.setRange(0, 100, 1)
        self._velocity_slider.setValue(self.velocity)
        self._velocity_slider.setMinimumWidth(200)
        self._velocity_slider.valueChanged.connect(self.set_velocity)
        self._velocity_layout.addWidget(self._velocity_slider)
        self.top_layout.addLayout(self._velocity_layout)
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
        self._threshold_counter.setRange(-120, 0, 1)
        self._threshold_counter.setNumButtons(2)
        self._threshold_counter.setValue(self.threshold)
        self._threshold_tool_bar.addWidget(self._threshold_counter)
        self._threshold_counter.valueChanged.connect(self.set_threshold)
        self._threshold_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._threshold_slider.setRange(-120, 0, 1)
        self._threshold_slider.setValue(self.threshold)
        self._threshold_slider.setMinimumWidth(200)
        self._threshold_slider.valueChanged.connect(self.set_threshold)
        self._threshold_layout.addWidget(self._threshold_slider)
        self.top_layout.addLayout(self._threshold_layout)
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
        self.top_layout.addLayout(self._protect_samp_layout)
        self._Range_layout = Qt.QVBoxLayout()
        self._Range_tool_bar = Qt.QToolBar(self)
        self._Range_layout.addWidget(self._Range_tool_bar)
        self._Range_tool_bar.addWidget(Qt.QLabel("Range"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._Range_counter = qwt_counter_pyslot()
        self._Range_counter.setRange(0, 1000, 1)
        self._Range_counter.setNumButtons(2)
        self._Range_counter.setValue(self.Range)
        self._Range_tool_bar.addWidget(self._Range_counter)
        self._Range_counter.valueChanged.connect(self.set_Range)
        self._Range_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._Range_slider.setRange(0, 1000, 1)
        self._Range_slider.setValue(self.Range)
        self._Range_slider.setMinimumWidth(200)
        self._Range_slider.valueChanged.connect(self.set_Range)
        self._Range_layout.addWidget(self._Range_slider)
        self.top_layout.addLayout(self._Range_layout)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=1,
                decimation=decim_fac,
                taps=None,
                fractional_bw=None,
        )
        self.radar_ts_fft_cc_0_1 = radar.ts_fft_cc(samp_down/decim_fac,  "packet_len")
        self.radar_ts_fft_cc_0_0 = radar.ts_fft_cc(samp_up/decim_fac,  "packet_len")
        self.radar_ts_fft_cc_0 = radar.ts_fft_cc(samp_cw/decim_fac,  "packet_len")
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc((Range,), (velocity,), (1e16,), (0,), samp_rate, center_freq, -10, True, True, "packet_len")
        (self.radar_static_target_simulator_cc_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0_0 = radar.split_cc(2, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0_0 = radar.split_cc(1, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0_0).set_min_output_buffer(98304)
        self.radar_split_cc_0 = radar.split_cc(0, ((samp_cw/decim_fac,samp_up/decim_fac,samp_down/decim_fac)), "packet_len")
        (self.radar_split_cc_0).set_min_output_buffer(98304)
        self.radar_signal_generator_fmcw_c_0 = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, -sweep_freq/2, sweep_freq, 1, "packet_len")
        (self.radar_signal_generator_fmcw_c_0).set_min_output_buffer(98304)
        self.radar_qtqui_range_velocity_0 = radar.qtqui_range_velocity((), ())
        self.radar_print_results_0 = radar.print_results()
        self.radar_find_max_peak_c_0_0_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, protect_samp, "packet_len")
        self.radar_find_max_peak_c_0_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, protect_samp, "packet_len")
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate/decim_fac, threshold, protect_samp, "packet_len")
        self.radar_estimator_fmcw_0 = radar.estimator_fmcw(samp_rate/decim_fac, center_freq, sweep_freq, samp_up/decim_fac, samp_down/decim_fac)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(98304)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/decim_fac)
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
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_split_cc_0_0_0, 0))
        self.connect((self.radar_ts_fft_cc_0, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.radar_split_cc_0, 0), (self.radar_ts_fft_cc_0, 0))
        self.connect((self.radar_split_cc_0_0, 0), (self.radar_ts_fft_cc_0_0, 0))
        self.connect((self.radar_split_cc_0_0_0, 0), (self.radar_ts_fft_cc_0_1, 0))
        self.connect((self.radar_ts_fft_cc_0_0, 0), (self.radar_find_max_peak_c_0_0, 0))
        self.connect((self.radar_ts_fft_cc_0_1, 0), (self.radar_find_max_peak_c_0_0_0, 0))
        self.connect((self.radar_signal_generator_fmcw_c_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_estimator_fmcw_0, "Msg out", self.radar_print_results_0, "Msg in")
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
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))
        self.set_samp_down(self.samp_up)
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_sweep_freq(self.samp_rate/2)
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.velocity,), (1e16,), (0,), self.samp_rate, self.center_freq, -10, True, True)

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

    def get_samp_cw(self):
        return self.samp_cw

    def set_samp_cw(self, samp_cw):
        self.samp_cw = samp_cw
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.set_min_output_buffer(int((self.samp_up+self.samp_down+self.samp_cw)*2))
        self.set_meas_duration((self.samp_cw+self.samp_up+self.samp_down)/float(self.samp_rate))

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.set_v_res(self.samp_rate/self.samp_cw*3e8/2/self.center_freq)
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.velocity,), (1e16,), (0,), self.samp_rate, self.center_freq, -10, True, True)

    def get_velocity(self):
        return self.velocity

    def set_velocity(self, velocity):
        self.velocity = velocity
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.velocity,), (1e16,), (0,), self.samp_rate, self.center_freq, -10, True, True)
        Qt.QMetaObject.invokeMethod(self._velocity_counter, "setValue", Qt.Q_ARG("double", self.velocity))
        Qt.QMetaObject.invokeMethod(self._velocity_slider, "setValue", Qt.Q_ARG("double", self.velocity))

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        Qt.QMetaObject.invokeMethod(self._threshold_counter, "setValue", Qt.Q_ARG("double", self.threshold))
        Qt.QMetaObject.invokeMethod(self._threshold_slider, "setValue", Qt.Q_ARG("double", self.threshold))
        self.radar_find_max_peak_c_0.set_threshold(self.threshold)
        self.radar_find_max_peak_c_0_0.set_threshold(self.threshold)
        self.radar_find_max_peak_c_0_0_0.set_threshold(self.threshold)

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
        self.radar_find_max_peak_c_0.set_samp_protect(self.protect_samp)
        self.radar_find_max_peak_c_0_0.set_samp_protect(self.protect_samp)
        self.radar_find_max_peak_c_0_0_0.set_samp_protect(self.protect_samp)

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

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/self.decim_fac)

    def get_Range(self):
        return self.Range

    def set_Range(self, Range):
        self.Range = Range
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.velocity,), (1e16,), (0,), self.samp_rate, self.center_freq, -10, True, True)
        Qt.QMetaObject.invokeMethod(self._Range_counter, "setValue", Qt.Q_ARG("double", self.Range))
        Qt.QMetaObject.invokeMethod(self._Range_slider, "setValue", Qt.Q_ARG("double", self.Range))

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

