#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2014 Communications Engineering Lab, KIT.
# Copyright 2022 A. Maitland Bottoms
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
  from gnuradio import radar
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import radar

class qa_split_fsk_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        test_len = 1200
        samp_rate = 4000
        samp_per_freq = 2
        blocks_per_tag = 100
        freq_low = 0
        freq_high = 200
        amplitude = 1

        samp_discard = 1

        src = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, freq_low, freq_high, amplitude)
        head = blocks.head(8,test_len)
        split = radar.split_fsk_cc(samp_per_freq,samp_discard)
        snk0 = blocks.vector_sink_c()
        snk1 = blocks.vector_sink_c()
        snk = blocks.vector_sink_c()

        self.tb.connect(src,head,split)
        self.tb.connect((split,0),snk0)
        self.tb.connect((split,1),snk1)
        self.tb.connect(head,snk)
        self.tb.run ()

        # check correct length of data
        nblock = test_len/2/samp_per_freq
        self.assertEqual(len(snk0.data()),nblock*(samp_per_freq-samp_discard))
        self.assertEqual(len(snk1.data()),nblock*(samp_per_freq-samp_discard))

        # split data self
        data = snk.data()
        data0 = []
        data1 = []
        k=0
        while k<len(data):
            k = k+samp_discard
            data0.append(data[k])
            k = k+1
            k = k+samp_discard
            data1.append(data[k])
            k = k+1

        # check if data0/1 is same as split from block
        self.assertComplexTuplesAlmostEqual(data0,snk0.data(),10)
        self.assertComplexTuplesAlmostEqual(data1,snk1.data(),10)



if __name__ == '__main__':
    gr_unittest.run(qa_split_fsk_cc, "qa_split_fsk_cc.xml")
