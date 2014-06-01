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
from time import sleep
import pmt

class qa_estimator_fsk (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# run full fsk setup on high sample rates
		test_len = 2**19

		packet_len = 2**17
		min_output_buffer = packet_len*2
		samp_rate = 5000000
		
		center_freq = 5.7e9
		
		Range = 90
		velocity = 10
				
		samp_per_freq = 1
		blocks_per_tag = packet_len/2
		freq_low = 0
		freq_high = 1250000
		amplitude = 1
		samp_discard = 0

		src = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, freq_low, freq_high, amplitude)
		src.set_min_output_buffer(min_output_buffer)
		
		head = blocks.head(8,test_len)
		head.set_min_output_buffer(min_output_buffer)
		
		sim = radar.static_target_simulator_cc((Range,Range),(velocity,velocity),(1e12,1e12),(0,0),samp_rate,center_freq,1,False,False)
		sim.set_min_output_buffer(min_output_buffer)
		
		mult = blocks.multiply_conjugate_cc()
		mult.set_min_output_buffer(min_output_buffer)
		
		fft1 = radar.ts_fft_cc(packet_len/2)
		fft1.set_min_output_buffer(min_output_buffer)
		fft2 = radar.ts_fft_cc(packet_len/2)
		fft2.set_min_output_buffer(min_output_buffer)
		
		split = radar.split_fsk_cc(samp_per_freq,samp_discard)
		split.set_min_output_buffer(min_output_buffer)
		
		mult_conj = blocks.multiply_conjugate_cc()
		mult_conj.set_min_output_buffer(min_output_buffer)
		
		cfar = radar.os_cfar_c(samp_rate/2, 5, 0, 0.78, 10, True)
		cfar.set_min_output_buffer(min_output_buffer)
		
		est = radar.estimator_fsk(center_freq,freq_high-freq_low)
		res = radar.print_results()
		debug = blocks.message_debug()

		self.tb.connect(src,head,(mult,1))
		self.tb.connect(head,sim,(mult,0))
		self.tb.connect(mult,split)
		self.tb.connect((split,0),fft1)
		self.tb.connect((split,1),fft2)
		self.tb.connect(fft1,(mult_conj,0))
		self.tb.connect(fft2,(mult_conj,1))
		self.tb.connect(mult_conj,cfar)
		self.tb.msg_connect(cfar,'Msg out',est,'Msg in')
		self.tb.msg_connect(est,'Msg out',res,'Msg in')
		self.tb.msg_connect(est,'Msg out',debug,'store')

		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		msg = debug.get_message(0)
		self.assertAlmostEqual( 1, velocity/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg))),0), 2 ) # check velocity value
		self.assertAlmostEqual( 1, Range/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(2,msg))),0), 2 ) # check range value


if __name__ == '__main__':
	#raw_input('Block for running gdb',)
	gr_unittest.run(qa_estimator_fsk)#, "qa_estimator_fsk.xml")
