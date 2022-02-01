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

import random
import pmt

class qa_os_cfar_2d_vc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up variables
		xlen = 50
		ylen = 100
		test_len = xlen*ylen
		
		samp_compare = (5,10)
		samp_protect = (2,4)
		rel_threshold = 0.78
		mult_threshold = 4
		
		# setup input data
		in_data = [0]*test_len
		for k in range(test_len):
			in_data[k] = random.random()
		
		x0 = 10
		y0 = 10
		in_data[x0+xlen*y0] = 2
		
		x1 = 40
		y1 = 15
		in_data[x1+xlen*y1] = 3
		
		x2 = 41
		y2 = 13
		in_data[x2+xlen*y2] = 3
		
		# set up fg
		src = blocks.vector_source_c(in_data)
		s2v = blocks.stream_to_vector(8,xlen)
		s2ts = blocks.stream_to_tagged_stream(8,xlen,ylen,'packet_len')
		cfar = radar.os_cfar_2d_vc(xlen,samp_compare,samp_protect,rel_threshold,mult_threshold)
		debug = blocks.message_debug()
		
		self.tb.connect(src,s2v,s2ts,cfar)
		self.tb.msg_connect(cfar,"Msg out",debug,"store")
		self.tb.msg_connect(cfar,"Msg out",debug,"print")
		
		self.tb.run ()
		
		# check data
		msg = debug.get_message(0)
		cfar_x0 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),0)
		cfar_x1 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),1)
		cfar_x2 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(1,msg)),2)
		cfar_y0 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(2,msg)),0)
		cfar_y1 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(2,msg)),1)
		cfar_y2 =  pmt.f32vector_ref(pmt.nth(1,pmt.nth(2,msg)),2)
		
		self.assertAlmostEqual(x0,cfar_x0,4)
		self.assertAlmostEqual(x1,cfar_x1,4)
		self.assertAlmostEqual(x2,cfar_x2,4)
		
		self.assertAlmostEqual(y0,cfar_y0,4)
		self.assertAlmostEqual(y1,cfar_y1,4)
		self.assertAlmostEqual(y2,cfar_y2,4)


if __name__ == '__main__':
	gr_unittest.run(qa_os_cfar_2d_vc)#, "qa_os_cfar_2d_vc.xml")
