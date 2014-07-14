#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Jul 14 15:59:52 2014
##################################################

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
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
        self.transpose_len = transpose_len = 11
        self.sync_word2 = sync_word2 = [0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 1, 1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0] 
        self.sync_word1 = sync_word1 = [0., 0., 0., 0., 0., 0., 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 0., 0., 0., 0., 0.]
        self.samp_rate_0 = samp_rate_0 = 100000
        self.samp_rate = samp_rate = 2000000
        self.pilot_symbols = pilot_symbols = ((1, 1, 1, -1,),)
        self.pilot_carriers = pilot_carriers = ((-21, -7, 7, 21,),)
        self.payload_mod = payload_mod = digital.constellation_qpsk()
        self.packet_len = packet_len = 400
        self.occupied_carriers = occupied_carriers = (range(-26, -21) + range(-20, -7) + range(-6, 0) + range(1, 7) + range(8, 21) + range(22, 27),)
        self.length_tag_key = length_tag_key = "packet_len"
        self.fft_len = fft_len = 64
        self.center_freq = center_freq = 2.45e9

        ##################################################
        # Blocks
        ##################################################
        self.radar_transpose_matrix_vcvc_0_0 = radar.transpose_matrix_vcvc(transpose_len, fft_len, "packet_len")
        self.radar_transpose_matrix_vcvc_0 = radar.transpose_matrix_vcvc(fft_len, transpose_len, "packet_len")
        self.radar_static_target_simulator_cc_0 = radar.static_target_simulator_cc((100, ), (20, ), (1e12, ), (0, ), samp_rate, center_freq, -10, True, True, "packet_len")
        self.radar_ofdm_cyclic_prefix_remover_cvc_0 = radar.ofdm_cyclic_prefix_remover_cvc(fft_len, fft_len/4, "packet_len")
        self.fft_vxx_0_1_0 = fft.fft_vcc(transpose_len, False, (()), True, 1)
        self.fft_vxx_0_1 = fft.fft_vcc(fft_len, True, (()), True, 1)
        self.fft_vxx_0_0 = fft.fft_vcc(fft_len, True, (()), True, 1)
        self.fft_vxx_0 = fft.fft_vcc(fft_len, False, (()), True, 1)
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(fft_len, fft_len+fft_len/4, 0, length_tag_key)
        self.digital_ofdm_carrier_allocator_cvc_0 = digital.ofdm_carrier_allocator_cvc(fft_len, occupied_carriers, pilot_carriers, pilot_symbols, (sync_word1, sync_word2), length_tag_key)
        self.digital_chunks_to_symbols_xx_0_0 = digital.chunks_to_symbols_bc((payload_mod.points()), 1)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_gr_complex*fft_len, "", ""); self.blocks_tag_debug_0.set_display(True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, packet_len, length_tag_key)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*fft_len)
        self.blocks_divide_xx_0 = blocks.divide_cc(fft_len)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 255, 1000)), True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.digital_chunks_to_symbols_xx_0_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.radar_static_target_simulator_cc_0, 0))
        self.connect((self.digital_ofdm_carrier_allocator_cvc_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.digital_chunks_to_symbols_xx_0_0, 0), (self.digital_ofdm_carrier_allocator_cvc_0, 0))
        self.connect((self.radar_static_target_simulator_cc_0, 0), (self.radar_ofdm_cyclic_prefix_remover_cvc_0, 0))
        self.connect((self.radar_ofdm_cyclic_prefix_remover_cvc_0, 0), (self.fft_vxx_0_0, 0))
        self.connect((self.digital_ofdm_carrier_allocator_cvc_0, 0), (self.blocks_divide_xx_0, 1))
        self.connect((self.fft_vxx_0_0, 0), (self.blocks_divide_xx_0, 0))
        self.connect((self.blocks_divide_xx_0, 0), (self.fft_vxx_0_1, 0))
        self.connect((self.fft_vxx_0_1, 0), (self.radar_transpose_matrix_vcvc_0, 0))
        self.connect((self.radar_transpose_matrix_vcvc_0, 0), (self.fft_vxx_0_1_0, 0))
        self.connect((self.fft_vxx_0_1_0, 0), (self.radar_transpose_matrix_vcvc_0_0, 0))
        self.connect((self.radar_transpose_matrix_vcvc_0_0, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.blocks_divide_xx_0, 0), (self.blocks_tag_debug_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

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

    def get_samp_rate_0(self):
        return self.samp_rate_0

    def set_samp_rate_0(self, samp_rate_0):
        self.samp_rate_0 = samp_rate_0

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.radar_static_target_simulator_cc_0.setup_targets((100, ), (20, ), (1e12, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)

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

    def get_occupied_carriers(self):
        return self.occupied_carriers

    def set_occupied_carriers(self, occupied_carriers):
        self.occupied_carriers = occupied_carriers

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
        self.radar_static_target_simulator_cc_0.setup_targets((100, ), (20, ), (1e12, ), (0, ), self.samp_rate, self.center_freq, -10, True, True)

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

