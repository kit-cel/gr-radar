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

class qa_estimator_cw (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1024

		packet_len = test_len
		samp_rate = 2000
		
		center_freq = 1e9
		velocity = 15

		src = radar.signal_generator_cw_c(packet_len,samp_rate,(0,0),1)
		head = blocks.head(8,test_len)
		sim = radar.static_target_simulator_cc((10,10),(velocity,velocity),(1e9,1e9),(0,0),(0,),samp_rate,center_freq,1,True,False)
		mult = blocks.multiply_cc()
		fft = radar.ts_fft_cc(packet_len)
		cfar = radar.os_cfar_c(samp_rate, 5, 0, 0.78, 10, True)
		est = radar.estimator_cw(center_freq)
		res = radar.print_results()
		debug = blocks.message_debug()

		self.tb.connect(src,head,(mult,1))
		self.tb.connect(head,sim,(mult,0))
		self.tb.connect(mult,fft,cfar)
		self.tb.msg_connect(cfar,'Msg out',est,'Msg in')
		self.tb.msg_connect(est,'Msg out',res,'Msg in')
		self.tb.msg_connect(est,'Msg out',debug,'store')
		#self.tb.msg_connect(est,'Msg out',debug,'print')

		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		msg = debug.get_message(0)
		self.assertEqual( "rx_time", pmt.symbol_to_string(pmt.nth(0,(pmt.nth(0,msg)))) ) # check rx_time message part (symbol)
		self.assertEqual( 0, pmt.to_uint64(pmt.tuple_ref(pmt.nth(1,(pmt.nth(0,msg))),0)) ) # check rx_time value
		self.assertEqual( 0.0, pmt.to_double(pmt.tuple_ref(pmt.nth(1,(pmt.nth(0,msg))),1)) )
		self.assertEqual( "velocity", pmt.symbol_to_string(pmt.nth(0,(pmt.nth(1,msg)))) ) # check velocity message part (symbol)
		self.assertAlmostEqual( 1, velocity/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg))),0), 2 ) # check velocity value

if __name__ == '__main__':
	gr_unittest.run(qa_estimator_cw)#, "qa_estimator_cw.xml")
