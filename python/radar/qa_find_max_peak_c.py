#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 Communications Engineering Lab, KIT.
# Copyright 2022 A. Maitland Bottoms
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks, analog
try:
  from gnuradio import radar
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import radar

from time import sleep
import pmt

class qa_find_max_peak_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# test on positive frequencies
		# set up fg
		test_len = 1000
		samp_rate = 2000
		freq = 200
		ampl = 1
		packet_len = test_len
		threshold = -100
		samp_protect = 2
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		fft = radar.ts_fft_cc(packet_len)
		peak = radar.find_max_peak_c(samp_rate, threshold, samp_protect, (0,0), False)
		debug = blocks.message_debug()
		
		self.tb.connect(src,head,s2ts,fft,peak)
		self.tb.msg_connect(peak,"Msg out",debug,"store")
		#self.tb.msg_connect(peak,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq,pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),8)
		
	def test_002_t (self):
		# test on negative frequencies
		# set up fg
		test_len = 1000
		samp_rate = 2000
		freq = -200
		ampl = 1
		packet_len = test_len
		threshold = -100
		samp_protect = 2
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		fft = radar.ts_fft_cc(packet_len)
		peak = radar.find_max_peak_c(samp_rate, threshold, samp_protect, (0,0), False)
		debug = blocks.message_debug()
		
		self.tb.connect(src,head,s2ts,fft,peak)
		self.tb.msg_connect(peak,"Msg out",debug,"store")
		#self.tb.msg_connect(peak,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq,pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),8)
		
	def test_003_t (self):
		# test cut frequency negative freq
		# set up fg
		test_len = 1000
		samp_rate = 2000
		freq1 = -200
		freq2 = -205
		ampl = 1
		packet_len = test_len
		threshold = -100
		samp_protect = 2
		
		src1 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq1, ampl*0.2)
		src2 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq2, ampl)
		add = blocks.add_cc();
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		fft = radar.ts_fft_cc(packet_len)
		peak = radar.find_max_peak_c(samp_rate, threshold, samp_protect, (-200,200), True)
		debug = blocks.message_debug()
		
		self.tb.connect((src1,0), (add,0))
		self.tb.connect((src2,0), (add,1))
		self.tb.connect(add,head,s2ts,fft,peak)
		self.tb.msg_connect(peak,"Msg out",debug,"store")
		#self.tb.msg_connect(peak,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq1,pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),8)
		
	def test_004t (self):
		# test cut frequency positive freq
		# set up fg
		test_len = 1000
		samp_rate = 2000
		freq1 = -200
		freq2 = 205
		ampl = 1
		packet_len = test_len
		threshold = -100
		samp_protect = 2
		
		src1 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq1, ampl*0.2)
		src2 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq2, ampl)
		add = blocks.add_cc();
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		fft = radar.ts_fft_cc(packet_len)
		peak = radar.find_max_peak_c(samp_rate, threshold, samp_protect, (-200,200), True)
		debug = blocks.message_debug()
		
		self.tb.connect((src1,0), (add,0))
		self.tb.connect((src2,0), (add,1))
		self.tb.connect(add,head,s2ts,fft,peak)
		self.tb.msg_connect(peak,"Msg out",debug,"store")
		#self.tb.msg_connect(peak,"Msg out",debug,"print")
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check frequency in os_cfar message with given one
		msg = debug.get_message(0)
		self.assertAlmostEqual(freq1,pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0),8)


if __name__ == '__main__':
	gr_unittest.run(qa_find_max_peak_c)#, "qa_find_max_peak_c.xml")
