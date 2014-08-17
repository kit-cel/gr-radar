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
import pmt
from time import sleep

class qa_trigger_command (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		Range = (10, 20, 30)
		power = (15, 30, 45)

		pmt_range = pmt.list2(pmt.string_to_symbol('range'),pmt.init_f32vector(len(Range),Range))
		pmt_power = pmt.list2(pmt.string_to_symbol('power'),pmt.init_f32vector(len(power),power))
		pmt_misc = pmt.list2(pmt.string_to_symbol('misc'),pmt.init_f32vector(3,(1,2,3)))
		pmt_in = pmt.list3(pmt_misc,pmt_range,pmt_power)

		src = blocks.message_strobe(pmt_in, 300)
		test = radar.trigger_command("echo test_command",("Range","power"),(5, 10),(35, 50))
		
		self.tb.msg_connect(src,"strobe",test,"Msg in")
		
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		# check data


if __name__ == '__main__':
	gr_unittest.run(qa_trigger_command)#, "qa_trigger_command.xml")
