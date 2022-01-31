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
		block_time = 100

		pmt_range = pmt.list2(pmt.string_to_symbol('range'),pmt.init_f32vector(len(Range),Range))
		pmt_power = pmt.list2(pmt.string_to_symbol('power'),pmt.init_f32vector(len(power),power))
		pmt_misc = pmt.list2(pmt.string_to_symbol('misc'),pmt.init_f32vector(3,(1,2,3)))
		pmt_in = pmt.list3(pmt_misc,pmt_range,pmt_power)

		src = blocks.message_strobe(pmt_in, 100)
		test = radar.trigger_command("echo test_command",("Range","power"),(5, 10),(35, 50), block_time)
		
		self.tb.msg_connect(src,"strobe",test,"Msg in")
		
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		# check data


if __name__ == '__main__':
	gr_unittest.run(qa_trigger_command)#, "qa_trigger_command.xml")
