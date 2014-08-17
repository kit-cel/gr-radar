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
	gr_unittest.run(qa_signal_generator_cw_c)#, "qa_signal_generator_cw_c.xml")
