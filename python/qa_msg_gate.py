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
from time import sleep
import pmt

class qa_msg_gate (gr_unittest.TestCase):

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
		velocity = (5,15,20)

		src = radar.signal_generator_cw_c(packet_len,samp_rate,(0,0),1)
		head = blocks.head(8,test_len)
		sim = radar.static_target_simulator_cc((10,10,10),velocity,(1e12,1e12,1e12),(0,0,0),(0,),samp_rate,center_freq,1,True,False)
		mult = blocks.multiply_cc()
		fft = radar.ts_fft_cc(packet_len)
		cfar = radar.os_cfar_c(samp_rate, 5, 0, 0.78, 10, True)
		est = radar.estimator_cw(center_freq)
		res1 = radar.print_results()
		res2 = radar.print_results()
		gate = radar.msg_gate(('velocity','bla'),(8,8),(17,17))
		debug1 = blocks.message_debug()
		debug2 = blocks.message_debug()

		self.tb.connect(src,head,(mult,1))
		self.tb.connect(head,sim,(mult,0))
		self.tb.connect(mult,fft,cfar)
		self.tb.msg_connect(cfar,'Msg out',est,'Msg in')
		self.tb.msg_connect(est,'Msg out',res1,'Msg in')
		self.tb.msg_connect(est,'Msg out',debug1,'store')
		self.tb.msg_connect(est,'Msg out',gate,'Msg in')
		self.tb.msg_connect(gate,'Msg out',debug2,'store')
		self.tb.msg_connect(gate,'Msg out',res2,'Msg in')

		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		msg1 = debug1.get_message(0) # msg without gate
		msg2 = debug2.get_message(0) # msg with gate
		self.assertEqual( "velocity", pmt.symbol_to_string(pmt.nth(0,(pmt.nth(1,msg1)))) ) # check velocity message part (symbol), 1
		self.assertEqual( "velocity", pmt.symbol_to_string(pmt.nth(0,(pmt.nth(1,msg2)))) ) # check velocity message part (symbol), 2
		self.assertEqual(pmt.length(pmt.nth(1,pmt.nth(1,msg1))),3) # check number of targets without gate
		self.assertEqual(pmt.length(pmt.nth(1,pmt.nth(1,msg2))),1) # check nubmer of targets with gate
		self.assertAlmostEqual( 1, velocity[1]/pmt.f32vector_ref(pmt.nth(1,(pmt.nth(1,msg2))),0), 1 ) # check velocity value


if __name__ == '__main__':
	#raw_input('Block for running gdb',)
	gr_unittest.run(qa_msg_gate)#, "qa_msg_gate.xml")
