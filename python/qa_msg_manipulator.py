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

class qa_msg_manipulator (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up pmt
		in_2 = (-1,2,-3,4,5,0)
		in_3 = (1,2,3,4,5,0)
		in_4 = (1,-2.1,3,-4.2,5.9,0)
		
		pmt_1 = pmt.list2(pmt.string_to_symbol("rx_time"),pmt.from_long(0))
		pmt_2 = pmt.list2(pmt.string_to_symbol("test"),pmt.init_f32vector(6,in_2))
		pmt_3 = pmt.list2(pmt.string_to_symbol("velocity"),pmt.init_f32vector(6,in_3))
		pmt_4 = pmt.list2(pmt.string_to_symbol("test2"),pmt.init_f32vector(6,in_4))
		pmt_in = pmt.list4(pmt_1,pmt_2,pmt_3,pmt_4)
		
		# set up fg
		symbols = ("test", "test2")
		const_add = (1, -2)
		const_mult = (-5, 1)
		
		strobe = blocks.message_strobe(pmt_in,400);
		test = radar.msg_manipulator(symbols,const_add,const_mult)
		debug = blocks.message_debug()
		
		self.tb.msg_connect(strobe, "strobe", test, "Msg in")
		self.tb.msg_connect(test, "Msg out", debug, "store")
		self.tb.msg_connect(test, "Msg out", debug, "print")
		
		# run fg
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		msg = debug.get_message(0)
		out_2 = pmt.f32vector_elements(pmt.nth(1,pmt.nth(1,msg)))
		out_3 = pmt.f32vector_elements(pmt.nth(1,pmt.nth(2,msg)))
		out_4 = pmt.f32vector_elements(pmt.nth(1,pmt.nth(3,msg)))
		
		ref_2 = [0]*6
		ref_3 = [0]*6
		ref_4 = [0]*6
		for k in range(6):
			ref_2[k] = (in_2[k]+const_add[0])*const_mult[0]
			ref_3[k] = in_3[k]
			ref_4[k] = (in_4[k]+const_add[1])*const_mult[1]
			
		for k in range(6): # do asserts
			self.assertAlmostEqual(ref_2[k],out_2[k],3)
			self.assertAlmostEqual(ref_3[k],out_3[k],3)
			self.assertAlmostEqual(ref_4[k],out_4[k],3)


if __name__ == '__main__':
	gr_unittest.run(qa_msg_manipulator)#, "qa_msg_manipulator.xml")
