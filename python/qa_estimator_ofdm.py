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

class qa_estimator_ofdm (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		Range = (2,4,22,23)
		velocity = (3,12,19,19)
		power = (10,10,10,1) # last value is thrown away with merging peaks
		pmt_time = pmt.list2(pmt.string_to_symbol('rx_time'),pmt.make_tuple(pmt.from_long(-1),pmt.from_double(0)))
		pmt_axisx = pmt.list2(pmt.string_to_symbol('axis_x'),pmt.init_f32vector(len(Range),Range))
		pmt_axisy = pmt.list2(pmt.string_to_symbol('axis_y'),pmt.init_f32vector(len(velocity),velocity))
		pmt_power = pmt.list2(pmt.string_to_symbol('power'),pmt.init_f32vector(len(power),power))
		pmt_in = pmt.list4(pmt_time, pmt_axisx, pmt_axisy, pmt_power)

		src = blocks.message_strobe(pmt_in, 300)
		est = radar.estimator_ofdm('range',30,(0,40,-40,10),'velocity',20,(-5,5))
		snk = blocks.message_debug()
		self.tb.msg_connect(src,"strobe",est,"Msg in")
		self.tb.msg_connect(est,"Msg out",snk,"store")
		self.tb.msg_connect(est,"Msg out",snk,"print")

		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()

		# get ref data
		ref_range = (0+40*2/15.0, 0+40*4/15.0, -40+50*(22-15)/15.0)
		ref_velocity = (-5+10*3/20.0, -5+10*12/20.0, -5+10*19/20.0)

		# check data
		msg = snk.get_message(0)
		val_range = pmt.f32vector_elements(pmt.nth(1,pmt.nth(1,msg)))
		val_velocity = pmt.f32vector_elements(pmt.nth(1,pmt.nth(2,msg)))
		print(val_range)
		self.assertFloatTuplesAlmostEqual(val_velocity,ref_velocity,4)
		self.assertFloatTuplesAlmostEqual(val_range,ref_range,4)


if __name__ == '__main__':
	gr_unittest.run(qa_estimator_ofdm)#, "qa_estimator_ofdm.xml")
