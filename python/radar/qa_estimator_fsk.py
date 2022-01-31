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

		packet_len = 2**19
		min_output_buffer = packet_len*2
		samp_rate = 5000000
		
		center_freq = 2.45e9
		
		Range = 50
		velocity = 5
				
		samp_per_freq = 1
		blocks_per_tag = int(packet_len/2)
		freq_low = 0.0
		freq_high = 1250000
		amplitude = 1.0
		samp_discard = 0

		src = radar.signal_generator_fsk_c(samp_rate, samp_per_freq, blocks_per_tag, freq_low, freq_high, amplitude)
		src.set_min_output_buffer(min_output_buffer)
		
		head = blocks.head(8,test_len)
		head.set_min_output_buffer(min_output_buffer)
		
		sim = radar.static_target_simulator_cc((Range,),(velocity,),(1e20,),(0,),(0,),samp_rate,center_freq,1,False,False)
		sim.set_min_output_buffer(min_output_buffer)
		
		mult = blocks.multiply_conjugate_cc()
		mult.set_min_output_buffer(min_output_buffer)
		
		fft1 = radar.ts_fft_cc(int(packet_len/2))
		fft1.set_min_output_buffer(min_output_buffer)
		fft2 = radar.ts_fft_cc(int(packet_len/2))
		fft2.set_min_output_buffer(min_output_buffer)
		
		split = radar.split_fsk_cc(samp_per_freq,samp_discard)
		split.set_min_output_buffer(min_output_buffer)
		
		mult_conj = blocks.multiply_conjugate_cc()
		mult_conj.set_min_output_buffer(min_output_buffer)
		
		cfar = radar.find_max_peak_c(int(samp_rate/2),-120,0,(),False)
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
		#print "VELOCITY:", pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg))),0), velocity
		#print "RANGE:", pmt.f32vector_ref(pmt.nth(1,(pmt.nth(2,msg))),0), Range
		self.assertAlmostEqual( 1, velocity/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg))),0), 1 ) # check velocity value
		self.assertAlmostEqual( 1, Range/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(2,msg))),0), 1 ) # check range value


if __name__ == '__main__':
	#raw_input('Block for running gdb',)
	gr_unittest.run(qa_estimator_fsk)#, "qa_estimator_fsk.xml")
