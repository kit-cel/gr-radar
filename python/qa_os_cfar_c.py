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
from gnuradio import blocks,analog
import radar_swig as radar
from time import sleep
import pmt

class qa_os_cfar_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1000
		samp_rate = 2000
		freq = 200
		ampl = 1
		packet_len = test_len
		compare_sample = 5
		protect_sample = 0
		rel_threshold = 0.78
		mult_threshold = 10
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		fft = radar.ts_fft_cc(packet_len)
		cfar = radar.os_cfar_c(samp_rate, compare_sample, protect_sample, rel_threshold, mult_threshold)
		debug = blocks.message_debug()
		
		self.tb.connect(src,head,s2ts,fft,cfar)
		self.tb.msg_connect(cfar,"Msg out",debug,"store")
		self.tb.msg_connect(cfar,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq,pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),8)
		
	def test_002_t (self):
		# set up fg
		test_len = 2**15
		samp_rate = 250000
		freq = -2000
		ampl = 1
		packet_len = test_len
		min_output_buffer = 2*packet_len
		compare_sample = 5
		protect_sample = 0
		rel_threshold = 0.78
		mult_threshold = 10
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		src.set_min_output_buffer(min_output_buffer)
		head = blocks.head(8,test_len)
		head.set_min_output_buffer(min_output_buffer)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		s2ts.set_min_output_buffer(min_output_buffer)
		fft = radar.ts_fft_cc(packet_len)
		fft.set_min_output_buffer(min_output_buffer)
		cfar = radar.os_cfar_c(samp_rate, compare_sample, protect_sample, rel_threshold, mult_threshold)
		debug = blocks.message_debug()
		
		self.tb.connect(src,head,s2ts,fft,cfar)
		self.tb.msg_connect(cfar,"Msg out",debug,"store")
		self.tb.msg_connect(cfar,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq/pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),1,2)


if __name__ == '__main__':
	gr_unittest.run(qa_os_cfar_c)#, "qa_os_cfar_c.xml")
