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
import math

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
		est = radar.estimator_rcs(num_mean, center_freq, antenna_gain_tx, antenna_gain_rx, usrp_gain_rx, amplitude, corr_factor, exponent)
		snk = blocks.message_debug()
		self.tb.msg_connect(src,"strobe",est,"Msg in")
		self.tb.msg_connect(est,"Msg out",snk,"store")
		self.tb.msg_connect(est,"Msg out",snk,"print")
		
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()

		# check data

		msg = snk.get_message(0)

		wavel = 3e8/center_freq
		d_antenna_gain_abs_rx = 10**(antenna_gain_rx/10)
		d_antenna_gain_abs_tx = 10**(antenna_gain_tx/10)
		power_rx = power[0]**exponent / 10**(usrp_gain_rx/10);
		power_tx = amplitude

		rcs_ref = (4*math.pi)**(3)/(d_antenna_gain_abs_rx*d_antenna_gain_abs_tx*wavel**2)*Range[0]**4*power_rx/power_tx*corr_factor

		self.assertAlmostEqual( rcs_ref, pmt.f32vector_ref(pmt.nth(1,(pmt.nth(0,msg))),0), 4 ) # check rcs value

if __name__ == '__main__':
	gr_unittest.run(qa_estimator_rcs)#, "qa_estimator_rcs.xml")
