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

import numpy as np

class qa_signal_generator_cw_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1000
		
		packet_len = 1024
		samp_rate = 32000
		frequency = (500, 500)
		amplitude = 1
		
		test = radar.signal_generator_cw_c(packet_len, samp_rate, frequency, amplitude)
		head = blocks.head(8,test_len)
		sink = blocks.vector_sink_c()
		
		self.tb.connect(test,head,sink)
		self.tb.run ()
		
		# create reference data
		ref_data = [0]*test_len
		phase = 0
		for i in range(test_len):
			ref_data[i] = amplitude*np.exp(1j*phase)
			phase = phase + 2*np.pi*frequency[0]/samp_rate
			#phase = np.modf(phase,2*np.pi)
		
		ref_data = [0]*test_len
		phase = 0
		for i in range(test_len):
			ref_data[i] = amplitude*np.exp(1j*phase)
			phase = phase+2*np.pi*frequency[0]/samp_rate
		
		# check data
		out_data = sink.data()
		
		self.assertEqual(len(out_data),test_len) # check out_data length
		self.assertComplexTuplesAlmostEqual(out_data,ref_data,2) # check out_data with ref_data

if __name__ == '__main__':
	gr_unittest.run(qa_signal_generator_cw_c)
