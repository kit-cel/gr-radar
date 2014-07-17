#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Wed Jul 16 20:16:52 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import numpy
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
        self.velocity = velocity = 0
        self.transpose_len = transpose_len = 20
        self.sync_word2 = sync_word2 = [0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 1, 1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0] 
        self.sync_word1 = sync_word1 = [0., 0., 0., 0., 0., 0., 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 0., 0., 0., 0., 0.]
        self.samp_rate = samp_rate = 5000000
        self.pilot_symbols = pilot_symbols = ((1, 1, 1, -1,),)
        self.pilot_carriers = pilot_carriers = ((-21, -7, 7, 21,),)
        self.payload_mod = payload_mod = digital.constellation_qpsk()
        self.packet_len = packet_len = 2**8
        self.occupied_carriers_all = occupied_carriers_all = (range(-26,27),)
        self.occupied_carriers = occupied_carriers = (range(-26, -21) + range(-20, -7) + range(-6, 0) + range(1, 7) + range(8, 21) + range(22, 27),)
        self.min_output_buffer = min_output_buffer = 0
        self.length_tag_key = length_tag_key = "packet_len"
        self.fft_len = fft_len = 2**7
        self.center_freq = center_freq = 2.45e9
        self.Range = Range = 100

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
        self._velocity_counter.setRange(-1000, 1000, 1)
        self._velocity_counter.setNumButtons(2)
        self._velocity_counter.setValue(self.velocity)
        self._velocity_tool_bar.addWidget(self._velocity_counter)
        self._velocity_counter.valueChanged.connect(self.set_velocity)
        self._velocity_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._velocity_slider.setRange(-1000, 1000, 1)
        self._velocity_slider.setValue(self.velocity)
        self._velocity_slider.setMinimumWidth(200)
        self._velocity_slider.valueChanged.connect(self.set_velocity)
        self._velocity_layout.addWidget(self._velocity_slider)
        self.top_layout.addLayout(self._velocity_layout)
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
        self._Range_counter.setRange(0.1, 10000, 1)
        self._Range_counter.setNumButtons(2)
        self._Range_counter.setValue(self.Range)
        self._Range_tool_bar.addWidget(self._Range_counter)
        self._Range_counter.valueChanged.connect(self.set_Range)
        self._Range_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._Range_slider.setRange(0.1, 10000, 1)
        self._Range_slider.setValue(self.Range)
        self._Range_slider.setMinimumWidth(200)
        self._Range_slider.valueChanged.connect(self.set_Range)
        self._Range_layout.addWidget(self._Range_slider)
        self.top_layout.addLayout(self._Range_layout)
        self.radar_transpose_matrix_vcvc_0_0 = radar.transpose_matrix_vcvc(transpose_len, fft_len, "packet_len")
        self.radar_transpose_matrix_vcvc_0 = radar.transpose_matrix_vcvc(fft_len, transpose_len, "packet_len")
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc((Range, ), (velocity, ), (1e25, ), (0, ), samp_rate, center_freq, -10, True, True, "packet_len")
        self.radar_qtgui_spectrogram_plot_0 = radar.qtgui_spectrogram_plot(fft_len, 500, 'Range', 'Velocity', 'OFDM Radar', (0,10), (0,10), (-15,-8), True, "packet_len")
        self.radar_ofdm_cyclic_prefix_remover_cvc_0 = radar.ofdm_cyclic_prefix_remover_cvc(fft_len, fft_len/4, "packet_len")
        self.fft_vxx_0_1_0 = fft.fft_vcc(transpose_len, False, (()), True, 1)
        self.fft_vxx_0_1 = fft.fft_vcc(fft_len, True, (()), True, 1)
        self.fft_vxx_0_0 = fft.fft_vcc(fft_len, True, (()), True, 1)
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (()), True, 1)
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(fft_len, fft_len+fft_len/4, 0, length_tag_key)
        self.digital_ofdm_carrier_allocator_cvc_0 = digital.ofdm_carrier_allocator_cvc(fft_len, occupied_carriers_all, ((),), ((),), (), length_tag_key)
        self.digital_chunks_to_symbols_xx_0_0 = digital.chunks_to_symbols_bc((payload_mod.points()), 1)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, packet_len, length_tag_key)
        self.blocks_repack_bits_bb_0 = blocks.repack_bits_bb(8, payload_mod.bits_per_symbol(), length_tag_key, False)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*fft_len)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(1, fft_len, 0)
        self.blocks_divide_xx_0 = blocks.divide_cc(fft_len)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(fft_len)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 255, 1000)), True)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.1, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.digital_ofdm_carrier_allocator_cvc_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.digital_chunks_to_symbols_xx_0_0, 0), (self.digital_ofdm_carrier_allocator_cvc_0, 0))
        self.connect((self.radar_ofdm_cyclic_prefix_remover_cvc_0, 0), (self.fft_vxx_0_0, 0))
        self.connect((self.blocks_divide_xx_0, 0), (self.fft_vxx_0_1, 0))
        self.connect((self.fft_vxx_0_1, 0), (self.radar_transpose_matrix_vcvc_0, 0))
        self.connect((self.radar_transpose_matrix_vcvc_0, 0), (self.fft_vxx_0_1_0, 0))
        self.connect((self.fft_vxx_0_1_0, 0), (self.radar_transpose_matrix_vcvc_0_0, 0))
        self.connect((self.blocks_repack_bits_bb_0, 0), (self.digital_chunks_to_symbols_xx_0_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.blocks_repack_bits_bb_0, 0))
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))
        self.connect((self.fft_vxx_0_0, 0), (self.blocks_divide_xx_0, 1))
        self.connect((self.digital_ofdm_carrier_allocator_cvc_0, 0), (self.blocks_divide_xx_0, 0))
        self.connect((self.radar_transpose_matrix_vcvc_0_0, 0), (self.blocks_complex_to_mag_squared_0, 0))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.blocks_nlog10_ff_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.radar_qtgui_spectrogram_plot_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.radar_ofdm_cyclic_prefix_remover_cvc_0, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_velocity(self):
        return self.velocity

    def set_velocity(self, velocity):
        self.velocity = velocity
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range, ), (self.velocity, ), (1e25, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)
        Qt.QMetaObject.invokeMethod(self._velocity_counter, "setValue", Qt.Q_ARG("double", self.velocity))
        Qt.QMetaObject.invokeMethod(self._velocity_slider, "setValue", Qt.Q_ARG("double", self.velocity))

    def get_transpose_len(self):
        return self.transpose_len

    def set_transpose_len(self, transpose_len):
        self.transpose_len = transpose_len

    def get_sync_word2(self):
        return self.sync_word2

    def set_sync_word2(self, sync_word2):
        self.sync_word2 = sync_word2

    def get_sync_word1(self):
        return self.sync_word1

    def set_sync_word1(self, sync_word1):
        self.sync_word1 = sync_word1

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range, ), (self.velocity, ), (1e25, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)

    def get_pilot_symbols(self):
        return self.pilot_symbols

    def set_pilot_symbols(self, pilot_symbols):
        self.pilot_symbols = pilot_symbols

    def get_pilot_carriers(self):
        return self.pilot_carriers

    def set_pilot_carriers(self, pilot_carriers):
        self.pilot_carriers = pilot_carriers

    def get_payload_mod(self):
        return self.payload_mod

    def set_payload_mod(self, payload_mod):
        self.payload_mod = payload_mod

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len

    def get_occupied_carriers_all(self):
        return self.occupied_carriers_all

    def set_occupied_carriers_all(self, occupied_carriers_all):
        self.occupied_carriers_all = occupied_carriers_all

    def get_occupied_carriers(self):
        return self.occupied_carriers

    def set_occupied_carriers(self, occupied_carriers):
        self.occupied_carriers = occupied_carriers

    def get_min_output_buffer(self):
        return self.min_output_buffer

    def set_min_output_buffer(self, min_output_buffer):
        self.min_output_buffer = min_output_buffer

    def get_length_tag_key(self):
        return self.length_tag_key

    def set_length_tag_key(self, length_tag_key):
        self.length_tag_key = length_tag_key

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range, ), (self.velocity, ), (1e25, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)

    def get_Range(self):
        return self.Range

    def set_Range(self, Range):
        self.Range = Range
        self.radar_static_target_simulator_cc_0.setup_targets((self.Range, ), (self.velocity, ), (1e25, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)
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

