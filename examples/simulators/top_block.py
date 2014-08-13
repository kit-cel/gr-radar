#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Wed Aug 13 15:51:22 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import analog
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

from distutils.version import StrictVersion
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
        self.packet_len = packet_len = 2**19
        self.freq_res = freq_res = samp_rate/float(packet_len)
        self.freq = freq = (-1000000,1000000)
        self.center_freq = center_freq = 2.45e9
        self.vel = vel = 50
        self.v_res = v_res = freq_res*3e8/2/center_freq
        self.time_res = time_res = packet_len/float(samp_rate)
        self.range_res = range_res = 3e8/2/float((freq[1]-freq[0]))
        self.min_output_buffer = min_output_buffer = int(packet_len*2)
        self.max_output_buffer = max_output_buffer = 0
        self.decim_fac = decim_fac = 2**10
        self.Range = Range = 30

        ##################################################
        # Blocks
        ##################################################
        self._vel_layout = Qt.QVBoxLayout()
        self._vel_tool_bar = Qt.QToolBar(self)
        self._vel_layout.addWidget(self._vel_tool_bar)
        self._vel_tool_bar.addWidget(Qt.QLabel("vel"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._vel_counter = qwt_counter_pyslot()
        self._vel_counter.setRange(-50, 50, 0.1)
        self._vel_counter.setNumButtons(2)
        self._vel_counter.setValue(self.vel)
        self._vel_tool_bar.addWidget(self._vel_counter)
        self._vel_counter.valueChanged.connect(self.set_vel)
        self._vel_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._vel_slider.setRange(-50, 50, 0.1)
        self._vel_slider.setValue(self.vel)
        self._vel_slider.setMinimumWidth(200)
        self._vel_slider.valueChanged.connect(self.set_vel)
        self._vel_layout.addWidget(self._vel_slider)
        self.top_layout.addLayout(self._vel_layout)
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
        self._Range_counter.setRange(0, 100, 1)
        self._Range_counter.setNumButtons(2)
        self._Range_counter.setValue(self.Range)
        self._Range_tool_bar.addWidget(self._Range_counter)
        self._Range_counter.valueChanged.connect(self.set_Range)
        self._Range_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._Range_slider.setRange(0, 100, 1)
        self._Range_slider.setValue(self.Range)
        self._Range_slider.setMinimumWidth(200)
        self._Range_slider.valueChanged.connect(self.set_Range)
        self._Range_layout.addWidget(self._Range_slider)
        self.top_layout.addLayout(self._Range_layout)
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
        self.radar_ts_fft_cc_0_0 = radar.ts_fft_cc(packet_len/decim_fac,  "packet_len")
        (self.radar_ts_fft_cc_0_0).set_min_output_buffer(1048576)
        self.radar_ts_fft_cc_0 = radar.ts_fft_cc(packet_len/decim_fac,  "packet_len")
        (self.radar_ts_fft_cc_0).set_min_output_buffer(1048576)
        self.radar_trigger_command_0 = radar.trigger_command("echo bla", ("velocity","range"), ((3,10)), ((9,50)))
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc((Range,), (vel, ), (1e14, ), (0,), (0,), samp_rate, center_freq, -10, True, True, "packet_len")
        (self.radar_static_target_simulator_cc_0).set_min_output_buffer(1048576)
        self.radar_signal_generator_cw_c_0_0 = radar.signal_generator_cw_c(packet_len, samp_rate, (freq[1], ), 1, "packet_len")
        (self.radar_signal_generator_cw_c_0_0).set_min_output_buffer(1048576)
        self.radar_signal_generator_cw_c_0 = radar.signal_generator_cw_c(packet_len, samp_rate, (freq[0], ), 1, "packet_len")
        (self.radar_signal_generator_cw_c_0).set_min_output_buffer(1048576)
        self.radar_print_results_0 = radar.print_results(False, "")
        self.radar_find_max_peak_c_0 = radar.find_max_peak_c(samp_rate/decim_fac, -200, 0, (-1000,1000), True, "packet_len")
        self.radar_estimator_fsk_0 = radar.estimator_fsk(center_freq, (freq[1]-freq[0]), False)
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
        self.qtgui_freq_sink_x_0.enable_grid(False)
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
        (self.blocks_throttle_0_0).set_min_output_buffer(1048576)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        (self.blocks_throttle_0).set_min_output_buffer(1048576)
        self.blocks_tagged_stream_multiply_length_0_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0_0).set_min_output_buffer(1048576)
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_gr_complex*1, "packet_len", 1.0/float(decim_fac))
        (self.blocks_tagged_stream_multiply_length_0).set_min_output_buffer(1048576)
        self.blocks_multiply_conjugate_cc_1 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_1).set_min_output_buffer(1048576)
        self.blocks_multiply_conjugate_cc_0_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0_0).set_min_output_buffer(1048576)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1)
        (self.blocks_multiply_conjugate_cc_0).set_min_output_buffer(1048576)
        self.blocks_add_xx_1 = blocks.add_vcc(1)
        (self.blocks_add_xx_1).set_min_output_buffer(1048576)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        (self.blocks_add_xx_0).set_min_output_buffer(1048576)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.5, 0)
        (self.analog_noise_source_x_0).set_min_output_buffer(1048576)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_cw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_conjugate_cc_0, 0))
        self.connect((self.radar_signal_generator_cw_c_0_0, 0), (self.blocks_throttle_0_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_add_xx_1, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_add_xx_1, 1))
        self.connect((self.blocks_add_xx_1, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_conjugate_cc_0_0, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_multiply_conjugate_cc_0_0, 1))
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.blocks_multiply_conjugate_cc_0_0, 0), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.radar_ts_fft_cc_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))
        self.connect((self.blocks_tagged_stream_multiply_length_0_0, 0), (self.radar_ts_fft_cc_0_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.blocks_tagged_stream_multiply_length_0_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.qtgui_freq_sink_x_0, 1))
        self.connect((self.blocks_multiply_conjugate_cc_1, 0), (self.radar_find_max_peak_c_0, 0))
        self.connect((self.radar_ts_fft_cc_0_0, 0), (self.blocks_multiply_conjugate_cc_1, 1))
        self.connect((self.radar_ts_fft_cc_0, 0), (self.blocks_multiply_conjugate_cc_1, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_print_results_0, "Msg in")
        self.msg_connect(self.radar_find_max_peak_c_0, "Msg out", self.radar_estimator_fsk_0, "Msg in")
        self.msg_connect(self.radar_estimator_fsk_0, "Msg out", self.radar_trigger_command_0, "Msg in")

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
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.vel, ), (1e14, ), (0,), (0,), self.samp_rate, self.center_freq, -10, True, True)

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
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.vel, ), (1e14, ), (0,), (0,), self.samp_rate, self.center_freq, -10, True, True)

    def get_vel(self):
        return self.vel

    def set_vel(self, vel):
        self.vel = vel
        Qt.QMetaObject.invokeMethod(self._vel_counter, "setValue", Qt.Q_ARG("double", self.vel))
        Qt.QMetaObject.invokeMethod(self._vel_slider, "setValue", Qt.Q_ARG("double", self.vel))
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.vel, ), (1e14, ), (0,), (0,), self.samp_rate, self.center_freq, -10, True, True)

    def get_v_res(self):
        return self.v_res

    def set_v_res(self, v_res):
        self.v_res = v_res

    def get_time_res(self):
        return self.time_res

    def set_time_res(self, time_res):
        self.time_res = time_res

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

    def get_decim_fac(self):
        return self.decim_fac

    def set_decim_fac(self, decim_fac):
        self.decim_fac = decim_fac
        self.blocks_tagged_stream_multiply_length_0.set_scalar(1.0/float(self.decim_fac))
        self.blocks_tagged_stream_multiply_length_0_0.set_scalar(1.0/float(self.decim_fac))
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.samp_rate/self.decim_fac)

    def get_Range(self):
        return self.Range

    def set_Range(self, Range):
        self.Range = Range
        Qt.QMetaObject.invokeMethod(self._Range_counter, "setValue", Qt.Q_ARG("double", self.Range))
        Qt.QMetaObject.invokeMethod(self._Range_slider, "setValue", Qt.Q_ARG("double", self.Range))
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range,), (self.vel, ), (1e14, ), (0,), (0,), self.samp_rate, self.center_freq, -10, True, True)

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
    if(StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0")):
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
