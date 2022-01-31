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

class qa_signal_generator_fmcw_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1000
		
		samp_rate = 32000
		samp_up = 100
		samp_down = 50
		samp_cw = 75
		freq_cw = 500
		freq_sweep = 1000
		amplitude = 1
		
		test = radar.signal_generator_fmcw_c(samp_rate,samp_up,samp_down,samp_cw,freq_cw,freq_sweep,amplitude)
		head = blocks.head(8,test_len)
		sink = blocks.vector_sink_c()
		
		self.tb.connect(test,head,sink)
		self.tb.run ()
		
		# create waveform and ref_data
		packet_len = samp_cw+samp_up+samp_down
		waveform = [0]*packet_len
		ref_data = [0]*test_len
		phase = 0
		
		for k in range(samp_cw): # create waveform
			waveform[k] = freq_cw
		for k in range(samp_up):
			waveform[samp_cw+k] = freq_cw+freq_sweep*k/samp_up
		for k in range(samp_down):
			waveform[samp_cw+samp_up+k] = freq_cw+freq_sweep-freq_sweep*k/samp_down
		
		counter = 0
		for k in range(test_len): # integrate phase for iq signal
			if counter%packet_len==0:
				counter = 0
			ref_data[k] = amplitude*np.exp(1j*phase)
			phase = phase + 2*np.pi*waveform[counter]/samp_rate
			counter = counter+1
		
		# check data
		out_data = sink.data()
		self.assertEqual(len(out_data),test_len) # check size of out_data
		self.assertComplexTuplesAlmostEqual(ref_data,out_data,4) # check out_data with ref_data

if __name__ == '__main__':
	gr_unittest.run(qa_signal_generator_fmcw_c,)# "qa_signal_generator_fmcw_c.xml")
