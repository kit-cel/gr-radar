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
from gnuradio import blocks, filter
import radar_swig as radar
from time import sleep
import pmt
from matplotlib import pyplot as plt
import numpy as np

class qa_estimator_fmcw (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		samp_cw = 2**14
		samp_up = 2**14
		samp_down = samp_up
		packet_len = samp_cw+samp_up+samp_down
		min_output_buffer = packet_len*2
		test_len = 2*packet_len
		samp_rate = 10000000
		
		center_freq = 5.7e9
		
		Range = 200
		velocity = 50
		
		freq_cw = 0
		freq_sweep = samp_rate/2
		amplitude = 1

		src = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, freq_cw, freq_sweep, amplitude)
		src.set_min_output_buffer(min_output_buffer)
		
		head = blocks.head(8,test_len)
		head.set_min_output_buffer(min_output_buffer)
		
		sim = radar.static_target_simulator_cc((Range,),(velocity,),(1e16,),(0,),samp_rate,center_freq,1,False,False)
		sim.set_min_output_buffer(min_output_buffer)
		
		mult = blocks.multiply_conjugate_cc()
		mult.set_min_output_buffer(min_output_buffer)
		
		decim_fac = 2**4
		
		resamp = filter.rational_resampler_ccc(1,decim_fac)
		resamp_tag = blocks.tagged_stream_multiply_length(8,'packet_len',1.0/float(decim_fac))
		resamp_tag.set_min_output_buffer(min_output_buffer/(decim_fac))
		
		packets = (samp_cw/(decim_fac), samp_up/(decim_fac), samp_down/(decim_fac))
		split_cw = radar.split_cc(0,packets)
		split_up = radar.split_cc(1,packets)
		split_down = radar.split_cc(2,packets)
		split_cw.set_min_output_buffer(min_output_buffer/(decim_fac))
		split_up.set_min_output_buffer(min_output_buffer/(decim_fac))
		split_down.set_min_output_buffer(min_output_buffer/(decim_fac))
		
		fft_cw = radar.ts_fft_cc(samp_cw/(decim_fac))
		fft_up = radar.ts_fft_cc(samp_up/(decim_fac))
		fft_down = radar.ts_fft_cc(samp_down/(decim_fac))
		fft_cw.set_min_output_buffer(min_output_buffer/(decim_fac))
		fft_up.set_min_output_buffer(min_output_buffer/(decim_fac))
		fft_down.set_min_output_buffer(min_output_buffer/(decim_fac))
		
		threshold = -300
		samp_protect = 0
		cfar_cw = radar.find_max_peak_c(samp_rate/(decim_fac), threshold, samp_protect)
		cfar_up = radar.find_max_peak_c(samp_rate/(decim_fac), threshold, samp_protect)
		cfar_down = radar.find_max_peak_c(samp_rate/(decim_fac), threshold, samp_protect)
		
		est = radar.estimator_fmcw(samp_rate/(decim_fac), center_freq, freq_sweep, samp_up/(decim_fac), samp_down/(decim_fac))
		
		res = radar.print_results()
		debug = blocks.message_debug()
		
		self.tb.connect(src,head,(mult,0))
		self.tb.connect(head,sim,(mult,1))
		self.tb.connect(mult,resamp, resamp_tag)
		self.tb.connect(resamp_tag,split_cw, fft_cw, cfar_cw)
		self.tb.connect(resamp_tag,split_up, fft_up, cfar_up)
		self.tb.connect(resamp_tag,split_down, fft_down, cfar_down)
		
		self.tb.msg_connect(cfar_cw,'Msg out',est,'Msg in CW')
		self.tb.msg_connect(cfar_up,'Msg out',est,'Msg in UP')
		self.tb.msg_connect(cfar_down,'Msg out',est,'Msg in DOWN')
		self.tb.msg_connect(est,'Msg out',res,'Msg in')
		self.tb.msg_connect(est,'Msg out',debug,'store')
		
		# run fg
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		msg = debug.get_message(0)
		self.assertGreater( velocity/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg))),0), 0.8 ) # check velocity value
		self.assertGreater( Range/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(2,msg))),0), 0.8 ) # check range value


if __name__ == '__main__':
	gr_unittest.run(qa_estimator_fmcw)#, "qa_estimator_fmcw.xml")
