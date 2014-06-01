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

class qa_signal_generator_fsk_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# compare signal in time domain
		# set up fg
		test_len = 1000
		
		samp_rate = 4000
		samp_per_freq = 2
		blocks_per_tag = 100
		freq_low = 0
		freq_high = 200
		amplitude = 1
		
		src = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, freq_low, freq_high, amplitude)
		head = blocks.head(8,test_len)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,head,snk)
		self.tb.run ()
		
		# get ref data
		ref_data = [0]*test_len
		phase_low = 0
		phase_high = 0
		counter = 0
		state = 0
		for k in range(len(ref_data)):
			if counter==samp_per_freq:
				if state:
					state = 0
				else:
					state = 1
				counter = 0
			
			if state:
				ref_data[k] = amplitude*np.exp(1j*phase_high)
			else:
				ref_data[k] = amplitude*np.exp(1j*phase_low) # first is low
				
			phase_low = phase_low+2*np.pi*freq_low/samp_rate
			phase_high = phase_high+2*np.pi*freq_high/samp_rate
			counter = counter+1
		
		# check data
		data = snk.data()
		self.assertComplexTuplesAlmostEqual(data,ref_data,4)
		
	def test_002_t (self):
		# compare signal in frequency domain
		# set up fg
		test_len = 1000
		
		samp_rate = 4000
		samp_per_freq = 1
		blocks_per_tag = 100
		freq_low = 0
		freq_high = 200
		amplitude = 1
		
		src = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, freq_low, freq_high, amplitude)
		head = blocks.head(8,test_len)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,head,snk)
		self.tb.run ()
		
		# split data
		data = snk.data()
		data0 = [0]*(test_len/2)
		data1 = [0]*(test_len/2)
		
		for k in range(test_len/2):
			data0[k] = data[2*k]
			data1[k] = data[2*k+1]
		
		# check data with fft
		fft0 = numpy.fft.fft(data0) # get fft
		fft1 = numpy.fft.fft(data1) # get fft
		num0 = np.argmax(abs(fft0)) # index of max sample
		fft_freq0 = samp_rate/2*num0/len(fft0) # calc freq out of max sample index, works only for frequencies < samp_rate/2!, samp rate is halfed
		num1 = np.argmax(abs(fft1)) # index of max sample
		fft_freq1 = samp_rate/2*num1/len(fft1) # calc freq out of max sample index, works only for frequencies < samp_rate/2!, samp rate is halfed
		
		self.assertEqual(freq_low,fft_freq0) # check if freq is correct
		self.assertEqual(freq_high,fft_freq1)

if __name__ == '__main__':
	gr_unittest.run(qa_signal_generator_fsk_c)#, "qa_signal_generator_fsk_c.xml")
