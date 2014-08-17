#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 Communications Engineering Lab, KIT.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import radar_swig as radar
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
