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

class qa_estimator_sync_pulse_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 1000
		packet_len = test_len
		pulse_send = (200,300,100)
		pulse_wait = (100,100)
		amplitude = 0.5

		num_skip = 0 # skip samples with skiphead
		num_xcorr = 300 # num of xcorrs to determine delay samples

		src = radar.signal_generator_sync_pulse_c(packet_len,pulse_send,pulse_wait,amplitude,"packet_len")
		head = blocks.head(8,test_len)
		skiphead = blocks.skiphead(8,num_skip)
		est = radar.estimator_sync_pulse_c(num_xcorr,"packet_len")
		res = radar.print_results()
		debug = blocks.message_debug()

		self.tb.connect(src,skiphead,head)
		self.tb.connect((head,0),(est,0)) # TX stream (undelayed but skiped)
		self.tb.connect((src,0),(est,1)) # RX stream (delayed but unskiped)
		self.tb.msg_connect(est,'Msg out',res,'Msg in')
		self.tb.msg_connect(est,'Msg out',debug,'store')
		self.tb.run ()

		# check data
		msg = debug.get_message(0)
		num_skip_est = pmt.to_long(pmt.nth(1,pmt.nth(1,msg)))
		self.assertEqual(num_skip_est,num_skip)


if __name__ == '__main__':
	gr_unittest.run(qa_estimator_sync_pulse_c)#, "qa_estimator_sync_pulse_c.xml")
