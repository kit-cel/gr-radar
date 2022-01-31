#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2014 Communications Engineering Lab, KIT.
# Copyright 2022 A. Maitland Bottoms
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks, digital
try:
  from gnuradio import radar
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import radar

class qa_ofdm_cyclic_prefix_remover_cvc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        test_len = 1000
        fft_len = 100;
        cp_len = int(fft_len/4)

        in_data = list(range(0,fft_len))*(test_len//fft_len);

        src = blocks.vector_source_c(in_data)
        s2v = blocks.stream_to_vector(8,fft_len)
        s2ts = blocks.stream_to_tagged_stream(8,fft_len,int(test_len/fft_len),'packet_len')
        cp_add = digital.ofdm_cyclic_prefixer(fft_len,fft_len+cp_len,0,'packet_len')
        cp_remove = radar.ofdm_cyclic_prefix_remover_cvc(fft_len,cp_len,'packet_len')
        v2s = blocks.vector_to_stream(8,fft_len)
        snk = blocks.vector_sink_c()

        self.tb.connect(src,s2v,s2ts,cp_add,cp_remove,v2s,snk)
        self.tb.run ()

        # check data
        out_data = snk.data()
        out_data_real = [0]*len(out_data)
        for k in range(len(out_data)):
            out_data_real[k] = out_data[k].real

        for k in range(len(in_data)):
            self.assertEqual(in_data[k],out_data_real[k])


if __name__ == '__main__':
    gr_unittest.run(qa_ofdm_cyclic_prefix_remover_cvc)#, "qa_ofdm_cyclic_prefix_remover_cvc.xml")
