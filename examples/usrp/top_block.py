#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Jun  2 18:48:31 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
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
        self.packet_len = packet_len = 50
        self.wait_to_start = wait_to_start = 0.02
        self.wait_samp = wait_samp = 10
        self.send_samp = send_samp = 10
        self.samp_rate = samp_rate = 250000
        self.num_delay_samp = num_delay_samp = 0
        self.num_corr = num_corr = 50
        self.min_output_buffer = min_output_buffer = packet_len*2
        self.center_freq = center_freq = 2400000000

        ##################################################
        # Blocks
        ##################################################
        self._num_delay_samp_layout = Qt.QVBoxLayout()
        self._num_delay_samp_tool_bar = Qt.QToolBar(self)
        self._num_delay_samp_layout.addWidget(self._num_delay_samp_tool_bar)
        self._num_delay_samp_tool_bar.addWidget(Qt.QLabel("num_delay_samp"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._num_delay_samp_counter = qwt_counter_pyslot()
        self._num_delay_samp_counter.setRange(0, packet_len, 1)
        self._num_delay_samp_counter.setNumButtons(2)
        self._num_delay_samp_counter.setValue(self.num_delay_samp)
        self._num_delay_samp_tool_bar.addWidget(self._num_delay_samp_counter)
        self._num_delay_samp_counter.valueChanged.connect(self.set_num_delay_samp)
        self._num_delay_samp_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._num_delay_samp_slider.setRange(0, packet_len, 1)
        self._num_delay_samp_slider.setValue(self.num_delay_samp)
        self._num_delay_samp_slider.setMinimumWidth(200)
        self._num_delay_samp_slider.valueChanged.connect(self.set_num_delay_samp)
        self._num_delay_samp_layout.addWidget(self._num_delay_samp_slider)
        self.top_layout.addLayout(self._num_delay_samp_layout)
        self._num_corr_layout = Qt.QVBoxLayout()
        self._num_corr_tool_bar = Qt.QToolBar(self)
        self._num_corr_layout.addWidget(self._num_corr_tool_bar)
        self._num_corr_tool_bar.addWidget(Qt.QLabel("num_corr"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._num_corr_counter = qwt_counter_pyslot()
        self._num_corr_counter.setRange(0, packet_len, 1)
        self._num_corr_counter.setNumButtons(2)
        self._num_corr_counter.setValue(self.num_corr)
        self._num_corr_tool_bar.addWidget(self._num_corr_counter)
        self._num_corr_counter.valueChanged.connect(self.set_num_corr)
        self._num_corr_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._num_corr_slider.setRange(0, packet_len, 1)
        self._num_corr_slider.setValue(self.num_corr)
        self._num_corr_slider.setMinimumWidth(200)
        self._num_corr_slider.valueChanged.connect(self.set_num_corr)
        self._num_corr_layout.addWidget(self._num_corr_slider)
        self.top_layout.addLayout(self._num_corr_layout)
        self.radar_usrp_echotimer_cc_0 = radar.usrp_echotimer_cc(samp_rate, center_freq, int(num_delay_samp), 'addr=192.168.10.6', '', 'internal', 'none', 'J1', 0, 0.1, wait_to_start, 0, 'addr=192.168.10.4', '', 'mimo', 'mimo', 'J1', 65, 0.1, wait_to_start, 0, "packet_len")
        (self.radar_usrp_echotimer_cc_0).set_min_output_buffer(100)
        self.radar_signal_generator_sync_pulse_c_0 = radar.signal_generator_sync_pulse_c(packet_len, (send_samp, ), (wait_samp, ), 0.5, "packet_len")
        (self.radar_signal_generator_sync_pulse_c_0).set_min_output_buffer(100)
        self.radar_print_results_0 = radar.print_results()
        self.radar_estimator_sync_pulse_c_0 = radar.estimator_sync_pulse_c(int(num_corr), "packet_len")
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	packet_len, #size
        	samp_rate, #samp_rate
        	"QT GUI Plot", #name
        	2 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.blocks_complex_to_real_0_0 = blocks.complex_to_real(1)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_sync_pulse_c_0, 0), (self.radar_usrp_echotimer_cc_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.blocks_complex_to_real_0_0, 0))
        self.connect((self.radar_signal_generator_sync_pulse_c_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.blocks_complex_to_real_0_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.radar_usrp_echotimer_cc_0, 0), (self.radar_estimator_sync_pulse_c_0, 1))
        self.connect((self.radar_signal_generator_sync_pulse_c_0, 0), (self.radar_estimator_sync_pulse_c_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_estimator_sync_pulse_c_0, "Msg out", self.radar_print_results_0, "Msg in")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.set_min_output_buffer(self.packet_len*2)

    def get_wait_to_start(self):
        return self.wait_to_start

    def set_wait_to_start(self, wait_to_start):
        self.wait_to_start = wait_to_start

    def get_wait_samp(self):
        return self.wait_samp

    def set_wait_samp(self, wait_samp):
        self.wait_samp = wait_samp

    def get_send_samp(self):
        return self.send_samp

    def set_send_samp(self, send_samp):
        self.send_samp = send_samp

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_num_delay_samp(self):
        return self.num_delay_samp

    def set_num_delay_samp(self, num_delay_samp):
        self.num_delay_samp = num_delay_samp
        Qt.QMetaObject.invokeMethod(self._num_delay_samp_counter, "setValue", Qt.Q_ARG("double", self.num_delay_samp))
        Qt.QMetaObject.invokeMethod(self._num_delay_samp_slider, "setValue", Qt.Q_ARG("double", self.num_delay_samp))
        self.radar_usrp_echotimer_cc_0.set_num_delay_samps(int(self.num_delay_samp))

    def get_num_corr(self):
        return self.num_corr

    def set_num_corr(self, num_corr):
        self.num_corr = num_corr
        Qt.QMetaObject.invokeMethod(self._num_corr_counter, "setValue", Qt.Q_ARG("double", self.num_corr))
        Qt.QMetaObject.invokeMethod(self._num_corr_slider, "setValue", Qt.Q_ARG("double", self.num_corr))
        self.radar_estimator_sync_pulse_c_0.set_num_xcorr(int(self.num_corr))

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

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

