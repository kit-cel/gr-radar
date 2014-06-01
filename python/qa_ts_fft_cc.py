#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
import numpy.fft

class qa_ts_fft_cc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1024
		
		packet_len = test_len
		samp_rate = 2000
		frequency = (100,100)
		amplitude = 1
		
		src = radar.signal_generator_cw_c(packet_len,samp_rate,frequency,amplitude)
		head = blocks.head(8,test_len)
		fft = radar.ts_fft_cc(packet_len)
		snk1 = blocks.vector_sink_c()
		snk2 = blocks.vector_sink_c()
		
		self.tb.connect(src,head,fft,snk2) # snk2 holds fft data
		self.tb.connect(head,snk1) # snk1 holds time samples
		self.tb.run ()
		# check data
		data = snk1.data()
		np_fft = numpy.fft.fft(data) # get fft
		self.assertComplexTuplesAlmostEqual(snk2.data(),np_fft,4) # compare numpy fft and fft from block
		
	def test_002_t (self):
		# set up fg
		# purpse is testing fft on high sample rates
		test_len = 2**19
		
		packet_len = 2**17
		min_output_buffer = packet_len*2
		samp_rate = 10000000
		frequency = 200000
		amplitude = 1
		
		src = radar.signal_generator_cw_c(packet_len,samp_rate,(frequency,frequency),amplitude)
		src.set_min_output_buffer(min_output_buffer)
		
		head = blocks.head(8,test_len)
		head.set_min_output_buffer(min_output_buffer)
		
		fft1 = radar.ts_fft_cc(packet_len)
		fft1.set_min_output_buffer(min_output_buffer)
		#fft2 = radar.ts_fft_cc()
		#fft2.set_min_output_buffer(min_output_buffer)
		
		snk1 = blocks.vector_sink_c()
		#snk2 = blocks.vector_sink_c()
		
		self.tb.connect(src,head,fft1,snk1)
		#self.tb.connect(head,fft2,snk2)
		self.tb.run ()

if __name__ == '__main__':
	#raw_input('block for gdb',)
	gr_unittest.run(qa_ts_fft_cc)#, "qa_ts_fft_cc.xml")
