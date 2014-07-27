#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Sun Jul 27 12:19:10 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import radar
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
        self.samp_rate = samp_rate = 250000
        self.num_skip = num_skip = 50
        self.num_corr = num_corr = 25

        ##################################################
        # Blocks
        ##################################################
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
        self._num_corr_counter.setRange(0, 100, 1)
        self._num_corr_counter.setNumButtons(2)
        self._num_corr_counter.setValue(self.num_corr)
        self._num_corr_tool_bar.addWidget(self._num_corr_counter)
        self._num_corr_counter.valueChanged.connect(self.set_num_corr)
        self._num_corr_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._num_corr_slider.setRange(0, 100, 1)
        self._num_corr_slider.setValue(self.num_corr)
        self._num_corr_slider.setMinimumWidth(200)
        self._num_corr_slider.valueChanged.connect(self.set_num_corr)
        self._num_corr_layout.addWidget(self._num_corr_slider)
        self.top_layout.addLayout(self._num_corr_layout)
        self.radar_signal_generator_sync_pulse_c_0 = radar.signal_generator_sync_pulse_c(2**12, ((300,50,200)), ((100,200,50)), 0.5, "packet_len")
        self.radar_print_results_0 = radar.print_results(False, "")
        self.radar_estimator_sync_pulse_c_0 = radar.estimator_sync_pulse_c(int(num_corr), "packet_len")
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_skiphead_0 = blocks.skiphead(gr.sizeof_gr_complex*1, num_skip)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_signal_generator_sync_pulse_c_0, 0), (self.blocks_skiphead_0, 0))
        self.connect((self.radar_signal_generator_sync_pulse_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.radar_estimator_sync_pulse_c_0, 1))
        self.connect((self.blocks_skiphead_0, 0), (self.radar_estimator_sync_pulse_c_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_estimator_sync_pulse_c_0, "Msg out", self.radar_print_results_0, "Msg in")

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_num_skip(self):
        return self.num_skip

    def set_num_skip(self, num_skip):
        self.num_skip = num_skip

    def get_num_corr(self):
        return self.num_corr

    def set_num_corr(self, num_corr):
        self.num_corr = num_corr
        self.radar_estimator_sync_pulse_c_0.set_num_xcorr(int(self.num_corr))
        Qt.QMetaObject.invokeMethod(self._num_corr_counter, "setValue", Qt.Q_ARG("double", self.num_corr))
        Qt.QMetaObject.invokeMethod(self._num_corr_slider, "setValue", Qt.Q_ARG("double", self.num_corr))

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
