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
import pmt
from time import sleep

class qa_estimator_rcs (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		num_mean = 1
		center_freq = 1
		antenna_gain_tx = 1
		antenna_gain_rx = 1
		usrp_gain_tx = 1
		usrp_gain_rx = 1
		amplitude = 1
		corr_factor = 1
		exponent = 1

		Range = (10, 20, 30)
		power = (15, 30, 45)

		pmt_range = pmt.list2(pmt.string_to_symbol('range'),pmt.init_f32vector(len(Range),Range))
		pmt_power = pmt.list2(pmt.string_to_symbol('power'),pmt.init_f32vector(len(power),power))
		pmt_misc = pmt.list2(pmt.string_to_symbol('misc'),pmt.init_f32vector(3,(1,2,3)))
		pmt_in = pmt.list3(pmt_misc,pmt_range,pmt_power)

		src = blocks.message_strobe(pmt_in, 300)
		est = radar.estimator_rcs(num_mean, center_freq, antenna_gain_tx, antenna_gain_rx, usrp_gain_tx, usrp_gain_rx, amplitude, corr_factor, exponent)
		snk = blocks.message_debug()
		self.tb.msg_connect(src,"strobe",est,"Msg in")
		self.tb.msg_connect(est,"Msg out",snk,"store")
		self.tb.msg_connect(est,"Msg out",snk,"print")
		
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()

		# check data

if __name__ == '__main__':
	gr_unittest.run(qa_estimator_rcs)#, "qa_estimator_rcs.xml")
